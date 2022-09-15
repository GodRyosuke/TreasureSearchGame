#include "MazeBox.hpp"
#include "Texture.hpp"
#include "MeshComponent.hpp"
#include "Game.hpp"

MazeBox::MazeBox(Game* game)
	:Actor(game)
	,mGrowingSpeed(0.007f)
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
