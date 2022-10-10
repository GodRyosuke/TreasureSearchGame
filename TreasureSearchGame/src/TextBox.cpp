#include "TextBox.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "SpriteComponent.hpp"

TextBox::TextBox(Game* game)
	:Actor(game)
	,mIsFrontCounter(false)
{
	mSprite = new SpriteComponent(this);
	mSprite->Load("./resources/TextBox.png", 0.7f);
	mSprite->SetIsDraw(false);	// デフォルトでは描画しない
}

void TextBox::UpdateActor(float deltatime)
{
	Player::State playerState = GetGame()->GetPlayer()->GetState();
	if (playerState == Player::TALK) {
		mSprite->SetIsDraw(true);
	}
	else {
		mSprite->SetIsDraw(false);
	}
}

void TextBox::ActorInput(const uint8_t* keyState)
{

}

