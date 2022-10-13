#include "Actor/TalkText.hpp"
#include "Component/TalkTextComponent.hpp"
#include "Game.hpp"
#include "Actor/TreasureBox.hpp"
#include "Sound.hpp"
#include "Actor/Clerk.hpp"
#include <fstream>
#include <codecvt>



TalkText::TalkText(Game* game)
	:Actor(game)
	,mTalkIdx(0)
	,mIsSelectText(false)
{
	mTalkTextComp = new TalkTextComponent(this);
	mTalkTextComp->SetIsDraw(false);

	// ��b�Ɏg��������ǂݍ���
	{
		std::string filePath = "./resources/TextData.json";
		std::ifstream ifs(filePath.c_str());
		if (ifs.good())
		{
			ifs >> mData;
		}
		ifs.close();
	}

	mTalkData = std::vector<nl::json>{
		mData["TalkCostomer"]["Welcome"]["Talk1"],
		mData["TalkCostomer"]["Welcome"]["Talk2"],
		mData["TalkCostomer"]["Welcome"]["Talk3"],
		mData["TalkCostomer"]["Bye"]["Talk1"],
		mData["TalkCostomer"]["Explain"]["Talk1"],
		mData["TalkCostomer"]["Explain"]["Talk2"],
		mData["TalkCostomer"]["Explain"]["Talk3"],
		mData["TalkCostomer"]["FailGame"]["Talk1"],
		mData["GameResult"]["Food"]["Result1"],
		mData["GameResult"]["Food"]["Result2"],
		mData["GameResult"]["Food"]["Result3"],
		mData["GameResult"]["Treasure"]["Result1"],
		mData["GameResult"]["Treasure"]["Result2"],
	};
}

void TalkText::UpdateActor(float deltatime)
{
	Player::State playerState = GetGame()->GetPlayer()->GetState();

	if ((playerState == Player::TALK) && (mPreviousPlayerState != Player::TALK)) {
		if (GetGame()->GetPhase() == Game::PHASE_FAIL_GAME) {
			// �Q�[�����s
			mTalkIdx = 7;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
		}
		else if (GetGame()->GetPhase() == Game::PHASE_SUCCSESS_GAME) {
			// �Q�[���N���A
			mTalkIdx = 8 + mResultIdx;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
		}
		else {
			// �X���ɘb��������
			mTalkIdx = 0;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
			mTalkTextComp->SetIsDraw(true);
			SetPosition(glm::vec3(0.0f, -150.0f, 0.f));
			mTalkTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
			GetGame()->GetSound()->StartMusic("event:/Clerk");
		}
	}

	// �I����ʂȂ�
	if (mTalkIdx == 2) {
		mIsSelectText = true;
	}
	else {
		mIsSelectText = false;
	}

	if (playerState == Player::TALK) {
		mTalkTextComp->SetIsDraw(true);
	}
	else {
		mTalkTextComp->SetIsDraw(false);
	}


	mPreviousPlayerState = playerState;
}

void TalkText::ActorInput(const uint8_t* keyState)
{
	Player::State playerState = GetGame()->GetPlayer()->GetState();
	Sound* sound = GetGame()->GetSound();
	// �`�悪�I����Ă���G���^�[�L�[���������ꂽ��
	if ((playerState == Player::TALK) && (mTalkTextComp->GetIsFinishDraw())) {
		if ((mTalkIdx == 2)&&(keyState[SDL_SCANCODE_2])) {
			// �܂����Ă˕`��
			mTalkIdx = 3;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
			GetGame()->GetClerk()->BowToPlayer();
			sound->StartMusic("event:/Cansel");
		}
		else if ((mTalkIdx == 2) && (keyState[SDL_SCANCODE_1])) {
			// �Q�[��������ƑI��
			mTalkIdx = 4;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
			sound->StartMusic("event:/Select");
		}
		else if ((mTalkIdx == 3) && (keyState[SDL_SCANCODE_RETURN])) {
			// �܂����Ă˕`��I��
			// player��Talk����Idle�Ɉڍs
			mTalkIdx = 0;
			GetGame()->GetPlayer()->SetState(Player::IDLE);
			GetGame()->GetPlayer()->WaitSeconds(1000);
			sound->StartMusic("event:/Select");
		}
		else if ((mTalkIdx == 6) && (keyState[SDL_SCANCODE_RETURN])) {
			// �Q�[���J�n�̂��߁ATALK�I���
			mTalkIdx = 0;
			GetGame()->SetPhase(Game::PHASE_GAME);	// �Q�[���X�^�[�g
			GetGame()->GetPlayer()->SetState(Player::IDLE);
			// ���H������
			GetGame()->SetLevel();
			// �󔠂̈ʒu�Ȃǐݒ�
			GetGame()->GetTreasureBox()->Init();
			// �󔠂̒��g����
			mResultIdx = GetGame()->GetRandom() % 5;
			// BGM�ύX
			Sound* sound = GetGame()->GetSound();
			sound->StopMusic("event:/NormalBGM");
			sound->StartMusic("event:/GameBGM");
		}
		else if (
			((GetGame()->GetPhase() == Game::PHASE_FAIL_GAME) ||
			(GetGame()->GetPhase() == Game::PHASE_SUCCSESS_GAME) ) &&
			(keyState[SDL_SCANCODE_RETURN])) {
			// �Q�[���I��
			GetGame()->SetPhase(Game::PHASE_NORMAL); // Normal�Ɉړ�
			mTalkIdx = 0;
			GetGame()->GetPlayer()->SetState(Player::IDLE);
			// BGM�ύX
			sound->StopMusic("event:/GameBGM");
			sound->StartMusic("event:/NormalBGM");
			sound->StartMusic("event:/Select");
		}
		else if (
			(mTalkIdx != 2) &&
			(keyState[SDL_SCANCODE_RETURN])) {
			sound->StartMusic("event:/Select");
			mTalkIdx++;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
		}

	}

}

bool TalkText::GetIsFinishDraw()
{
	return mTalkTextComp->GetIsFinishDraw();
}

std::u16string TalkText::JsonToString(nl::json data)
{
	std::string str;
	data.get_to(str);
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	return convert.from_bytes(str);
}
