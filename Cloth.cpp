#include "Cloth.h"
#include "Milieu.h"


Cloth::Cloth()
{
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &IndexBuffer);
}

Cloth::~Cloth()
{
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &IndexBuffer);
}

void Cloth::InitCloth(int width, int height)
{
	mWidth = width;
	mHeight = height;
	const float sep = 1;
	const float df = 0.05;
	const float sc = 1.80f;
	const float df2 = 0.04;
	const float sc2 = 0.100f;
	const float rl = 1;
	mParticles.reserve(mWidth*mHeight);
	
	// Now create particles..
	for (int ii = 0; ii < mHeight; ++ii) {
		for (int jj = 0; jj < mWidth; ++jj) {
			auto p = std::make_shared<Particle>();
			if (ii == 0) {
				p->mIsFixed = true;
			}
			p->mPosition = glm::vec3(jj*sep - width / 2.0f*sep, -ii * sep, 0.0f);
			p->mMass = 0.002f; // cloth would generally weigh 200g.. say 100 particles.. divy up equally
			mParticles.emplace_back(p);
		}
	}

	// Now create triangles..
	mTriangles.reserve(2*(mHeight-1)*(mWidth-1));

	for (int ii = 0; ii < mHeight - 1; ++ii) {
		for (int jj = 0; jj < mWidth - 1; ++jj) {
			auto tr1 = std::make_shared<ClothTriangle>();
			auto tr2 = std::make_shared<ClothTriangle>();
			//First Triangle in cell.. Going in anticlockwise manner
			tr1->mP0 = mParticles[ii*mWidth + jj];
			tr1->mP1 = mParticles[(ii + 1)*mWidth + jj];
			tr1->mP2 = mParticles[(ii + 1)*mWidth + jj + 1];
			// Second Triangle in cell.. Anticlockwise
			tr2->mP0 = mParticles[(ii + 1)*mWidth + jj + 1];
			tr2->mP1 = mParticles[ii*mWidth + jj+1];
			tr2->mP2 = mParticles[ii*mWidth + jj];

			mTriangles.emplace_back(tr1);
			mTriangles.emplace_back(tr2);
		}
	}
#if 1
	// Now create springs... there would be two springs inside each cell
	mSpringDampers.reserve((mWidth - 1)*mHeight + (mHeight - 1)*mWidth + (mWidth - 1)*(mHeight - 1) * 2);
	for (int ii = 0; ii < mHeight; ++ii) {
		for (int jj = 0; jj < mWidth-1; ++jj) {
			auto sp = std::make_shared<SpringDamper>();
			sp->mDampingFactor = df;
			sp->mSpringConstant = sc;
			sp->mRestLength = rl;
			sp->mP0 = mParticles[ii*mWidth + jj];
			sp->mP1 = mParticles[ii*mWidth + jj+1];
			mSpringDampers.emplace_back(sp);
		}
	}
#endif

	for (int ii = 0; ii < mHeight - 1; ++ii) {
		for (int jj = 0; jj < mWidth; ++jj) {
			auto sp = std::make_shared<SpringDamper>();
			sp->mDampingFactor = df;
			sp->mSpringConstant = sc;
			sp->mRestLength = rl;
			sp->mP0 = mParticles[ii*mWidth + jj];
			sp->mP1 = mParticles[(ii+1)*mWidth + jj];
			mSpringDampers.emplace_back(sp);
		}
	}
	// Now diagonals..
#if 1
	for (int ii = 0; ii < mHeight - 1; ++ii) {
		for (int jj = 0; jj < mWidth - 1; ++jj) {
			auto sp1 = std::make_shared<SpringDamper>();
			sp1->mDampingFactor = df;
			sp1->mSpringConstant = sc*.1f;
			sp1->mRestLength = 1.41421356f * rl;
			sp1->mP0 = mParticles[ii*mWidth + jj];
			sp1->mP1 = mParticles[(ii+1)*mWidth + jj+1];
			mSpringDampers.emplace_back(sp1);

			auto sp2 = std::make_shared<SpringDamper>();
			sp2->mDampingFactor = df;
			sp2->mSpringConstant = sc*.1f;
			sp2->mRestLength = 1.41421356f * rl;
			sp2->mP0 = mParticles[ii*mWidth + jj+1];
			sp2->mP1 = mParticles[(ii + 1)*mWidth + jj];
			mSpringDampers.emplace_back(sp2);
		}
	}
#endif

	// Add bending forces...
	
	for (int ii = 0; ii < mHeight; ++ii) {
		for (int jj = 0; jj < mWidth - 2; ++jj) {
			auto sp = std::make_shared<SpringDamper>();
			sp->mDampingFactor = df2;
			sp->mSpringConstant = sc2;
			sp->mRestLength = 2.0f*rl;
			sp->mP0 = mParticles[ii*mWidth + jj];
			sp->mP1 = mParticles[ii*mWidth + jj + 2];
			mSpringDampers.emplace_back(sp);
		}
	}


	for (int ii = 0; ii < mHeight - 2; ++ii) {
		for (int jj = 0; jj < mWidth; ++jj) {
			auto sp = std::make_shared<SpringDamper>();
			sp->mDampingFactor = df2;
			sp->mSpringConstant = sc2;
			sp->mRestLength = 2.0f*rl;
			sp->mP0 = mParticles[ii*mWidth + jj];
			sp->mP1 = mParticles[(ii + 2)*mWidth + jj];
			mSpringDampers.emplace_back(sp);
		}
	}
	// Now diagonals..

	for (int ii = 0; ii < mHeight - 2; ++ii) {
		for (int jj = 0; jj < mWidth - 2; ++jj) {
			auto sp1 = std::make_shared<SpringDamper>();
			sp1->mDampingFactor = df2;
			sp1->mSpringConstant = sc2;
			sp1->mRestLength = 2.0f*1.41421356f * rl;
			sp1->mP0 = mParticles[ii*mWidth + jj];
			sp1->mP1 = mParticles[(ii + 2)*mWidth + jj + 2];
			mSpringDampers.emplace_back(sp1);

			auto sp2 = std::make_shared<SpringDamper>();
			sp2->mDampingFactor = df2;
			sp2->mSpringConstant = sc2;
			sp2->mRestLength = 2.0f*1.41421356f * rl;
			sp2->mP0 = mParticles[ii*mWidth + jj + 2];
			sp2->mP1 = mParticles[(ii + 2)*mWidth + jj];
			mSpringDampers.emplace_back(sp2);
		}
	}

}

void Cloth::Update()
{
	for (int ii = 0; ii < 50; ++ii) {
		float delT = 0.001f;//0.0001f;
		for (const auto &pt : mParticles) {
			// Apply gravity
			pt->ApplyForce(Milieu::mGravity*pt->mMass);
		}

		for (const auto &tr : mTriangles) {
			// Apply air-drag
			tr->ApplyAerodynamicForce(Milieu::mAirSpeed, Milieu::mC, Milieu::mRho);
		}

		for (const auto &sp : mSpringDampers) {
			// Apply spring forces here..
			sp->ApplySpringForces();
		}

		// Update and check for collision...
		for (const auto &pt : mParticles) {
			pt->Update(delT);

			if (pt->mPosition.x > Milieu::mWall.x) {
				pt->mPosition.x -= pt->mPosition.x- Milieu::mWall.x;
				pt->mVelocity.x = -0.8f * pt->mVelocity.x;
				pt->mVelocity.y = (1.0f-0.4f) * pt->mVelocity.y;
				pt->mVelocity.z = (1.0f-0.4f) * pt->mVelocity.z;
			}
			if (pt->mPosition.y < Milieu::mWall.y) {
				pt->mPosition.y -= pt->mPosition.y - Milieu::mWall.y;
				pt->mVelocity.y = -0.8f * pt->mVelocity.y;
				pt->mVelocity.x = (1.0f - 0.4f) * pt->mVelocity.x;
				pt->mVelocity.z = (1.0f - 0.4f) * pt->mVelocity.z;

			}
			if (pt->mPosition.z > Milieu::mWall.z) {
				pt->mPosition.z -= pt->mPosition.z - Milieu::mWall.z;
				pt->mVelocity.z = -0.8f * pt->mVelocity.z;
				pt->mVelocity.y = (1.0f - 0.4f) * pt->mVelocity.y;
				pt->mVelocity.x = (1.0f - 0.4f) * pt->mVelocity.x;

			}
		}
	}

	// Now fill in the vectors to pass to shader..
	// reset
	for (const auto &pt : mParticles) {
		pt->mNormal = glm::vec3{ 0.0f, 0.0f, 0.0f };
	}
	// compute normals..
	for (const auto &tr : mTriangles) {
		tr->ComputeNormal();
	}
	// normalize the vecs..
	for (const auto &pt : mParticles) {
		pt->mNormal = glm::normalize(pt->mNormal);
	}

	// Fill cloth...
	mFinalVertexData.clear();
	idx.clear();
	mFinalVertexData.reserve(2*3*(mHeight-1)*(mWidth-1));
	idx.reserve(2*6 * (mHeight - 1)*(mWidth - 1));
	int count = 0;
	for (int ii = 0; ii < mHeight - 1; ++ii) {
		for (int jj = 0; jj < mWidth - 1; ++jj) {
			mFinalVertexData.emplace_back(ModelVertex{ mParticles[ii*mWidth + jj]->mPosition, mParticles[ii*mWidth + jj]->mNormal });
			mFinalVertexData.emplace_back(ModelVertex{ mParticles[(ii+1)*mWidth + jj]->mPosition, mParticles[(ii + 1)*mWidth + jj]->mNormal });
			mFinalVertexData.emplace_back(ModelVertex{ mParticles[(ii + 1)*mWidth + jj+1]->mPosition, mParticles[(ii + 1)*mWidth + jj+1]->mNormal });
			idx.emplace_back(count++);
			idx.emplace_back(count++);
			idx.emplace_back(count++);

			mFinalVertexData.emplace_back(ModelVertex{ mParticles[(ii + 1)*mWidth + jj + 1]->mPosition, mParticles[(ii + 1)*mWidth + jj + 1]->mNormal });
			mFinalVertexData.emplace_back(ModelVertex{ mParticles[ii*mWidth + jj+1]->mPosition, mParticles[ii*mWidth + jj + 1]->mNormal });
			mFinalVertexData.emplace_back(ModelVertex{ mParticles[ii*mWidth + jj]->mPosition, mParticles[ii*mWidth + jj]->mNormal });
			idx.emplace_back(count++);
			idx.emplace_back(count++);
			idx.emplace_back(count++);

			// for the back side!!
			mFinalVertexData.emplace_back(ModelVertex{ mParticles[ii*mWidth + jj]->mPosition, -mParticles[ii*mWidth + jj]->mNormal });
			mFinalVertexData.emplace_back(ModelVertex{ mParticles[(ii + 1)*mWidth + jj + 1]->mPosition, -mParticles[(ii + 1)*mWidth + jj + 1]->mNormal });
			mFinalVertexData.emplace_back(ModelVertex{ mParticles[(ii + 1)*mWidth + jj]->mPosition, -mParticles[(ii + 1)*mWidth + jj]->mNormal });
			
			idx.emplace_back(count++);
			idx.emplace_back(count++);
			idx.emplace_back(count++);

			mFinalVertexData.emplace_back(ModelVertex{ mParticles[(ii + 1)*mWidth + jj + 1]->mPosition, -mParticles[(ii + 1)*mWidth + jj + 1]->mNormal });
			mFinalVertexData.emplace_back(ModelVertex{ mParticles[ii*mWidth + jj]->mPosition, -mParticles[ii*mWidth + jj]->mNormal });
			mFinalVertexData.emplace_back(ModelVertex{ mParticles[ii*mWidth + jj + 1]->mPosition, -mParticles[ii*mWidth + jj + 1]->mNormal });
			idx.emplace_back(count++);
			idx.emplace_back(count++);
			idx.emplace_back(count++);

		}
	}
}

void Cloth::Draw(const glm::mat4 & viewProjMtx, uint shader)
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "ViewProjMtx"), 1, false, (float*)&viewProjMtx);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mFinalVertexData.size() * sizeof(ModelVertex), &mFinalVertexData[0], GL_DYNAMIC_DRAW);
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
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0);

	uint normLoc = 1;
	glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)12);

	// Draw geometry
	glDrawElements(GL_TRIANGLES, idx.size(), GL_UNSIGNED_INT, 0);
	// Clean up state
	glDisableVertexAttribArray(normLoc);
	glDisableVertexAttribArray(posLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
}
