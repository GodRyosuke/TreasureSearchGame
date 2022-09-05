#pragma once
#include "Actor.hpp"


class Player : public Actor
{
public:
	Player(class Game* game);

	void ActorInput(const uint8_t* keys) override;
	void UpdateActor(float deltaTime) override;


	//glm::vec3 GetForward();
	//void Update(float deltatime);
	//float GetPlayerRot() { return mPlayerRot; }
	//class Game* GetGame() { return mGame; }
	void SetPlayerRot(float rotate);

private:
	class FollowCamera* mFollowCamera;
	class SkinMesh* mMesh;

	float mPlayerRot;
};
