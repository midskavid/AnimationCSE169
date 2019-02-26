#include "Particle.h"

void Particle::Update(float delT)
{
	if (!mIsFixed) {
		glm::vec3 a = (1.0f/mMass)*mForce;
		mVelocity = mVelocity + a * delT;
		mPosition = mPosition + mVelocity * delT;
		mForce = glm::vec3(0.0f);
	}
}

void Particle::ApplyForce(glm::vec3 & force)
{
	mForce += force;
}
