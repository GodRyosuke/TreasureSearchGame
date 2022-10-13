#include "Sound.hpp"

Sound::Sound()
{
	void* extraDriverData = NULL;
	Common_Init(&extraDriverData);

	mAudioSystem = NULL;
	ERRCHECK(FMOD::Studio::System::create(&mAudioSystem));

	// The example Studio project is authored for 5.1 sound, so set up the system output mode to match
	FMOD::System* coreSystem = NULL;
	ERRCHECK(mAudioSystem->getCoreSystem(&coreSystem));
	ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));

	ERRCHECK(mAudioSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData));

	// サウンドデータ読み込み
	FMOD::Studio::Bank* masterBank = NULL;
	ERRCHECK(mAudioSystem->loadBankFile(Common_MediaPath("./resources/Master.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));

	FMOD::Studio::Bank* stringsBank = NULL;
	ERRCHECK(mAudioSystem->loadBankFile(Common_MediaPath("./resources/Master.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
	mBanks.push_back(masterBank);
	mBanks.push_back(stringsBank);

	// Audio Data読み出し
	// 店内で流れるBGM
	LoadEvents("event:/NormalBGM");
	LoadEvents("event:/GameBGM");
	LoadEvents("event:/Select");
	LoadEvents("event:/Cansel");
	LoadEvents("event:/CountDown");
	LoadEvents("event:/OpenChest");
	LoadEvents("event:/Clerk");
	LoadEvents("event:/Walk");
}

void Sound::LoadEvents(std::string eventName)
{
	FMOD::Studio::EventDescription* eventDesc = NULL;
	ERRCHECK(mAudioSystem->getEvent(eventName.c_str(), &eventDesc));
	FMOD::Studio::EventInstance* eventInstance = NULL;
	ERRCHECK(eventDesc->createInstance(&eventInstance));

	if (eventInstance) {
		mEvents.emplace(eventName, eventInstance);
	}
	else {
		delete eventInstance;
		eventInstance = nullptr;
	}
}

void Sound::Update()
{
	ERRCHECK(mAudioSystem->update());
}

void Sound::StartMusic(std::string eventName)
{
	auto iter = mEvents.find(eventName);
	if (iter != mEvents.end()) {
		iter->second->start();
	}
}

void Sound::StopMusic(std::string eventName)
{
	auto iter = mEvents.find(eventName);
	if (iter != mEvents.end()) {
		iter->second->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	}
}

void Sound::ShutDown()
{
	// Unload Bank
	for (auto& iter : mBanks)
	{
		// Unload the sample data, then the bank itself
		iter->unloadSampleData();
		iter->unload();
	}
	mBanks.clear();

	// Unload Events
	mEvents.clear();
	mAudioSystem->unloadAll();
}