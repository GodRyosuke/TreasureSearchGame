#include "Clerk.hpp"
#include "SkinMeshComponent.hpp"
#include "Game.hpp"
#include "Player.hpp"

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
	if (playerState == Player::TALK) {
		mState = TALK;
		mSkinMeshComp->SetAnimIdx(7);
	}

}
