#include "DoF.h"

void DoF::LimitRotation(float minR, float maxR)
{
	mMaxRot = maxR;
	mMinRot = minR;
}

glm::vec2 DoF::GetLimits() const
{
	return glm::vec2(mMinRot, mMaxRot);
}
