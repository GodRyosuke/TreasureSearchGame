#include "FollowCamera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Game.hpp"
#include "Actor.hpp"

FollowCamera::FollowCamera(Actor* owner)
	:Component(owner)
	,mHorzDist(3.0f)
	,mVertDist(2.0f)
	,mTargetDist(5.0f)
	,mSpringConstant(8.0f)
{


}


void FollowCamera::Update(float deltaTime)
{
	// Compute dampening from spring constant
	float dampening = 2.0f * sqrt(mSpringConstant);
	// Compute ideal position
	glm::vec3 idealPos = ComputeCameraPos();
	// Compute difference between actual and ideal
	glm::vec3 diff = mActualPos - idealPos;
	// Compute acceleration of spring
	glm::vec3 acel = -mSpringConstant * diff -
		dampening * mVelocity;
	// Update velocity
	mVelocity += acel * deltaTime;
	// Update actual camera position
	mActualPos += mVelocity * deltaTime;
	// Target is target dist in front of owning actor
	glm::vec3 target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	// Use actual position here, not ideal
	glm::mat4 view = glm::lookAt(mActualPos, target,
		glm::vec3(0.0f, 0.0f, 1.0f));
	SetViewMatrix(view, mActualPos);
}

void FollowCamera::SnapToIdeal()
{
	// Set actual position to ideal
	mActualPos = ComputeCameraPos();
	// Zero velocity
	mVelocity = glm::vec3(0.f);
	// Compute target and view
	glm::vec3 target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	// Use actual position here, not ideal
	glm::mat4 view = glm::lookAt(mActualPos, target,
		glm::vec3(0.f, 0.f, 1.0f));
	SetViewMatrix(view, mActualPos);
}

glm::vec3 FollowCamera::ComputeCameraPos() const
{
	// Set camera position behind and above owner
	glm::vec3 cameraPos = mOwner->GetPosition();
	cameraPos -= mOwner->GetForward() * mHorzDist;
	cameraPos += glm::vec3(0.0f, 0.0f, 1.0f) * mVertDist;
	return cameraPos;
}

void FollowCamera::SetViewMatrix(glm::mat4 view, glm::vec3 cameraPos)
{
	Game* game = mOwner->GetGame();
	game->SetViewMatrix(view, cameraPos);
}