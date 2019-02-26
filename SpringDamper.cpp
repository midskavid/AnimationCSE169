#include "SpringDamper.h"
#include "Particle.h"

void SpringDamper::ApplySpringForces()
{
#if 1
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
#else
	auto e = (mP1->mPosition - mP0->mPosition);
	if (glm::length(e) == 0) {
		e = mP1->mPosition + glm::vec3(0.001f, 0.001f, 0.001f) - mP0->mPosition;
	}

	float   x = glm::length(mP1->mPosition - mP0->mPosition) - mRestLength;
	float   v = glm::dot(mP1->mVelocity, e) - glm::dot(mP0->mVelocity, e);

	if (std::isnan(x) || std::isnan(v) || std::isnan(e.x) || std::isnan(e.y) || std::isnan(e.z))
		return;

	mP0->ApplyForce((-mSpringConstant * x - mDampingFactor * v) * -e);
	mP1->ApplyForce((-mSpringConstant * x - mDampingFactor * v) * e);
#endif
}
