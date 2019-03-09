#include "Chain.h"
#include "Joint.h"
#include "BallJoint.h"
#include "Goal.h"
#include <algorithm>

void Chain::Init(int numJoints)
{
	//numJoints = 2;
	Joint* parent = nullptr;
	for (int ii = 0; ii < numJoints; ++ii) {
		auto jnt = std::make_shared<BallJoint>();
#pragma message("FOR NOW ALL HAVE 1DOF")
		if (ii != 0) {
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
	float THRESH = 0.0001f;
	mChain->Update(glm::mat4(1.0));
	// calculate endaffector position...
	auto lstJnt = mJoints.back();
	std::vector<float> delPhiAll(mJoints.size() + 2);
	glm::vec4 eLocal = { 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec3 endAffec = lstJnt->mWorldMtx*eLocal;
	float beta = 0.0001f;
	auto delE = goal - endAffec;

	// Need an if check here to short-circuit if endeffector is near enough or stretched max
	for (int ii = 0; ii < 10; ++ii) {
		int count = 0;
		float maxDelPhi = 0.0f;
		delPhiAll.clear();
		for (const auto& jnt : mJoints) {
			if (count == 0) {
				// Base joint.. Handle separately as this has 3 DOFs...
				++count;
				glm::vec3 r = jnt->mParentWorldMtx * glm::vec4{ jnt->mOffset, 1.0f };

				// X
				glm::mat4 local(1.0);
				local = glm::rotate(local, glm::clamp(jnt->mPose.z, jnt->mJointDoF[2].GetLimits().x, jnt-> mJointDoF[2].GetLimits().y), glm::vec3(0, 0, 1));//about Z
				local = glm::rotate(local, glm::clamp(jnt->mPose.y, jnt->mJointDoF[1].GetLimits().x, jnt->mJointDoF[1].GetLimits().y), glm::vec3(0, 1, 0));//about Y
				glm::vec3 a = jnt->mParentWorldMtx * local * glm::vec4{ 1.0f, 0.0f, 0.0f, 0.0f };				
				a = glm::normalize(a);
				auto delEdelphi = glm::cross(a, (endAffec - r));
				auto delPhi = glm::dot(delEdelphi, delE);
				delPhiAll.emplace_back(delPhi);
				if (std::abs(delPhi) > maxDelPhi)
					maxDelPhi = std::abs(delPhi);
				//jnt->mPose.x += delPhi;

				// Y
				glm::mat4 local1(1.0);
				local1 = glm::rotate(local1, glm::clamp(jnt->mPose.z, jnt->mJointDoF[2].GetLimits().x, jnt->mJointDoF[2].GetLimits().y), glm::vec3(0, 0, 1));//about Z
				a = jnt->mParentWorldMtx * local1 * glm::vec4{ 0.0f, 1.0f, 0.0f, 0.0f };
				a = glm::normalize(a);
				delEdelphi = glm::cross(a, (endAffec - r));
				delPhi = glm::dot(delEdelphi, delE);
				delPhiAll.emplace_back(delPhi);
				if (std::abs(delPhi) > maxDelPhi)
					maxDelPhi = std::abs(delPhi);
				//jnt->mPose.y += delPhi;

				// Z
				a = jnt->mParentWorldMtx * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f };
				a = glm::normalize(a);
				delEdelphi = glm::cross(a, (endAffec - r));
				delPhi = glm::dot(delEdelphi, delE);
				delPhiAll.emplace_back(delPhi);
				if (std::abs(delPhi) > maxDelPhi)
					maxDelPhi = std::abs(delPhi);
				//jnt->mPose.z += delPhi;

			}
			else {
				++count;
				glm::vec3 a = jnt->mParentWorldMtx * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f };
				glm::vec3 r = jnt->mParentWorldMtx * glm::vec4{ jnt->mOffset, 1.0f };
				a = glm::normalize(a);
				auto delEdelphi = glm::cross(a, (endAffec - r));
				auto delPhi = glm::dot(delEdelphi, delE);
				delPhiAll.emplace_back(delPhi);
				if (std::abs(delPhi) > maxDelPhi)
					maxDelPhi = std::abs(delPhi);
				//jnt->mPose.z += delPhi;
			}
		}

		beta = THRESH / std::max(THRESH, maxDelPhi);
		// Now update Joint Pose...
		count = 0;
		for (const auto& jnt : mJoints) {
			if (count == 0) {
				jnt->mPose.x += beta * delPhiAll[count++];
				jnt->mPose.y += beta * delPhiAll[count++];
				jnt->mPose.z += beta * delPhiAll[count++];
			}
			else {
				jnt->mPose.z += beta * delPhiAll[count++];
			}
		}
		mChain->Update(glm::mat4(1.0));
	}
}

void Chain::Draw(const glm::mat4 & viewProjMtx, uint shader)
{
	mChain->Draw(viewProjMtx, shader);
}
