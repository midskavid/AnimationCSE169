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

public:
	Joint *mRoot=nullptr;
};
#endif // SKELETON_H
