#include "Actor/Clerk.hpp"
#include "Component/SkinMeshComponent.hpp"
#include "Game.hpp"
#include "Actor/Player.hpp"

Clerk::Clerk(Game* game)
	:Actor(game)
	,mState(IDLE)
{
	SetPosition(glm::vec3(1.f, 1.f, 0.f));
	SetScale(0.002f);

	mSkinMeshComp = new SkinMeshComponent(this);
	mSkinMeshComp->SetSkinMesh(game->GetSkinMesh("SimpleMan", "fbx"));
	mSkinMeshComp->SetAnimIdx(6);
}

void Clerk::UpdateActor(float deltatime)
{
	Player::State playerState = GetGame()->GetPlayer()->GetState();



	if (mSkinMeshComp->GetIsOneAnim()) {
		if (mSkinMeshComp->GetIsFinishOneAnim()) {
			// IDLEのアニメーションにする
			mState = BOW;
			mSkinMeshComp->SetAnimIdx(7);
			mSkinMeshComp->SetIsOneAnim(false); // ループにする
		}
	} else 	if (playerState == Player::TALK) {
		mState = TALK;
		mSkinMeshComp->SetAnimIdx(7);
	}
	else {
		mState = IDLE;
		mSkinMeshComp->SetAnimIdx(6);
	}


	switch (mState)
	{
	case Clerk::IDLE:
	{
		mSkinMeshComp->SetAnimIdx(6);
		break;
	}
	case Clerk::TALK:
	{
		mSkinMeshComp->SetAnimIdx(7);
	}
	break;
	case Clerk::BOW:
	{
		mSkinMeshComp->SetAnimIdx(9);
		break;
	}
	}
}

void Clerk::BowToPlayer()
{
	mSkinMeshComp->SetIsOneAnim(true);
	mSkinMeshComp->SetAnimIdx(9);
	mState = BOW;
}
