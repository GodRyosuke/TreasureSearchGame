#pragma once
#include "Actor.hpp"


class Player : public Actor
{
public:
	Player(class Game* game);

	void ActorInput(const uint8_t* keys) override;
	void UpdateActor(float deltaTime) override;

	void SetPlayerRot(float rotate);

private:
	class FollowCamera* mFollowCamera;
	class SkinMeshComponent* mSkinMeshComp;
	float mMoveSpeed;

	float mPlayerRot;
};
