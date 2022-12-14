#include "Actor.hpp"

// ???ق??т̕???
class TreasureBox : public Actor
{
public:
	TreasureBox(class Game* game);
	void UpdateActor(float deltatime) override;
	void ActorInput(const uint8_t* keys) override;
	void Init();

private:
	class SkinMeshComponent* mSkinMeshComp;
	bool mIsDraw;
	uint32_t mOpenTime;
	float mAlpha;
};