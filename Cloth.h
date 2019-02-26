#ifndef CLOTH_H
#define CLOTH_H

#include "SpringDamper.h"
#include "Particle.h"
#include "ClothTriangle.h"
#include "Model.h"

#include <memory>

class Cloth {
public:
	Cloth();
	~Cloth();

	void InitCloth(int width, int height);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);

public:
	uint VertexBuffer;
	uint IndexBuffer;

	int mWidth;
	int mHeight;
	std::vector<std::shared_ptr<Particle>> mParticles;
	std::vector<std::shared_ptr<SpringDamper>> mSpringDampers;
	std::vector<std::shared_ptr<ClothTriangle>> mTriangles;

private:
	std::vector<ModelVertex> mFinalVertexData;
	std::vector<uint> idx;
};
#endif // !CLOTH_H

