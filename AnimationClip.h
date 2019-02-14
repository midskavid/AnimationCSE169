#ifndef ANIMATIONCLIP_H
#define ANIMATIONCLIP_H

#include "Model.h"
#include "Channel.h"
#include <memory>

class AnimationClip {
public:
	void Load(std::string &filename);
	void Evaluate(float time, std::vector<float> &pose);
public:
	std::vector<std::shared_ptr<Channel>> mChannels;
	glm::vec2 mTimeRange;
};
#endif // !ANIMATIONCLIP_H

