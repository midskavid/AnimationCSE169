#include "Goal.h"

void Goal::Update(glm::vec3 pos)
{
	mPosGoal = mPosGoal + pos;
	mWorldMtx = glm::translate(glm::mat4(1.0f), mPosGoal);
}

void Goal::Draw(const glm::mat4 & viewProjMtx, uint shader)
{
	Model mdl;
	mdl.MakeBox(mBoxMin, mBoxMax);
	mdl.Draw(mWorldMtx, viewProjMtx, shader);
}
