#ifndef SHIN_H
#define SKIN_H

#include "Tokenizer.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Model.h"

struct PostCompVertex {
	glm::vec3 pos;
	glm::vec3 nor;
};

class Skin {
public:
	Skin();
	~Skin();
	void Load(std::string &filename);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
private:
	uint VertexBuffer;
	uint IndexBuffer;

	std::vector<Vertex*> mVertices;
	std::vector<std::vector<std::pair<int,float>>> mVertWeights;
	std::vector<Triangle*> mTriangles;
	std::vector<glm::mat4> mBindingMat;
	std::vector<glm::mat4> mBindingMatInv;
	std::vector<PostCompVertex> mFinalVertexData;
	std::vector<uint> idx;
};
#endif // !SHIN_H

