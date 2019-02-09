#ifndef  SKELETON_H
#define SKELETON_H

#include "Core.h"
#include <string>

class Joint;

class Skeleton {
public:
	void Load(std::string &filename);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	static glm::mat4 GetWorldMatrix(int id);
public:
	Joint *mRoot=nullptr;
	static std::vector<Joint*> sOrderedJoints;
#pragma message("TODO : Make use of this!!")
	static std::vector<std::string> sOrderedJointName;
};
#endif // SKELETON_H
