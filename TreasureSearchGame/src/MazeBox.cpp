#include "MazeBox.hpp"
#include "Texture.hpp"
#include "MeshComponent.hpp"
#include "Game.hpp"
#include "Player.hpp"

MazeBox::MazeBox(Game* game)
	:Actor(game)
	,mGrowingSpeed(0.007f)
	,mZ(0.f)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetMesh("MazeBox", "fbx"));
}

void MazeBox::UpdateActor(float deltaTime)
{
	if (mType == WHITE) {
		mMeshComp->SetTexture("./resources/MazeBox/WhiteUV.png");
	}
	else if (mType == BLACK) {
		mMeshComp->SetTexture("./resources/MazeBox/BlackUV.png");
	}
	else {
		mMeshComp->SetTexture("");
	}

	glm::vec3 boxPos = GetPosition();
	if (GetGame()->GetPhase() == Game::PHASE_GAME) {
		glm::vec3 playerPos = GetGame()->GetPlayer()->GetPosition();
		if (
			(boxPos.x - 1.5 <= playerPos.x) && (playerPos.x <= boxPos.x + 1.5) &&
			(boxPos.y - 1.5 <= playerPos.y) && (playerPos.y <= boxPos.y + 1.5)
			) {
			if ((GetGame()->IsWall(boxPos)) && (mZ < 2.0f)) {
				mZ += 0.1f;
			}
		}
		else {
			if (0.f < mZ) {
				mZ -= 0.1f;
			}
		}

	}
	else {
		// ゲーム終了したら、立ち上がっているMazeBoxを下げる
		if (0.f < mZ) {
			mZ -= 0.1f;
		}
	}

	// 下がりすぎた、上がりすぎたmZを修正
	if (mZ < 0.f) {
		mZ = 0.f; 
	}
	else if (2.f < mZ) {
		mZ = 2.f;
	}
	SetPosition(glm::vec3(boxPos.x, boxPos.y, mZ));
}

void MazeBox::GrowUp()
{
	if (z >= 2.0f) {
		mState = UP_STATE;
		IsGrowing = false;
		return;
	}
	mState = GROW_UP_STATE;
	IsGrowing = true;
	z += mGrowingSpeed;
}

void MazeBox::GrowDown()
{
	if (z <= 0.f) {
		mState = DOWN_STATE;
		IsDowning = false;
		return;
	}
	mState = GROW_DOWN_STATE;
	IsDowning = true;
	z -= mGrowingSpeed;
}
