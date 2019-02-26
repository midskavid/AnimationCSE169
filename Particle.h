#ifndef PARTICLE_H
#define PARTICLE_H

#include "Model.h"

class Particle {
public:
	void Update(float delT);
	void ApplyForce(glm::vec3 &force);
public:
	glm::vec3 mPosition{0.0f, 0.0f, 0.0f};
	glm::vec3 mNormal{0.0f, 0.0f, 0.0f};
	glm::vec3 mForce{0.0f, 0.0f, 0.0f};
	glm::vec3 mVelocity{0.0f, 0.0f, 0.0f};
	float mMass;
	bool mIsFixed = false;
};
#endif // !PARTICLE_H

