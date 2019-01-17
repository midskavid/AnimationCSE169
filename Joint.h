#ifndef JOINT_H
#define JOINT_H

#include "DoF.h"
#include "Core.h"
#include "Tokenizer.h"
#include "Model.h"

#include <vector>

class Joint {
public:
	virtual void MakeLocalMatrix()=0;

	void Update(glm::mat4& matrixParent);
	void Load(Tokenizer& token);
	void AddChild(Joint *joint);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
public:
	std::vector<DoF> mJointDoF = std::vector<DoF>(3);
	glm::mat4 mWorldMtx;
	glm::mat4 mLocalMtx;

	// Tree Info
	Joint *mParent=nullptr;
	std::vector<Joint *>mChildrens;
	glm::vec3 mOffset{0,0,0};
	glm::vec3 mBoxMin{-0.1,-0.1,-0.1};
	glm::vec3 mBoxMax{ 0.1,0.1,0.1 };
	glm::vec3 mPose;
};
#endif //JOINT_H
