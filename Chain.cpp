#include "Chain.h"
#include "Joint.h"
#include "BallJoint.h"

void Chain::Init(int numJoints)
{
	Joint* parent = nullptr;
	for (int ii = 0; ii < numJoints; ++ii) {
		auto jnt = std::make_shared<BallJoint>();
		if (ii != 0) {
			// This joint would have 1 DOF
			DoF dof;
			dof.LimitRotation(0.0f, 0.0f);
			jnt->mJointDoF[0] = dof; // Limit rotation in x
			jnt->mJointDoF[1] = dof; // Limit rotation in y
		}
		jnt->mPose = { 0.0f,0.0f,ii*1.0f };
		jnt->mOffset = { 1.0f,0.0f,0.0f };
		jnt->mBoxMin = { 0.0f,-0.1f,-0.1f };
		jnt->mBoxMax = { 1.0f,0.1f,0.1f };
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
	mChain->Update(glm::mat4(1.0));
}

void Chain::Draw(const glm::mat4 & viewProjMtx, uint shader)
{
	mChain->Draw(viewProjMtx, shader);
}
