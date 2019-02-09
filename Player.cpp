#include "Player.h"
#include "AnimationClip.h"
#include "Skeleton.h"
#include "Joint.h"
#include <assert.h>

Player::Player(Skeleton * skel, float delTime, AnimationClip *anim)
{
	mSkeleton = skel;
	mDelTime = delTime;
	mAnimationClip = anim;
	mTime = 0.0f;
}

void Player::Animate()
{
	mTime += mDelTime;
	std::vector<float> pose;
	mAnimationClip->Evaluate(mTime, pose);

	// Now pose has all information about DOFs..

	//Handle root translation..
	Joint * jt = Skeleton::sOrderedJoints[0];
	jt->mOffset.x = pose[0];
	jt->mOffset.y = pose[1];
	jt->mOffset.z = pose[2];

	//Now pose all other joints...
	assert((Skeleton::sOrderedJoints.size() * 3 == pose.size()-3) && "Whoaa! THis is definitely not right!");
	for (int ii = 0; ii < Skeleton::sOrderedJoints.size(); ++ii) {
		// update!!!
		Joint * jt = Skeleton::sOrderedJoints[ii];
		jt->mPose.x = pose[3 * (ii + 1)];
		jt->mPose.y = pose[3 * (ii + 1) + 1];
		jt->mPose.z = pose[3 * (ii + 1) + 2];
	}
}
