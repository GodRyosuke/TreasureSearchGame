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
	mSkinMesh->GetBoneTransform(mOwner->GetGame()->GetTicksCount() / 1000.f, mBoneMatrixPallete);
}

void SkinMeshComponent::Draw(Shader* shader)
{
	mSkinMesh->SetAnimIdx(mAnimIdx);
	shader->UseProgram();
	shader->SetMatrixUniform("ModelTransform", mOwner->GetWorldTransform());
	for (int i = 0; i < mBoneMatrixPallete.size(); i++) {
		std::string uniformName = "uMatrixPalette[" + std::to_string(i) + ']';
		shader->SetMatrixUniform(uniformName, mBoneMatrixPallete[i]);
	}
	mSkinMesh->BindVertexArray();
	mSkinMesh->SetMaterialUniform(shader);  // Material, Texture‚È‚Ç‚ðÝ’è
	mSkinMesh->UnBindVertexArray();
}

