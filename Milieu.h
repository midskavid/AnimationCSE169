#ifndef MILIEU_H
#define MILIEU_H

#include "Model.h"

class Milieu {
public:
	static glm::vec3 mGravity;
	static glm::vec3 mAirSpeed;
	static glm::vec3 mWall;
	static float mC;
	static float mRho;
};
#endif // !MILIEU_H


