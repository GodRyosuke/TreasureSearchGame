#include "Game.hpp"
#include "Actor/Counter.hpp"
#include "Actor/MazeBox.hpp"
#include "Component/MeshComponent.hpp"

Counter::Counter(Game* game)
	:Actor(game)
{
	CounterBox* cb = nullptr;
	for (int z = 0; z < 2; z++) {
		for (int y = 0; y < 7; y++) {
			cb = new CounterBox(game);
			cb->SetBoxType((z * 7 + y) % 2 ? CounterBox::BLACK : CounterBox::WHITE);
			cb->SetPosition(glm::vec3(1.75f, 0.5f * y + 0.25, 0.5f * z + 0.5f));
			cb->SetScale(0.25f);
		}
		for (int x = 0; x < 3; x++) {
			cb = new CounterBox(game);
			cb->SetScale(0.25f);
			cb->SetBoxType((z * 3 + x) % 2 ? CounterBox::WHITE : CounterBox::BLACK);
			cb->SetPosition(glm::vec3(0.5f * x + 0.25, 3.25f, 0.5f * z + 0.5f));
		}
	}
}

CounterBox::CounterBox(Game* game)
	:Actor(game)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetMesh("MazeBox", "fbx"));
}

void CounterBox::UpdateActor(float deltatime)
{
	if (mType == WHITE) {
		mMeshComp->SetTexture("./resources/MazeBox/WhiteUV.png");
	}
	else if (mType == BLACK) {
		mMeshComp->SetTexture("./resources/MazeBox/BlackUV.png");
	}
	else {
		mMeshComp->SetTexture("");
	}
}
