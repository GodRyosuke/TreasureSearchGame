#include "TreasureBox.hpp"
#include "Game.hpp"
#include"SkinMeshComponent.hpp"
#include "Player.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"

static Game::PHASE preGamePhase;

TreasureBox::TreasureBox(Game* game)
	:Actor(game)
	,mIsDraw(false)
	,mOpenTime(0)
	,mAlpha(0.f)
{
	mSkinMeshComp = new SkinMeshComponent(this);
	mSkinMeshComp->SetSkinMesh(game->GetSkinMesh("TreasureBox", "gltf"));
	mSkinMeshComp->SetAnimIdx(0);
	mSkinMeshComp->SetAnimTime(50.f);
	mSkinMeshComp->SetAlpha(0.f);

	SetPosition(game->GetTreasurePos());
	{
		glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), (float)M_PI, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f), (float)M_PI, glm::vec3(0.0f, 0.0f, 1.0f));
		SetRotation(rotate_x * rotate_z);
	}
	SetScale(0.01f / 2.f);

	preGamePhase = game->GetPhase();
}

void TreasureBox::UpdateActor(float deltatime)
{
	if ((GetGame()->GetPhase() == Game::PHASE_GAME) &&
		(mAlpha < 1.0f)) {
		// Game Phase‚É‚È‚Á‚½‚çA•ó” •`‰æ
		mSkinMeshComp->SetAlpha(mAlpha);
		mAlpha += 0.05f;
	}

	if (mOpenTime > 0) {
		// Return ‚ª‰Ÿ‰º‚³‚ê‚½‚ç
		uint32_t treasureTicks = GetGame()->GetTicksCount() - mOpenTime;
		if (treasureTicks > 1600) {
			mOpenTime = 0; // •ó” ‚ªŠJ‚¢‚½
		}
		mSkinMeshComp->SetAnimTime(treasureTicks);
	}
}

void TreasureBox::ActorInput(const uint8_t* keys)
{
	// •ó” ‚Ì‘O‚ÅReturn
	glm::vec3 playerPos = GetGame()->GetPlayer()->GetPosition();
	glm::vec3 boxPos = GetPosition();
	if (GetGame()->GetPhase() == Game::PHASE_GAME) {
		if (
			((boxPos.x - 1.f) < playerPos.x) && (playerPos.x < (boxPos.x + 1.f)) &&
			((boxPos.y - 1.f) < playerPos.y) && (playerPos.y < (boxPos.y + 1.f))
			) {
			if (keys[SDL_SCANCODE_RETURN]) {
				mOpenTime = GetGame()->GetTicksCount();
			}
		}
	}
}