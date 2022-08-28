#include "Plane.hpp"
#include <iostream>
#include "Texture.hpp"

Plane::Plane()
	:Mesh()
{

}

void Plane::BindTexture(int materialIndex)
{
	switch (mType)
	{
	case CONCRETE:
		mConcreteTex->BindTexture();
		break;
	case BRICK:
		mBrickTex->BindTexture();
		break;
	}
}

bool Plane::LoadBrickTex(std::string filePath)
{
	Texture* tex = new Texture(filePath);
	if (!tex) {
		std::cout << "error: failed to load texture" << filePath << std::endl;
		return false;
	}
	mBrickTex = tex;
	return true;
}

bool Plane::LoadConcreteTex(std::string filePath)
{
	Texture* tex = new Texture(filePath);
	if (!tex) {
		std::cout << "error: failed to load texture" << filePath << std::endl;
		return false;
	}
	mConcreteTex = tex;
	return true;
}

