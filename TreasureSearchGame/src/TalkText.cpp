#include "TalkText.hpp"
#include "TalkTextComponent.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include <fstream>
#include <codecvt>
#include <random>


TalkText::TalkText(Game* game)
	:Actor(game)
	,mTalkIdx(0)
	,mIsSelectText(false)
{
	mTalkTextComp = new TalkTextComponent(this);
	mTalkTextComp->SetIsDraw(false);

	// 会話に使う文字列読み込み
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

	// プレイヤーが話しかけたら
	if ((playerState == Player::TALK) && (mPreviousPlayerState != Player::TALK)) {
		if (GetGame()->GetPhase() == Game::PHASE_FAIL_GAME) {
			// ゲーム失敗
			mTalkIdx = 7;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
		}
		else if (GetGame()->GetPhase() == Game::PHASE_SUCCSESS_GAME) {
			// ゲームクリア
			mTalkIdx = 8 + mResultIdx;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
		}
		else {
			mTalkIdx = 0;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
			mTalkTextComp->SetIsDraw(true);
			SetPosition(glm::vec3(0.0f, -150.0f, 0.f));
			mTalkTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
		}
	}

	// 選択画面なら
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
	// 描画が終わってからエンターキーが押下されたら
	if ((playerState == Player::TALK) && (mTalkTextComp->GetIsFinishDraw())) {
		if ((mTalkIdx == 2)&&(keyState[SDL_SCANCODE_2])) {
			// またきてね描画
			mTalkIdx = 3;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
		}
		else if ((mTalkIdx == 2) && (keyState[SDL_SCANCODE_1])) {
			// ゲームをすると選択
			mTalkIdx = 4;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
		}
		else if ((mTalkIdx == 3) && (keyState[SDL_SCANCODE_RETURN])) {
			// またきてね描画終了
			// playerをTalkからIdleに移行
			mTalkIdx = 0;
			GetGame()->GetPlayer()->SetState(Player::IDLE);
			GetGame()->GetPlayer()->WaitSeconds(1000);
		}
		else if ((mTalkIdx == 6) && (keyState[SDL_SCANCODE_RETURN])) {
			// ゲーム開始のため、TALK終わり
			mTalkIdx = 0;
			GetGame()->SetPhase(Game::PHASE_GAME);	// ゲームスタート
			GetGame()->GetPlayer()->SetState(Player::IDLE);
			// 宝箱決定
			std::mt19937 mt(static_cast<int>(time(0)));
			mResultIdx = mt() % 5;
			// BGM変更
			Sound* sound = GetGame()->GetSound();
			sound->SetType(Sound::NORMAL);
			sound->StopMusic();
			sound->SetType(Sound::GAME);
			sound->StartMusic();
		}
		else if (
			((GetGame()->GetPhase() == Game::PHASE_FAIL_GAME) ||
			(GetGame()->GetPhase() == Game::PHASE_SUCCSESS_GAME) ) &&
			(keyState[SDL_SCANCODE_RETURN])) {
			// ゲーム終了
			GetGame()->SetPhase(Game::PHASE_NORMAL); // Normalに移動
			mTalkIdx = 0;
			GetGame()->GetPlayer()->SetState(Player::IDLE);
			// BGM変更
			Sound* sound = GetGame()->GetSound();
			sound->SetType(Sound::GAME);
			sound->StopMusic();
			sound->SetType(Sound::NORMAL);
			sound->StartMusic();
		}
		else if ((mTalkIdx != 2) &&(keyState[SDL_SCANCODE_RETURN])) {
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
