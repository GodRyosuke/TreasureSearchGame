#pragma once

#include "Mesh.hpp"

class MazeBox : public Mesh
{
public:
	MazeBox(std::string fileRoot, std::string meshFileName, 
		std::string blackFileName, std::string whiteFileName);
	enum BoxType {
		WHITE,
		BLACK,
		NUM_BOXTYPE
	};
	void BindTexture(int materialIndex) override;
	void SetBoxType(BoxType type) { mType = type; }

private:
	BoxType mType;

	Texture* mBlackTex;
	Texture* mWhiteTex;
};