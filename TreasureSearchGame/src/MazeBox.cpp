#include "MazeBox.hpp"

MazeBox::MazeBox(std::string fileRoot, std::string meshFileName,
	std::string blackFileName, std::string whiteFileName)
	: Mesh()
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

