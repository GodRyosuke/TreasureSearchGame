#include "Player.hpp"
#include "Game.hpp"
#include "FollowCamera.hpp"
#include "SkinMeshComponent.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"

#include "TextComponent.hpp"

Player::Player(Game* game)
	:Actor(game)
	,mMoveSpeed(0.1)
{
	SetPosition(glm::vec3(10.0f, 3.0f, 0.0f));
	mPlayerRot = 180;
	SetScale(0.002f);
	mSkinMeshComp = new SkinMeshComponent(this);
	mSkinMeshComp->SetSkinMesh(game->GetSkinMesh("SimpleMan", "fbx"));
	mSkinMeshComp->SetAnimIdx(4);

	Actor* a = new Actor(game);
	mDebugText = new TextComponent(a);
	
	mFollowCamera = new FollowCamera(this);
	mFollowCamera->SnapToIdeal();
}

void Player::ActorInput(const uint8_t* keys)
{
	glm::vec3 playerNewPos = GetPosition();
	if (keys[SDL_SCANCODE_W]) {
		playerNewPos = GetPosition() + GetForward() * mMoveSpeed;
	}
	if (keys[SDL_SCANCODE_S]) {
		playerNewPos = GetPosition() - GetForward() * mMoveSpeed;
	}
	if (keys[SDL_SCANCODE_A]) {
		SetPlayerRot(mPlayerRot + 1.f);
	}
	if (keys[SDL_SCANCODE_D]) {
		SetPlayerRot(mPlayerRot - 1.f);
	}

	// 受付と話す
	if ((mState != TALK) && (mPreviousState != TALK)) {
		if (
			(2.0f < GetPosition().x) && (GetPosition().x < 3.0f) &&
			(1.0f < GetPosition().y) && (GetPosition().y < 2.0f)
			) {
			if (keys[SDL_SCANCODE_RETURN]) {
				GetGame()->SetPhase(Game::PHASE_TALK);
				mState = TALK;
			}
		}
	}


	// Playerの行動範囲制限
	bool IsUpdatePlayerPos = false;
	// 壁の外には行けない
	if ((playerNewPos.x < 0.25f) || (playerNewPos.x > 29.75f) ||
		(playerNewPos.y < 0.25f) || (playerNewPos.y > 29.75f)
		) {
		IsUpdatePlayerPos = false;
	}
	else if (
		// カウンターの中には入れない
		(0.f < playerNewPos.x) && (playerNewPos.x < 2.f) &&
		(0.f < playerNewPos.y) && (playerNewPos.y < 3.5f)
		) {
		IsUpdatePlayerPos = false;
	}
	else if (mState == TALK) {
		// 話し中なら動かない
		IsUpdatePlayerPos = false;
	}
	else {
		IsUpdatePlayerPos = true;
	}
	// PlayerがMazeの領域にあるか？
	if (
		(0.f < playerNewPos.x) && (playerNewPos.x < 30.f) &&
		(10.f < playerNewPos.y) && (playerNewPos.y < 30.f)
		) {
		if (GetGame()->IsWall(playerNewPos)) {
			// Playerのいる場所が壁なら更新しない
			IsUpdatePlayerPos = false;
		}
		else {
			IsUpdatePlayerPos = true;
		}
	}


	if (IsUpdatePlayerPos && (playerNewPos != GetPosition())) {
		mState = WALK;
		SetPosition(playerNewPos);
	}
	else if (mState == TALK) {

	}
	else {
		mState = IDLE;
	}
}

void Player::SetPlayerRot(float rotate)
{
	if (rotate > 360) {
		rotate -= 360;
	}
	if (rotate < 0) {
		rotate += 360;
	}
	mPlayerRot = rotate;
}

void Player::UpdateActor(float deltatime)
{
	SetRotation(glm::rotate(glm::mat4(1.0f), (float)M_PI * mPlayerRot / 180, glm::vec3(0.f, 0.f, 1.f)));

	switch (mState)
	{
	case Player::WALK:
	{
		mDebugText->SetText(u"walk");
		mSkinMeshComp->SetAnimIdx(8);
		break;
	}
	case Player::IDLE:
	{
		mDebugText->SetText(u"idle");
		mSkinMeshComp->SetAnimIdx(4);
		break;
	}
	case Player::TALK:
	{
		mDebugText->SetText(u"Talk");
	}
		break;
	case Player::OPEN_CHEST:
		mDebugText->SetText(u"Open ");
		break;
	}

	mPreviousState = mState;
}