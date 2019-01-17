#ifndef DOF_H
#define DOF_H

#include "Core.h"

class DoF {
public:
	void LimitRotation(float min, float max);
	//void SetValue(float x);
	glm::vec2 GetLimits() const;

private:
	float mMinRot = -100000;
	float mMaxRot = 100000;
};
#endif //DOF_H
