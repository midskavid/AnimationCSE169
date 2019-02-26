#include "SpringDamper.h"
#include "Particle.h"

void SpringDamper::ApplySpringForces()
{
	auto e = mP1->mPosition - mP0->mPosition;
	float l = glm::length(e);
	if (l == 0) {
		// Add a little
		l = 0.0000000001f;
	}

	e = e/l ;
	auto v0 = glm::dot(e, mP0->mVelocity);
	auto v1 = glm::dot(e, mP1->mVelocity);
	auto fsd = -mSpringConstant * (mRestLength - l) - mDampingFactor * (v0 - v1);
	if (std::isnan(fsd) || std::isnan(e.x) || std::isnan(e.y) || std::isnan(e.z))
		return;
	mP0->ApplyForce(fsd*e);
	mP1->ApplyForce(-fsd * e);
}
