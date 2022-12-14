#pragma once

#include "glm.hpp"
#include "Component.hpp"

class FollowCamera : public Component
{
public:
	FollowCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	// 固定カメラ用
	void SetConstCamera(glm::vec3 pos, glm::vec3 target) { 
		mConstCameraPos = pos; mConstCameraTarget = target; mIsConstCamera = true; }

	void SetIsConstCamera(bool isConst) { mIsConstCamera = isConst; }
	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }
private:
	glm::vec3 ComputeCameraPos() const;
	void SetViewMatrix(glm::mat4 view, glm::vec3 cameraPos);

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

	bool mIsConstCamera;
	glm::vec3 mConstCameraPos;
	glm::vec3 mConstCameraTarget;
};
