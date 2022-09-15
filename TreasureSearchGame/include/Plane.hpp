#pragma once

#include "Actor.hpp"
#include <string>

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
	bool LoadBrickTex(std::string filePath);
	bool LoadConcreteTex(std::string filePath);
	void SetPlaneType(PlaneType type) { mType = type; }

private:
	class MeshComponent* mMeshComp;
	PlaneType mType;
};