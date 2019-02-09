#include "Skin.h"
#include "Skeleton.h"
#include <assert.h>


Skin::Skin() {
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &IndexBuffer);
}

////////////////////////////////////////////////////////////////////////////////

Skin::~Skin() {
	glDeleteBuffers(1, &IndexBuffer);
	glDeleteBuffers(1, &VertexBuffer);
}


void Skin::Load(std::string &filename)
{
	//Parse positions..
	Tokenizer tokenPos;
	tokenPos.Open(filename.c_str());
	tokenPos.FindToken("positions");
	int numVertices = tokenPos.GetInt();

	//Parse Normals..
	Tokenizer tokenNor;
	tokenNor.Open(filename.c_str());
	tokenNor.FindToken("normals");
	auto temp = tokenNor.GetInt();
	assert((numVertices == temp) && "Whoaa! This is definitely not right!");
	
	//Parse Weights...
	Tokenizer tokenWt;
	tokenWt.Open(filename.c_str());
	tokenWt.FindToken("skinweights");
	temp = tokenWt.GetInt();
	assert((numVertices == temp) && "Whoaa! This is definitely not right!");

	//Skip 
	tokenNor.SkipLine();
	tokenPos.SkipLine();
	tokenWt.SkipLine();

	mVertices.reserve(numVertices);
	mVertWeights.reserve(numVertices);
	for (int ii = 0; ii < numVertices; ++ii) {
		auto nx = tokenNor.GetFloat();
		auto ny = tokenNor.GetFloat();
		auto nz = tokenNor.GetFloat();

		auto px = tokenPos.GetFloat();
		auto py = tokenPos.GetFloat();
		auto pz = tokenPos.GetFloat();

		Vertex* vt = new Vertex;
		vt->SetNormal(glm::vec3(nx, ny, nz));
		vt->SetPostion(glm::vec3(px, py, pz));
		mVertices.emplace_back(vt);

		auto numJ = tokenWt.GetInt();
		std::vector<std::pair<int, float>> wts;
		for (int jj = 0; jj < numJ; ++jj) {
			auto id = tokenWt.GetInt();
			auto wt = tokenWt.GetFloat();
			wts.emplace_back(std::make_pair(id, wt));
		}
		mVertWeights.emplace_back(wts);
	}

	// Now read in the triangles..
	tokenWt.FindToken("triangles");
	auto numTriangles = tokenWt.GetInt();
	mTriangles.reserve(numTriangles);
	tokenWt.SkipLine();
	idx.reserve(numTriangles);
	for (int ii = 0; ii < numTriangles; ++ii) {
		auto v1 = tokenWt.GetInt();
		auto v2 = tokenWt.GetInt();
		auto v3 = tokenWt.GetInt();
		Triangle *t = new Triangle;
		t->mVertices.emplace_back(mVertices[v1]);
		t->mVertices.emplace_back(mVertices[v2]);
		t->mVertices.emplace_back(mVertices[v3]);
		mTriangles.emplace_back(t);
		idx.emplace_back(v1);
		idx.emplace_back(v2);
		idx.emplace_back(v3);
	}

	//Parse Binding Matrix..
	tokenWt.FindToken("bindings");
	auto numMat = tokenWt.GetInt();
	mBindingMat.reserve(numMat);
	mBindingMatInv.reserve(numMat);
	for (int ii = 0; ii < numMat; ++ii) {
		tokenWt.FindToken("matrix");
		tokenWt.SkipLine();
		glm::mat4 bM(0);
		for (int jj = 0; jj < 12; ++jj) {
			bM[jj / 3][jj % 3] = tokenWt.GetFloat();
		}
		bM[3][3] = 1.0f;
		mBindingMat.emplace_back(bM);
		mBindingMatInv.emplace_back(glm::inverse(bM));
	}
	

	tokenNor.Close();
	tokenPos.Close();
	tokenWt.Close();
}

void Skin::Update()
{
	//Reset!
	mFinalVertexData.clear();
	// Compute skinning matrix
	std::vector<glm::mat4> skinMat;
	skinMat.reserve(mBindingMatInv.size());
	for (size_t ii = 0; ii < mBindingMatInv.size(); ++ii) {
		skinMat.emplace_back(Skeleton::GetWorldMatrix(ii)*mBindingMatInv[ii]);
	}

	int ii = 0;
	// Compute Positions and Normals
	mFinalVertexData.reserve(mVertices.size());
	for (const auto& vt : mVertices) {
		glm::vec4 pos{0,0,0,0};
		glm::vec4 nor{ 0,0,0,0 };
		for (const auto& wt : mVertWeights[ii]) {
			// We now have joint Id and matrices.. calc new pos
			pos += wt.second*(skinMat[wt.first] * glm::vec4((vt->GetPosition()).x, (vt->GetPosition()).y, (vt->GetPosition()).z, 1));
			nor += wt.second*(skinMat[wt.first] * glm::vec4((vt->GetNormal()).x, (vt->GetNormal()).y, (vt->GetNormal()).z, 0));
		}
		// re-normalize normal!
		glm::normalize(nor);
		mFinalVertexData.emplace_back(PostCompVertex{ pos, nor });
		++ii;
	}
}

void Skin::Draw(const glm::mat4 &viewProjMtx, uint shader)
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "ViewProjMtx"), 1, false, (float*)&viewProjMtx);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mFinalVertexData.size() * sizeof(PostCompVertex), &mFinalVertexData[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Store index buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(uint), &idx[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(shader);

	// Set up state
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	uint posLoc = 0;
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(PostCompVertex), 0);

	uint normLoc = 1;
	glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(PostCompVertex), (void*)12);

	// Draw geometry
	glDrawElements(GL_TRIANGLES, idx.size(), GL_UNSIGNED_INT, 0);
	// Clean up state
	glDisableVertexAttribArray(normLoc);
	glDisableVertexAttribArray(posLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
}
