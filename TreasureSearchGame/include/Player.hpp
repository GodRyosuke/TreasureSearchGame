#pragma once
#include "Actor.hpp"


class Player : public Actor
{
public:
	Player(class Game* game);
	glm::vec3 GetForward();
	void Update(float deltatime);
	float GetPlayerRot() { return mPlayerRot; }
	void SetPlayerRot(float rotate);
	class Game* GetGame() { return mGame; }

private:
	class FollowCamera* mFollowCamera;
	class SkinMesh* mMesh;

	float mPlayerRot;
	class Game* mGame;
	glm::vec3 mPos;
	float mScale;
};
