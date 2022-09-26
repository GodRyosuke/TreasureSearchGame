#include "Plane.hpp"
#include <iostream>
#include "Game.hpp"
#include "MeshComponent.hpp"

Plane::Plane(Game* game)
	:Actor(game)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetMesh("Plane", "obj"));
}

void Plane::UpdateActor(float deltatime)
{
	if (mType == CONCRETE) {
		mMeshComp->SetTexture("./resources/Plane/Textures/concrete_brick_wall_001_diffuse_4k.jpg");
	}
	else if (mType == BRICK) {
		mMeshComp->SetTexture("./resources/Plane/Textures/Bricks077_4K_Color.jpg");
	}
	else {
		mMeshComp->SetTexture("");
	}
}

