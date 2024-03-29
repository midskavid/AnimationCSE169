#ifndef CHANNEL_H
#define CHANNEL_H

#include "Model.h"
#include "KeyFrame.h"
#include "Tokenizer.h"
#include <memory>

class Channel {
public:

	enum ExtrapolationType {
		CONSTANT,
		LINEAR,
		CYCLE,
		CYCLE_OFFSET,
		BOUNCE
	};

	void Load(Tokenizer &token);
	float Evaluate(float time);
	void PreCompute();
public:
	std::vector<std::shared_ptr<KeyFrame>> mKeyFrames;
	ExtrapolationType mExtrapIn;
	ExtrapolationType mExtrapOut;
};
#endif // !CHANNEL_H

