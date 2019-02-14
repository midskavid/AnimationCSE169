#include "Joint.h"
#include "BallJoint.h"
#include "Skeleton.h"

void Joint::Update(glm::mat4& matrixParent)
{
	MakeLocalMatrix(); // Local Matrix..
	mWorldMtx = matrixParent * mLocalMtx; // Forward Kinematics!!

	for (const auto& chld : mChildrens) { // Propagate down the tree...
		chld->Update(mWorldMtx);
	}
}

void Joint::Load(Tokenizer& token)
{
	token.FindToken("{");
	while (true) {
		char temp[256];
		token.GetToken(temp);
		if (strcmp(temp, "offset") == 0) {
			mOffset.x = token.GetFloat();
			mOffset.y = token.GetFloat();
			mOffset.z = token.GetFloat();
		}
		else if (strcmp(temp, "boxmin") == 0) {
			mBoxMin.x = token.GetFloat();
			mBoxMin.y = token.GetFloat();
			mBoxMin.z = token.GetFloat();
		}
		else if (strcmp(temp, "boxmax") == 0) {
			mBoxMax.x = token.GetFloat();
			mBoxMax.y = token.GetFloat();
			mBoxMax.z = token.GetFloat();
		}
		else if (strcmp(temp, "pose") == 0) {
			mPose.x = token.GetFloat();
			mPose.y = token.GetFloat();
			mPose.z = token.GetFloat();
		}
		else if (strcmp(temp, "balljoint") == 0) {
			auto jnt = std::make_shared<BallJoint>();
			Skeleton::sOrderedJoints.emplace_back(jnt);
			char str[100];
			token.GetToken(str);
			Skeleton::sOrderedJointName.emplace_back(str);
			jnt->Load(token);
			AddChild(jnt);
		}
		else if (strcmp(temp, "rotxlimit") == 0) {
			DoF dof;
			float min = token.GetFloat();// Though it might be tempting to directly pop tokens in function call!!!
			float max = token.GetFloat();// Do not even think of doing it!!! The last would be evaluated first and therefore 
			dof.LimitRotation(min, max); // min and max would be swapped.. Nightmare to debug
			mJointDoF[0] = dof; 
		}
		else if (strcmp(temp, "rotylimit") == 0) {
			DoF dof;
			float min = token.GetFloat();
			float max = token.GetFloat();
			dof.LimitRotation(min, max);
			mJointDoF[1] = dof;
		}
		else if (strcmp(temp, "rotzlimit") == 0) {
			DoF dof;
			float min = token.GetFloat();
			float max = token.GetFloat();
			dof.LimitRotation(min, max);
			mJointDoF[2] = dof;
		}
		else if (strcmp(temp, "}") == 0)
			return;
		else token.SkipLine(); // Unrecognized token
	}
}

void Joint::AddChild(std::shared_ptr<Joint> joint)
{
	mChildrens.emplace_back(joint);
}

void Joint::Draw(const glm::mat4 &viewProjMtx, uint shader)
{
	// Draw child first or parent? 
	for (const auto& chld : mChildrens) {
		chld->Draw(viewProjMtx, shader);
	}

	Model mdl;
	mdl.MakeBox(mBoxMin, mBoxMax);
	mdl.Draw(mWorldMtx, viewProjMtx, shader);

}
