#include "AnimationClip.h"
#include "Tokenizer.h"
#include <assert.h>

void AnimationClip::Load(std::string &filename)
{
	Tokenizer tokenPos;
	tokenPos.Open(filename.c_str());
	tokenPos.FindToken("animation");
	tokenPos.FindToken("range");
	auto tMin = tokenPos.GetFloat();
	auto tMax = tokenPos.GetFloat();
	tokenPos.FindToken("numchannels");
	auto numChannels = tokenPos.GetInt();

	// Now parse channels..
	mChannels.reserve(numChannels);
	for (int ii = 0; ii < numChannels; ++ii) {
		auto ch = std::make_shared<Channel>();
		ch->Load(tokenPos);
		mChannels.emplace_back(ch);
	}
	tokenPos.Close();
}

void AnimationClip::Evaluate(float time, std::vector<float> &pose)
{
	pose.reserve(mChannels.size());
	for (const auto &ch : mChannels) {
		pose.emplace_back(ch->Evaluate(time));
	}
}
