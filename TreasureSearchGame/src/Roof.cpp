#include "Roof.hpp"
#include "Game.hpp"
#include "MeshComponent.hpp"

Roof::Roof(Game* game)
	:Actor(game)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetMesh("Roof", "obj"));
	SetPosition(glm::vec3(15.f, 15.f, 4.0f));
	SetRotation(glm::mat4(1.f));
	SetScale(15.f);
}