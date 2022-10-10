#include "SkinMeshComponent.hpp"
#include "Actor.hpp"
#include "Skinning.hpp"
#include "Game.hpp"
#include "Shader.hpp"

SkinMeshComponent::SkinMeshComponent(Actor* owner)
	:MeshComponent(owner, true)
	,mStopAnimation(false)
	,mZeroTransFrag(false)
	,mAnimtime(-1.3)
{
}

void SkinMeshComponent::Update(float deltatime)
{
	mSkinMesh->SetAnimIdx(mAnimIdx);

	float timeInTicks = 0;
	if (mAnimtime >= 0) {
		timeInTicks = mAnimtime;
	}
	else {
		timeInTicks = mOwner->GetGame()->GetTicksCount();
	}

	//if (!mZeroTransFrag) {
	//	timeInTicks = mOwner->GetGame()->GetTicksCount();
	//}

	mSkinMesh->GetBoneTransform(timeInTicks / 1000.f, mBoneMatrixPallete);
	if (!mStopAnimation) {
	}
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
	shader->SetFloatUniform("gAlpha", mAlpha);

	mSkinMesh->BindVertexArray();
	mSkinMesh->SetMaterialUniform(shader);  // Material, Texture‚È‚Ç‚ðÝ’è
	mSkinMesh->UnBindVertexArray();
}

