#include "Actor/MazeBox.hpp"
#include "Texture.hpp"
#include "Component/MeshComponent.hpp"
#include "Game.hpp"
#include "Actor/Player.hpp"

MazeBox::MazeBox(Game* game)
	:Actor(game)
	,mGrowingSpeed(0.13f)
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
				mZ += mGrowingSpeed;
			}
		}
		else {
			if (0.f < mZ) {
				mZ -= mGrowingSpeed;
			}
		}

	}
	else {
		// �Q�[���I��������A�����オ���Ă���MazeBox��������
		if (0.f < mZ) {
			mZ -= mGrowingSpeed;
		}
	}

	// �����肷�����A�オ�肷����mZ���C��
	if (mZ < 0.f) {
		mZ = 0.f; 
	}
	else if (2.f < mZ) {
		mZ = 2.f;
	}
	SetPosition(glm::vec3(boxPos.x, boxPos.y, mZ));
}

