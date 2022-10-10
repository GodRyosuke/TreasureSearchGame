#include "Actor.hpp"

class TreasureBox : public Actor
{
public:
	TreasureBox(class Game* game);
	void UpdateActor(float deltatime) override;
	void ActorInput(const uint8_t* keys) override;


private:
	class SkinMeshComponent* mSkinMeshComp;
	bool mIsDraw;
	uint32_t mOpenTime;
	float mAlpha;
};