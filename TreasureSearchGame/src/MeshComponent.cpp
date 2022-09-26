#include "MeshComponent.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Actor.hpp"
#include "Game.hpp"

MeshComponent::MeshComponent(Actor* owner, bool isSkeletal)
	:Component(owner)
    ,mIsSkeletal(isSkeletal)
{
    mOwner->GetGame()->AddMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
    shader->UseProgram();
    mMesh->SetTexture(mTexturePath);
    shader->SetMatrixUniform("ModelTransform", mOwner->GetWorldTransform());

    mMesh->BindVertexArray();
    mMesh->SetMaterialUniform(shader);  // Material, Texture‚È‚Ç‚ðÝ’è
    mMesh->UnBindVertexArray();
}