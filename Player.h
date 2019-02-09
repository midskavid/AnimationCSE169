#ifndef PLAYER_H
#define PLAYER_H

#include "Model.h"

class Skeleton;
class AnimationClip;

class Player {
public:
	Player(Skeleton *skel, float delTime, AnimationClip *anim);
	void Animate();
public:
	float mTime;
private:
	float mDelTime;
	Skeleton *mSkeleton = nullptr;
	AnimationClip *mAnimationClip = nullptr;
};
#endif // !PLAYER_H

