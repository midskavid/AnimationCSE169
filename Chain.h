#ifndef CHAIN_H
#define CHAIN_H

#include "Model.h"
#include <memory>

class Joint;

class Chain {
public:
	void Init(int numJoints);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);

public:
	std::shared_ptr<Joint> mChain;
	std::vector<std::shared_ptr<Joint>> mJoints;
	bool mConverged = true;
};
#endif // !CHAIN_H

