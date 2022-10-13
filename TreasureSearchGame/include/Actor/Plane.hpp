#pragma once

#include "Actor.hpp"
#include <string>

// è∞Ç‚ï«Ç…égópÇ∑ÇÈïΩñ 
class Plane : public Actor
{
public:
	Plane(class Game* game);
	enum PlaneType {
		CONCRETE,
		BRICK,
		NUM_PLANETYPE
	};

	void UpdateActor(float deltatime) override;
	void SetPlaneType(PlaneType type) { mType = type; }

private:
	class MeshComponent* mMeshComp;
	PlaneType mType;
};