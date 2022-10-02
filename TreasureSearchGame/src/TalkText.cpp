#include "TalkText.hpp"
#include "TalkTextComponent.hpp"
#include "Game.hpp"
#include <fstream>
#include <codecvt>


TalkText::TalkText(Game* game)
	:Actor(game)
	,mTalkIdx(0)
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
	};
}

void TalkText::UpdateActor(float deltatime)
{
	Player::State playerState = GetGame()->GetPlayer()->GetState();

	if ((playerState == Player::TALK) && (mPreviousPlayerState != Player::TALK)) {
		mTalkTextComp->SetText(JsonToString(mData["TalkCostomer"]["Welcome"]["Talk1"]));
		mTalkTextComp->InittextPos();
		mTalkTextComp->SetIsDraw(true);
		SetPosition(glm::vec3(0.0f, -150.0f, 0.f));
		mTalkTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
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
	// 描画が終わってからエンターキーが押下されたら
	if (mTalkTextComp->GetIsFinishDraw()) {
		if ((mTalkIdx == 2)&&(keyState[SDL_SCANCODE_2])) {
			// またきてね描画
			mTalkIdx = 3;
			mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
			mTalkTextComp->InittextPos();
		}
		else if ((mTalkIdx == 3) && (keyState[SDL_SCANCODE_RETURN])) {
			// またきてね描画終了
			// playerをTalkからIdleに移行
			GetGame()->GetPlayer()->SetState(Player::IDLE);
		}
	}
	if ((mTalkTextComp->GetIsFinishDraw() == true) && (keyState[SDL_SCANCODE_RETURN])) {
		mTalkIdx++;
		mTalkTextComp->SetText(JsonToString(mTalkData[mTalkIdx]));
		mTalkTextComp->InittextPos();
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
