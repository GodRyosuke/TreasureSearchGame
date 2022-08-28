#include "Player.hpp"
#include "Game.hpp"
#include "FollowCamera.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"

Player::Player(Game* game)
	:SkinMesh(),
	mGame(game)
{
	SetPos(glm::vec3(29.0f, 1.0f, 0.0f));
	mFollowCamera = new FollowCamera(this);
	mFollowCamera->SnapToIdeal();
}

glm::vec3 Player::GetForward()
{
	return glm::rotate(glm::vec3(-1.0f, 0.f, 0.f), (float)M_PI * mPlayerRot / 180, glm::vec3(0.0f, 0.f, 1.0f));
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
	mMeshRotate = glm::rotate(glm::mat4(1.0f), (float)M_PI * rotate / 180, glm::vec3(0.f, 0.f, 1.f));
}

void Player::Update(float deltatime)
{
	mFollowCamera->Update(deltatime);
}