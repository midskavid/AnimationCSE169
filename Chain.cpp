#include "Chain.h"
#include "Joint.h"
#include "BallJoint.h"
#include "Goal.h"

void Chain::Init(int numJoints)
{
	//numJoints = 2;
	Joint* parent = nullptr;
	for (int ii = 0; ii < numJoints; ++ii) {
		auto jnt = std::make_shared<BallJoint>();
#pragma message("FOR NOW ALL HAVE 1DOF")
		if (ii != -1) {
			// This joint would have 1 DOF
			DoF dof;
			dof.LimitRotation(0.0f, 0.0f);
			jnt->mJointDoF[0] = dof; // Limit rotation in x
			jnt->mJointDoF[1] = dof; // Limit rotation in y
		}
		jnt->mPose = { 0.0f,0.0f,0.0f };
		jnt->mOffset = { 1.0f,0.0f,0.0f };
		jnt->mBoxMin = { 0.0f,-0.1f,-0.1f };
		jnt->mBoxMax = { 1.0f,0.1f,0.1f };
		mJoints.emplace_back(jnt);
		if (ii == 0) {
			mChain = jnt;
			parent = jnt.get();
		}
		else {
			parent->AddChild(jnt);
			parent = jnt.get();
		}
	}
}

void Chain::Update()
{
	auto goal = Goal::mPosGoal;
	float TOLERANCE = 0.000001f;
	mChain->Update(glm::mat4(1.0));
#if 1
	// calculate endaffector position...
	auto lstJnt = mJoints.back();
	glm::vec4 eLocal = { 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec3 endAffec = lstJnt->mWorldMtx*eLocal;
	float beta = 0.0001f;
	auto delE = beta * (goal - endAffec);

	// Need an if check here to short-circuit if endeffector is near enough or stretched max
	for (int ii = 0; ii < 10; ++ii) {
		for (const auto& jnt : mJoints) {
			glm::vec3 a = jnt->mParentWorldMtx * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f };
			glm::vec3 r = jnt->mParentWorldMtx * glm::vec4{ jnt->mOffset, 1.0f };
			a = glm::normalize(a);
			auto delEdelphi = glm::cross(a, (endAffec - r));
			auto delPhi = glm::dot(delEdelphi, delE);
			jnt->mPose.z += delPhi;
		}
		mChain->Update(glm::mat4(1.0));
	}
#endif
}

void Chain::Draw(const glm::mat4 & viewProjMtx, uint shader)
{
	mChain->Draw(viewProjMtx, shader);
}
