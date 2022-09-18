#include "Roof.hpp"
#include "Game.hpp"
#include "MeshComponent.hpp"

Roof::Roof(Game* game)
	:Actor(game)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetMesh("Roof", "obj"));
}