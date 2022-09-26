#include "Actor.hpp"

class Clerk : public Actor
{
public:
	enum State {
		IDLE,
		BOW,
		TALK,
		NUM_STATE
	};

	Clerk(class Game* game);
	void UpdateActor(float deltaTime) override;


private:
	class SkinMeshComponent* mSkinMeshComp;

	float mPlayerRot;
	State mState;
};