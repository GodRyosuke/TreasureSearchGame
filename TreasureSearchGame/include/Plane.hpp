#pragma once

#include "Mesh.hpp"

class Plane : public Mesh
{
public:
	Plane();
	enum PlaneType {
		CONCRETE,
		BRICK,
		NUM_PLANETYPE
	};
	bool LoadBrickTex(std::string filePath);
	bool LoadConcreteTex(std::string filePath);
	void BindTexture(int materialIndex) override;
	void SetPlaneType(PlaneType type) { mType = type; }

private:
	class Texture* mConcreteTex;
	class Texture* mBrickTex;
	PlaneType mType;
};