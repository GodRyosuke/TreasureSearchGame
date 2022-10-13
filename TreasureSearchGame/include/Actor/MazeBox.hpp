#pragma once

#include "Actor.hpp"

class MazeBox : public Actor
{
public:
	MazeBox(class Game* game);
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
	void UpdateActor(float deltatime) override;
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
	
	class MeshComponent* mMeshComp;

	class Texture* mBlackTex;
	class Texture* mWhiteTex;

	float mZ;

	bool IsGrowing;
	bool IsDowning;
	float z;
	float mGrowingSpeed;
};