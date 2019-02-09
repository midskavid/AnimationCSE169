#ifndef ANIMATIONCLIP_H
#define ANIMATIONCLIP_H

#include "Model.h"
#include "Channel.h"

class AnimationClip {
public:
	void Load(std::string &filename);
	void Evaluate(float time, std::vector<float> &pose);
public:
	std::vector<Channel*> mChannels;
	glm::vec2 mTimeRange;
};
#endif // !ANIMATIONCLIP_H

