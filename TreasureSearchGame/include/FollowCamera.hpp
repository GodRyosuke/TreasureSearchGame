#pragma once

#include "glm.hpp"
#include "Player.hpp"

class FollowCamera
{
public:
	FollowCamera(Player* owner);

	void Update(float deltaTime);

	void SnapToIdeal();

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }
private:
	glm::vec3 ComputeCameraPos() const;
	void SetViewMatrix(glm::mat4 view, glm::vec3 cameraPos);

	Player* mOwner;

	// Actual position of camera
	glm::vec3 mActualPos;
	// Velocity of actual camera
	glm::vec3 mVelocity;
	// Horizontal follow distance
	float mHorzDist;
	// Vertical follow distance
	float mVertDist;
	// Target distance
	float mTargetDist;
	// Spring constant (higher is more stiff)
	float mSpringConstant;
};
