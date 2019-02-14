#include "Channel.h"

void Channel::Load(Tokenizer &token)
{
	token.FindToken("channel");
	token.FindToken("extrapolate");
	char s1[100], s2[100];
	token.GetToken(s1);
	token.GetToken(s2);

	if (strcmp(s1, "constant") == 0) {
		mExtrapIn = ExtrapolationType::CONSTANT;
	}
	else if (strcmp(s1, "linear") == 0) {
		mExtrapIn = ExtrapolationType::LINEAR;
	}
	else if (strcmp(s1, "cycle") == 0) {
		mExtrapIn = ExtrapolationType::CYCLE;
	}
	else if (strcmp(s1, "cycle_offset") == 0) {
		mExtrapIn = ExtrapolationType::CYCLE_OFFSET;
	}
	else {
		mExtrapIn = ExtrapolationType::BOUNCE;
	}

	if (strcmp(s2, "constant") == 0) {
		mExtrapOut = ExtrapolationType::CONSTANT;
	}
	else if (strcmp(s2, "linear") == 0) {
		mExtrapOut = ExtrapolationType::LINEAR;
	}
	else if (strcmp(s2, "cycle") == 0) {
		mExtrapOut = ExtrapolationType::CYCLE;
	}
	else if (strcmp(s2, "cycle_offset") == 0) {
		mExtrapOut = ExtrapolationType::CYCLE_OFFSET;
	}
	else {
		mExtrapOut = ExtrapolationType::BOUNCE;
	}
	token.FindToken("keys");
	auto numKeys = token.GetInt();
	mKeyFrames.reserve(numKeys);
	token.SkipLine();
	for (int ii = 0; ii < numKeys; ++ii) {
		// Parse key...
		auto kf = std::make_shared<KeyFrame>();
		kf->mTime = token.GetFloat();
		kf->mValue = token.GetFloat();
		char sv1[100], sv2[100];
		token.GetToken(sv1);
		token.GetToken(sv2);

		if (strcmp(sv1, "flat") == 0) {
			kf->mTangentIn = KeyFrame::FLAT;
		}
		else if (strcmp(sv1, "linear") == 0) {
			kf->mTangentIn = KeyFrame::LINEAR;
		}
		else if (strcmp(sv1, "smooth") == 0) {
			kf->mTangentIn = KeyFrame::SMOOTH;
		}
		else {
			kf->mTangentIn = KeyFrame::VALUE;
#pragma message("Convert sv1 to float!!!")
		}
		
		if (strcmp(sv2, "flat") == 0) {
			kf->mTangentOut = KeyFrame::FLAT;
		}
		else if (strcmp(sv2, "linear") == 0) {
			kf->mTangentOut = KeyFrame::LINEAR;
		}
		else if (strcmp(sv2, "smooth") == 0) {
			kf->mTangentOut = KeyFrame::SMOOTH;
		}
		else {
			kf->mTangentOut = KeyFrame::VALUE;
#pragma message("Convert sv2 to float!!!")
		}
		mKeyFrames.emplace_back(kf);
	}
	// Calculate co-efficients!!
	PreCompute();
}



float Channel::Evaluate(float time)
{
	float duration = mKeyFrames[mKeyFrames.size()-1]->mTime - mKeyFrames[0]->mTime;
	float offsetJump = mKeyFrames[mKeyFrames.size() - 1]->mValue - mKeyFrames[0]->mValue;
	int offsetCycles = 0;

	if (time < mKeyFrames[0]->mTime) {
		switch (mExtrapIn)
		{
		case Channel::CONSTANT:
			return mKeyFrames[0]->mValue;
		case Channel::LINEAR:
			return -mKeyFrames[0]->mTangentOut*(mKeyFrames[0]->mTime-time) + mKeyFrames[0]->mValue;
		case Channel::CYCLE:
			time = mKeyFrames[0]->mTime + duration - std::fmod(mKeyFrames[0]->mTime - time, duration);
			break;
		case Channel::CYCLE_OFFSET:
			offsetCycles = (int)std::ceil((mKeyFrames[0]->mTime - time) / duration);
			time = mKeyFrames[0]->mTime + duration - std::fmod(mKeyFrames[0]->mTime - time, duration);
			break;
		case Channel::BOUNCE:
			if ((int)std::ceil((mKeyFrames[0]->mTime - time) / duration) % 2 == 0) {
				time = mKeyFrames[0]->mTime + duration - std::fmod(-time + mKeyFrames[0]->mTime, duration);
			}
			else {
				time = duration - (mKeyFrames[0]->mTime + duration - std::fmod(-time + mKeyFrames[0]->mTime, duration));
			}
			break;
		}
	}
	else if (time > mKeyFrames[mKeyFrames.size() - 1]->mTime) {
		switch (mExtrapOut)
		{
		case Channel::CONSTANT:
			return mKeyFrames.back()->mValue;
		case Channel::LINEAR:
			return mKeyFrames.back()->mTangentIn*(time - mKeyFrames[0]->mTime) + mKeyFrames.back()->mValue;
		case Channel::CYCLE:
			time = mKeyFrames[0]->mTime + std::fmod(time - mKeyFrames[0]->mTime, duration);
		case Channel::CYCLE_OFFSET:
			offsetCycles = (int)std::floor((time - mKeyFrames[0]->mTime) / duration);
			time = mKeyFrames[0]->mTime + std::fmod(time - mKeyFrames[0]->mTime, duration);
			break;
		case Channel::BOUNCE:
			if ((int)std::floor((time - mKeyFrames[0]->mTime) / duration) % 2 == 0) {
				time = duration - (mKeyFrames[0]->mTime + std::fmod(time - mKeyFrames[0]->mTime, duration));
			}
			else {
				time = mKeyFrames[0]->mTime + std::fmod(time - mKeyFrames[0]->mTime, duration);
			}
			break;
		}
	}

	// Now evaluate at time... taking offset into consideration...
	// do a O(n) search for now...

	for (size_t ii = 0; ii < mKeyFrames.size() - 1; ++ii) {// ignore the last key point!
		if (time < mKeyFrames[ii]->mTime || time > mKeyFrames[ii + 1]->mTime)
			continue;
		// Find u
		if (time == mKeyFrames[ii]->mTime)
			return mKeyFrames[ii]->mValue;
		if (time == mKeyFrames[ii + 1]->mTime)
			return mKeyFrames[ii + 1]->mTime;

		float u = (time - mKeyFrames[ii]->mTime) / (mKeyFrames[ii + 1]->mTime - mKeyFrames[ii]->mTime);
		return mKeyFrames[ii]->mC3 + u * (mKeyFrames[ii]->mC2 + u * (mKeyFrames[ii]->mC1 + u * mKeyFrames[ii]->mC0)) + offsetCycles * offsetJump;
	}
	return 0.0f;
}

void Channel::PreCompute()
{
	if (mKeyFrames.size() > 1) { // taking tangents 0 else
		for (size_t ii = 0; ii < mKeyFrames.size(); ++ii) {
			const auto &kfCur = mKeyFrames[ii];
			switch (kfCur->mTangentIn)
			{
			case KeyFrame::FLAT: {
				kfCur->mTangentInVal = 0.0f;
				break;
			}
			case KeyFrame::LINEAR: {
				// check if first..
				if (ii == 0)
					kfCur->mTangentInVal = 0.0f;
				else {
					kfCur->mTangentInVal = (mKeyFrames[ii]->mValue - mKeyFrames[ii - 1]->mValue) / (mKeyFrames[ii]->mTime - mKeyFrames[ii - 1]->mTime);
					mKeyFrames[ii - 1]->mTangentOutVal = kfCur->mTangentInVal;
				}
				break;
			}
			case KeyFrame::SMOOTH: {
				if (ii == 0) {
					kfCur->mTangentInVal = 0.0f;
					kfCur->mTangentOutVal = (mKeyFrames[ii + 1]->mValue - mKeyFrames[ii]->mValue) / (mKeyFrames[ii + 1]->mTime - mKeyFrames[ii]->mTime);
				}
				else if (ii == mKeyFrames.size() - 1) {
					kfCur->mTangentOutVal = 0.0f;
					kfCur->mTangentInVal = (mKeyFrames[ii]->mValue - mKeyFrames[ii - 1]->mValue) / (mKeyFrames[ii]->mTime - mKeyFrames[ii - 1]->mTime);
				}
				else {
					kfCur->mTangentInVal = (mKeyFrames[ii + 1]->mValue - mKeyFrames[ii - 1]->mValue) / (mKeyFrames[ii + 1]->mTime - mKeyFrames[ii - 1]->mTime);
					kfCur->mTangentOutVal = kfCur->mTangentInVal;
				}
				break;
			}
			}
		}
		// Now calculate coefficients...


		for (size_t ii = 0; ii < mKeyFrames.size() - 1; ++ii) {
			const auto &kfCur = mKeyFrames[ii];
			const auto &kfNext = mKeyFrames[ii + 1];

			float scaledV0 = kfCur->mTangentOutVal*(kfNext->mTime - kfCur->mTime);
			float scaledV1 = kfNext->mTangentInVal*(kfNext->mTime - kfCur->mTime);

			kfCur->mC0 = 2.0f * kfCur->mValue - 2.0f * kfNext->mValue + scaledV0 + scaledV1;
			kfCur->mC1 = -3.0f * kfCur->mValue + 3.0f * kfNext->mValue - 2.0f*scaledV0 - scaledV1;
			kfCur->mC2 = scaledV0;
			kfCur->mC3 = kfCur->mValue;
		}

	}
}
