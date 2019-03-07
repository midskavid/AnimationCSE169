#ifndef GOAL_H
#define GOAL_H

#include "Model.h"

class Goal {
public :
	void Update(glm::vec3 pos);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
public :
	glm::mat4 mWorldMtx{ 1.0f };
	glm::vec3 mPosGoal{ 0.0f,0.0f,0.0f };
	glm::vec3 mBoxMin{ -0.09f,-0.09f,-0.09f };
	glm::vec3 mBoxMax{ 0.09,0.09,0.09 };
};
#endif // !GOAL_H

