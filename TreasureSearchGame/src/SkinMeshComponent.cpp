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
	,mIsOneAnim(false)
	,mIsFinishOneAnim(false)
{
}

void SkinMeshComponent::SetIsOneAnim(bool isOne)
{
	mIsOneAnim = isOne;
	if (isOne) {
		mAnimStartClock = mOwner->GetGame()->GetTicksCount();
		mIsFinishOneAnim = false;
	}
}

bool SkinMeshComponent::GetIsFinishOneAnim()
{
	if (!mIsOneAnim) {
		return false;
	}
	return mIsFinishOneAnim;
}

void SkinMeshComponent::Update(float deltatime)
{
	mSkinMesh->SetAnimIdx(mAnimIdx);

	// �Œ�Ȃ�
	if (mStopAnimation) {
		mSkinMesh->GetBoneTransform(mAnimtime / 1000.f, mBoneMatrixPallete);
		return;
	}

	float timeInTicks = 0;
	//if (mAnimtime >= 0) {
	//	timeInTicks = mAnimtime;
	//}
	//else {
	//	timeInTicks = mOwner->GetGame()->GetTicksCount();
	//}

	if (mIsOneAnim) {
		// ��x����̃A�j���[�V�����Ȃ�
		timeInTicks = mOwner->GetGame()->GetTicksCount() - mAnimStartClock;
	}
	else {
		timeInTicks = mOwner->GetGame()->GetTicksCount();
	}

	std::vector<glm::mat4> matrixPallete;
	int traseTime = mSkinMesh->GetBoneTransform(timeInTicks / 1000.f, matrixPallete);

	if (mIsOneAnim) {
		if (traseTime > 0) {
			// ��x����̃A�j���[�V�����Ȃ�A���[�v�ŕ`�悵�Ȃ�
			mIsFinishOneAnim = true;
			return;
		}
	}
	mBoneMatrixPallete = matrixPallete;
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
	mSkinMesh->SetMaterialUniform(shader);  // Material, Texture�Ȃǂ�ݒ�
	mSkinMesh->UnBindVertexArray();
}

