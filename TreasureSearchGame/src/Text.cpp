#include "Text.hpp"
#include "TextComponent.hpp"
#include "Game.hpp"
#include "Player.hpp"

UserAssistText::UserAssistText(Game* game)
	:Actor(game)
{
	mTextComp = new TextComponent(this);
}

void UserAssistText::UpdateActor(float deltatime)
{
	glm::vec3 playerPos = GetGame()->GetPlayer()->GetPosition();
	// playerがカウンターの前に来たら
	if (
		(2.0f < playerPos.x) && (playerPos.x < 3.0f) &&
		(1.0f < playerPos.y) && (playerPos.y < 2.0f)
		) {
		SetPosition(glm::vec3(0.0f, 300.f, 0.f));
		mTextComp->SetText(u"エンターキーで話す");
		mTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
	}
	else {
		mTextComp->SetText(u"");
	}

}



