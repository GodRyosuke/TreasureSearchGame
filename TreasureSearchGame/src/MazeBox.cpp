#include "MazeBox.hpp"
#include "Texture.hpp"

MazeBox::MazeBox(std::string fileRoot, std::string meshFileName,
	std::string blackFileName, std::string whiteFileName)
	: Mesh(),
	mGrowingSpeed(0.007f)
{
	mBlackTex = new Texture(fileRoot + blackFileName);
	mWhiteTex = new Texture(fileRoot + whiteFileName);
	Load(fileRoot, meshFileName);
}

void MazeBox::BindTexture(int materialIndex)
{
	switch (mType)
	{
	case WHITE:
		mWhiteTex->BindTexture();
		break;
	case BLACK:
		mBlackTex->BindTexture();
		break;
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
