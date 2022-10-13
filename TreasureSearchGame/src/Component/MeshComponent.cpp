#include "Component/MeshComponent.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Actor/Actor.hpp"
#include "Game.hpp"

MeshComponent::MeshComponent(Actor* owner, bool isSkeletal)
	:Component(owner)
    ,mIsSkeletal(isSkeletal)
    ,mAlpha(1.0f)
{
    mOwner->GetGame()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
    mOwner->GetGame()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
    shader->UseProgram();
    mMesh->SetTexture(mTexturePath);
    shader->SetMatrixUniform("ModelTransform", mOwner->GetWorldTransform());
    shader->SetFloatUniform("gAlpha", mAlpha);
    mMesh->BindVertexArray();
    mMesh->SetMaterialUniform(shader);  // Material, Texture‚È‚Ç‚ðÝ’è
    mMesh->UnBindVertexArray();
}