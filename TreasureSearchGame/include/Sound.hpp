#pragma once

#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "FMOD/common.hpp"

class Sound
{
public:
	Sound();
	enum Type {
		NORMAL
		,GAME
		,NUM_TYPE
	};
	void Update();
	void StartMusic();
	void StopMusic();
	void SetType(const Type type) { mType = type; }

private:
	FMOD::Studio::System* mAudioSystem;
	FMOD::Studio::EventInstance* mNormalBGM;
	FMOD::Studio::EventInstance* mGameBGM;

	Type mType;
};