#include "SkinMeshComponent.hpp"
#include "Actor.hpp"
#include "Skinning.hpp"
#include "Game.hpp"
#include "Shader.hpp"

SkinMeshComponent::SkinMeshComponent(Actor* owner)
	:MeshComponent(owner, true)
{
}

void SkinMeshComponent::Update(float deltatime)
{
	mSkinMesh->GetBoneTransform(mOwner->GetGame()->GetTicksCount(), mBoneMatrixPallete);
}

void SkinMeshComponent::SetMatrixUniform(Shader* shader)
{
	MeshComponent::SetMatrixUniform(shader); // world trans‚Ìİ’è
	for (int i = 0; i < mBoneMatrixPallete.size(); i++) {
		std::string uniformName = "uMatrixPalette[" + std::to_string(i) + ']';
		shader->SetMatrixUniform(uniformName, mBoneMatrixPallete[i]);
	}
}
