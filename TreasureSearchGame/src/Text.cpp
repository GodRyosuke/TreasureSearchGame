#include <codecvt>
#include "Text.hpp"
#include "TextComponent.hpp"
#include "Game.hpp"
#include "TalkText.hpp"
#include "Player.hpp"

static Game::PHASE preGamePhase;

UserAssistText::UserAssistText(Game* game)
	:Actor(game)
{
	mTextComp = new TextComponent(this);
}

void UserAssistText::UpdateActor(float deltatime)
{
	glm::vec3 playerPos = GetGame()->GetPlayer()->GetPosition();
	Player::State playerState = GetGame()->GetPlayer()->GetState();
	// player���J�E���^�[�̑O�ɗ�����
	if (
		(2.0f < playerPos.x) && (playerPos.x < 3.0f) &&
		(1.0f < playerPos.y) && (playerPos.y < 2.0f) &&
		(playerState != Player::TALK)
		) {
		SetScale(1.f);
		SetPosition(glm::vec3(0.0f, 300.f, 0.f));
		mTextComp->SetText(u"�G���^�[�L�[�Řb��");
		mTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
	}
	else {
		mTextComp->SetText(u"");
	}

	if ((playerState == Player::TALK) && (GetGame()->GetTalkText()->GetIsFinishDraw() == true)) {
		SetPosition(glm::vec3(300.f, -300.f, 0.f));
		SetScale(0.5f);
		if (GetGame()->GetTalkText()->GetIsSelectText()) {
			mTextComp->SetText(u"�D���Ȃق���I��");
		}
		else {
			mTextComp->SetText(u"�G���^�[�L�[�Ői��");
		}
		mTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
	}

	// �󔠂̑O�ɗ�����\��
	glm::vec3 boxPos = GetGame()->GetTreasurePos();
	if (GetGame()->GetPhase() == Game::PHASE_GAME) {
		if (
			((boxPos.x - 1.f) < playerPos.x) && (playerPos.x < (boxPos.x + 1.f)) &&
			((boxPos.y - 1.f) < playerPos.y) && (playerPos.y < (boxPos.y + 1.f))
			) {
			mTextComp->SetText(u"�G���^�[�L�[�ŊJ����");
			mTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
		}
	}

}


TimerText::TimerText(Game* game)
	:Actor(game)
{
	mTextComp = new TextComponent(this);
	SetPosition(glm::vec3(500.f, 300.f, 0.f));
	mTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
	mTime = mMaxTime = 60;
}

void TimerText::UpdateActor(float deltatime)
{
	Game::PHASE currGamePhase = GetGame()->GetPhase();
	std::string timeStr = std::to_string(mTime);
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;

	if ((preGamePhase != Game::PHASE_GAME) && (currGamePhase == Game::PHASE_GAME)) {
		// ���������Q�[�����J�n������
		mTextComp->SetText(convert.from_bytes(timeStr));
		mStartTime = GetGame()->GetTicksCount();
	}
	else if (currGamePhase == Game::PHASE_GAME) {
		mTextComp->SetText(convert.from_bytes(timeStr));
		if (mTime <= 10) {
			// �c��10�b�Ȃ�����ԐF
			mTextComp->SetTextColor(glm::vec3(1.f, 0.f, 0.1f));
		}
		else {
			mTextComp->SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
		}
		if (mTime < 0) {
			// ���Ԑ؂�
			mTextComp->SetText(u"");
			GetGame()->GetPlayer()->SetState(Player::TALK);
			GetGame()->SetPhase(Game::PHASE_FAIL_GAME);
		}
	}
	else {
		// �Q�[���N���A�A���̂ق�
		mTextComp->SetText(u"");
	}

	uint32_t currTime = GetGame()->GetTicksCount();
	mTime = mMaxTime - ((currTime - mStartTime) / 1000);	// �c�莞�ԍX�V



	preGamePhase = currGamePhase;
}