#pragma once

#include "MeshComponent.hpp"
#include "glm.hpp"
#include <vector>

class SkinMeshComponent : public MeshComponent
{
public:
	SkinMeshComponent(class Actor* owner);
	void Update(float deltatime) override;
	void SetSkinMesh(class SkinMesh* mesh) { mSkinMesh = mesh; }

private:
	void SetMatrixUniform(class Shader* shader) override;

	class SkinMesh* mSkinMesh;
	std::vector<glm::mat4> mBoneMatrixPallete;
};