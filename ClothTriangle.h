#ifndef CLOTHTRIANGLE_H
#define CLOTHTRIANGLE_H

#include "Model.h"
#include <memory>

class Particle;

class ClothTriangle {
public:
	void ApplyAerodynamicForce(glm::vec3 &vAir, float c, float rho);
	void ComputeNormal();
public:
	std::shared_ptr<Particle> mP0;
	std::shared_ptr<Particle> mP1;
	std::shared_ptr<Particle> mP2;
};
#endif // !CLOTHTRIANGLE_H

