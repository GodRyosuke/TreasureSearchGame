#pragma once

#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "FMOD/common.hpp"
#include <vector>
#include <unordered_map>

class Sound
{
public:
	Sound();
	//enum Type {
	//	NORMAL
	//	,GAME
	//	,SELECT
	//	,CANSEL
	//	,COUNT_DOWN
	//	,OPEN_CHST
	//	,CLERK
	//	,WALK
	//	,NUM_TYPE
	//};
	void Update();
	void StartMusic(std::string eventName);
	void StopMusic(std::string eventName);
	//void SetType(const Type type) { mType = type; }
	void ShutDown();

private:
	void LoadEvents(std::string eventNane);

	std::vector<FMOD::Studio::Bank*> mBanks;
	FMOD::Studio::System* mAudioSystem;
	std::unordered_map<std::string, FMOD::Studio::EventInstance*> mEvents;
	FMOD::Studio::EventInstance* mNormalBGM;
	FMOD::Studio::EventInstance* mGameBGM;
	FMOD::Studio::EventInstance* mSelect;
	FMOD::Studio::EventInstance* mCansel;
	FMOD::Studio::EventInstance* mCountDown;
	FMOD::Studio::EventInstance* mOpenChest;
	FMOD::Studio::EventInstance* mClerk;
	FMOD::Studio::EventInstance* mWalk;

	//Type mType;
};