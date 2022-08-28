#pragma once

#include "Skinning.hpp"

class Player : public SkinMesh
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

	float mPlayerRot;
	class Game* mGame;
};
