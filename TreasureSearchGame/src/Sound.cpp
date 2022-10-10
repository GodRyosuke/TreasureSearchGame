#include "Sound.hpp"

Sound::Sound()
	:mType(NORMAL)
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

	// Audio Data読み出し
	// 店内で流れるBGM
	FMOD::Studio::EventDescription* NormalBGMDesc = NULL;
	ERRCHECK(mAudioSystem->getEvent("event:/NormalBGM", &NormalBGMDesc));
	mNormalBGM = NULL;
	ERRCHECK(NormalBGMDesc->createInstance(&mNormalBGM));

	// ゲーム時に流れる音楽
	FMOD::Studio::EventDescription* GameBGMDesc = NULL;
	ERRCHECK(mAudioSystem->getEvent("event:/GameBGM", &GameBGMDesc));
	mGameBGM = NULL;
	ERRCHECK(GameBGMDesc->createInstance(&mGameBGM));
}

void Sound::Update()
{
	ERRCHECK(mAudioSystem->update());
}

void Sound::StartMusic()
{
	switch (mType)
	{
	case Sound::NORMAL:
		ERRCHECK(mNormalBGM->start());
		break;
	case Sound::GAME:
		ERRCHECK(mGameBGM->start());
		break;
	default:
		break;
	}
}

void Sound::StopMusic()
{
	switch (mType)
	{
	case Sound::NORMAL:
		ERRCHECK(mNormalBGM->stop(FMOD_STUDIO_STOP_IMMEDIATE));
		break;
	case Sound::GAME:
		ERRCHECK(mGameBGM->stop(FMOD_STUDIO_STOP_IMMEDIATE));
		break;
	default:
		break;
	}
}