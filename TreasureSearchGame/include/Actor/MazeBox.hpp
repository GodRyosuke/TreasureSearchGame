#pragma once

#include "Actor.hpp"

// –À˜H‚Ì°‚ğŒ`¬‚·‚éˆê‚Â‚Ì” 
class MazeBox : public Actor
{
public:
	MazeBox(class Game* game);
	enum BoxType {
		WHITE,
		BLACK,
		NUM_BOXTYPE
	};

	void UpdateActor(float deltatime) override;
	void SetBoxType(BoxType type) { mType = type; }
	

private:
	BoxType mType;
	class MeshComponent* mMeshComp;
	float mZ;
	float mGrowingSpeed;
};