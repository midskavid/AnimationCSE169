#include "Skeleton.h"
#include "Tokenizer.h"
#include "Joint.h"
#include "BallJoint.h"

void Skeleton::Load(std::string &filename)
{
	Tokenizer token;
	token.Open(filename.c_str());
	token.FindToken("balljoint");
	
	mRoot = new BallJoint;
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
