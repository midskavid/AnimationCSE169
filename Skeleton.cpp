#include "Skeleton.h"
#include "Tokenizer.h"
#include "Joint.h"
#include "BallJoint.h"
#include <string>

std::vector<Joint*> Skeleton::sOrderedJoints;
std::vector<std::string> Skeleton::sOrderedJointName;

void Skeleton::Load(std::string &filename)
{
	Tokenizer token;
	token.Open(filename.c_str());
	token.FindToken("balljoint");
	char str[100];
	token.GetToken(str);
	mRoot = new BallJoint;
	sOrderedJoints.emplace_back(mRoot);
	sOrderedJointName.emplace_back(str);
	mRoot->Load(token);

	token.Close();
}

void Skeleton::Update()
{
	mRoot->Update(glm::mat4(1.0));
}

void Skeleton::Draw(const glm::mat4 &viewProjMtx, uint shader)
{
	mRoot->Draw(viewProjMtx, shader);
}

glm::mat4 Skeleton::GetWorldMatrix(int id)
{
	return sOrderedJoints[id]->mWorldMtx;
}
