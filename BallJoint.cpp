#include "BallJoint.h"

void BallJoint::MakeLocalMatrix()
{
	glm::mat4 local(1.0);
	local = glm::translate(local, mOffset);
#if 0
	local = glm::rotate(local, glm::clamp(mPose.x, mJointDoF[0].GetLimits().x, mJointDoF[0].GetLimits().y), glm::vec3(1, 0, 0));//about X
	local = glm::rotate(local, glm::clamp(mPose.y, mJointDoF[1].GetLimits().x, mJointDoF[1].GetLimits().y), glm::vec3(0, 1, 0));//about Y
	local = glm::rotate(local, glm::clamp(mPose.z, mJointDoF[2].GetLimits().x, mJointDoF[2].GetLimits().y), glm::vec3(0, 0, 1));//about Z
#else
	local = glm::rotate(local, glm::clamp(mPose.z, mJointDoF[2].GetLimits().x, mJointDoF[2].GetLimits().y), glm::vec3(0, 0, 1));//about Z
	local = glm::rotate(local, glm::clamp(mPose.y, mJointDoF[1].GetLimits().x, mJointDoF[1].GetLimits().y), glm::vec3(0, 1, 0));//about Y
	local = glm::rotate(local, glm::clamp(mPose.x, mJointDoF[0].GetLimits().x, mJointDoF[0].GetLimits().y), glm::vec3(1, 0, 0));//about X
#endif
	mLocalMtx = local;
}
