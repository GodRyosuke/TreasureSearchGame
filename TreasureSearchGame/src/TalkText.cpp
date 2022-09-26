#include "TalkText.hpp"
#include "TalkTextComponent.hpp"
#include "Game.hpp"
#include <fstream>
#include <codecvt>


TalkText::TalkText(Game* game)
	:Actor(game)
{
	mTalkTextComp = new TalkTextComponent(this);
	mTalkTextComp->SetIsDraw(false);

	// ‰ï˜b‚ÉŽg‚¤•¶Žš—ñ“Ç‚Ýž‚Ý
	{
		std::string filePath = "./resources/TextData.json";
		std::ifstream ifs(filePath.c_str());
		if (ifs.good())
		{
			ifs >> mData;
		}
		ifs.close();
	}
}

void TalkText::UpdateActor(float deltatime)
{
	if (GetGame()->GetPhase() == Game::PHASE_TALK) {
		mTalkTextComp->SetIsDraw(true);
		SetPosition(glm::vec3(0.0f, -150.0f, 0.f));
		mTalkTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
		mTalkTextComp->SetText(JsonToString(mData["TalkCostomer"]["Welcome"]["Talk1"]));
	}
	else {
		mTalkTextComp->SetIsDraw(false);
	}
}

std::u16string TalkText::JsonToString(nl::json data)
{
	std::string str;
	data.get_to(str);
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	return convert.from_bytes(str);
}
