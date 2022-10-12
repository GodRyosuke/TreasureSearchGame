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
		,SELECT
		,CANSEL
		,COUNT_DOWN
		,OPEN_CHST
		,CLERK
		,WALK
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
	FMOD::Studio::EventInstance* mSelect;
	FMOD::Studio::EventInstance* mCansel;
	FMOD::Studio::EventInstance* mCountDown;
	FMOD::Studio::EventInstance* mOpenChest;
	FMOD::Studio::EventInstance* mClerk;
	FMOD::Studio::EventInstance* mWalk;

	Type mType;
};