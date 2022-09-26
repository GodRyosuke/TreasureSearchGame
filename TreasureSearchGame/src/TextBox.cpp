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
	mSprite->SetIsDraw(true);	// �f�t�H���g�ł͕`�悵�Ȃ�
}

void TextBox::UpdateActor(float deltatime)
{
	glm::vec3 playerPos = GetGame()->GetPlayer()->GetPosition();
	// player���J�E���^�[�̑O�ɗ�����
	if (
		(2.0f < playerPos.x) && (playerPos.x < 3.0f) &&
		(1.0f < playerPos.y) && (playerPos.y < 2.0f)
		) {
		mIsFrontCounter = true;
	}
	else {
		mIsFrontCounter = false;
	}
}

void TextBox::ActorInput(const uint8_t* keyState)
{
	if ((mIsFrontCounter == true) && (GetGame()->GetPhase() != Game::PHASE_TALK)) {
		if (keyState[SDL_SCANCODE_RETURN]) {
			// �J�E���^�[�̑O�ŃG���^�[�L�[�������ꂽ��ATalkPhase�ɕς��
			GetGame()->SetPhase(Game::PHASE_TALK);

		}
	}
}

