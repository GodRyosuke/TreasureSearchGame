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

void MeshComponent::SetMatrixUniform(Shader* shader)
{
    shader->SetMatrixUniform("ModelTransform", mOwner->GetWorldTransform());
}

void MeshComponent::SetTexture(std::string filePath)
{
    mMesh->SetTexture(filePath);
}

void MeshComponent::Draw(Shader* shader)
{
    shader->UseProgram();

    SetMatrixUniform(shader);

    mMesh->BindVertexArray();
    mMesh->SetMaterialUniform(shader);  // Material, Textureなどを設定
    mMesh->UnBindVertexArray();
}