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
	void Update();
	void StartMusic(std::string eventName);
	void StopMusic(std::string eventName);
	void ShutDown();

private:
	void LoadEvents(std::string eventNane);

	std::vector<FMOD::Studio::Bank*> mBanks;
	FMOD::Studio::System* mAudioSystem;
	std::unordered_map<std::string, FMOD::Studio::EventInstance*> mEvents;
};