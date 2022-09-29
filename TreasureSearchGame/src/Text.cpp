#include "Text.hpp"
#include "TextComponent.hpp"
#include "Game.hpp"
#include "TalkText.hpp"
#include "Player.hpp"

UserAssistText::UserAssistText(Game* game)
	:Actor(game)
{
	mTextComp = new TextComponent(this);
}

void UserAssistText::UpdateActor(float deltatime)
{
	glm::vec3 playerPos = GetGame()->GetPlayer()->GetPosition();
	Player::State playerState = GetGame()->GetPlayer()->GetState();
	// playerがカウンターの前に来たら
	if (
		(2.0f < playerPos.x) && (playerPos.x < 3.0f) &&
		(1.0f < playerPos.y) && (playerPos.y < 2.0f) &&
		(playerState != Player::TALK)
		) {
		SetScale(1.f);
		SetPosition(glm::vec3(0.0f, 300.f, 0.f));
		mTextComp->SetText(u"エンターキーで話す");
		mTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
	}
	else {
		mTextComp->SetText(u"");
	}

	if ((playerState == Player::TALK) && (GetGame()->GetTalkText()->GetIsFinishDraw() == true)) {
		SetPosition(glm::vec3(300.f, -300.f, 0.f));
		SetScale(0.5f);
		mTextComp->SetText(u"エンターキーで進む");
		mTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
	}

}



