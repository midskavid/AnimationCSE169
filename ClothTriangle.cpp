#include "ClothTriangle.h"
#include "Particle.h"

void ClothTriangle::ApplyAerodynamicForce(glm::vec3 & vAir, float c, float rho)
{
	auto vSurf = (mP0->mVelocity + mP1->mVelocity + mP2->mVelocity) / 3.0f;
	auto v = vSurf - vAir;
	auto n = glm::cross(mP1->mPosition - mP0->mPosition, mP2->mPosition - mP0->mPosition);
	auto nCap = glm::normalize(n);
	auto ar = 0.5f*(glm::length(n));
	ar = ar * glm::dot(v, nCap) / glm::length(v);

	auto fAero = -0.5f*rho*v.length()*glm::length(v)*c*ar*nCap;
	if (std::isnan(fAero.x) || std::isnan(fAero.y) || std::isnan(fAero.z))
		return;
	mP0->ApplyForce(fAero / 3.0f);
	mP1->ApplyForce(fAero / 3.0f);
	mP2->ApplyForce(fAero / 3.0f);
}

void ClothTriangle::ComputeNormal()
{
	auto n = glm::cross(mP1->mPosition - mP0->mPosition, mP2->mPosition - mP0->mPosition);
	auto nCap = glm::normalize(n);
	if (std::isnan(nCap.x) || std::isnan(nCap.y) || std::isnan(nCap.z))
		return;
	// A vertex would fall on many triangles.. we need to vector sum normals on a vertex coming from each...
	mP0->mNormal += nCap; 
	mP1->mNormal += nCap;
	mP2->mNormal += nCap;
}
