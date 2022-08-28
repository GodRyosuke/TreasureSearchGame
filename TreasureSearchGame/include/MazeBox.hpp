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

	enum MazeBoxState {
		UP_STATE,
		DOWN_STATE,
		GROW_UP_STATE,
		GROW_DOWN_STATE,
		NUM_STATE
	};
	void BindTexture(int materialIndex) override;
	void SetBoxType(BoxType type) { mType = type; }
	void GrowUp();	// óßÇøè„Ç™ÇÈ
	void GrowDown(); // â∫Ç™ÇÈ
	MazeBoxState GetMazeBoxState() { return mState; }
	
	bool GetIsGrowing() { return IsGrowing; }
	bool GetIsDowning() { return IsDowning; }
	float GetZ() { return z; }

private:
	BoxType mType;
	MazeBoxState mState;

	class Texture* mBlackTex;
	class Texture* mWhiteTex;

	bool IsGrowing;
	bool IsDowning;
	float z;
	float mGrowingSpeed;
};