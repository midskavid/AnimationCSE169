#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "Model.h"

class KeyFrame {
public:
	enum TangentType {
		FLAT,
		LINEAR,
		SMOOTH,
		VALUE
	};

	float mTime;
	float mValue;
	TangentType mTangentIn;
	TangentType mTangentOut;
	float mTangentInVal = 0.0f;
	float mTangentOutVal = 0.0f;
	float mC0 = 0.0f;
	float mC1 = 0.0f;
	float mC2 = 0.0f;
	float mC3 = 0.0f;
};
#endif // !KEYFRAME_H
