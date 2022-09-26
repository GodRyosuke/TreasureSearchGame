#include "Game.hpp"
#include "Counter.hpp"
#include "MazeBox.hpp"

Counter::Counter(Game* game)
	:Actor(game)
{
	MazeBox* box = nullptr;
	for (int z = 0; z < 2; z++) {
		for (int y = 0; y < 7; y++) {
			box = new MazeBox(game);
			box->SetScale(0.25f);
			box->SetBoxType((z * 7 + y) % 2 ? MazeBox::BLACK : MazeBox::WHITE);
			box->SetPosition(glm::vec3(1.75f, 0.5f * y + 0.25, 0.5f * z + 0.5f));
		}
		for (int x = 0; x < 3; x++) {
			box = new MazeBox(game);
			box->SetScale(0.25f);
			box->SetBoxType((z * 3 + x) % 2 ? MazeBox::WHITE : MazeBox::BLACK);
			box->SetPosition(glm::vec3(0.5f * x + 0.25, 3.25f, 0.5f * z + 0.5f));
		}
	}
}
