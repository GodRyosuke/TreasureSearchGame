#pragma once
#include "Actor.hpp"

class Player : public Actor
{
public:
	enum State {
		WALK,
		IDLE,
		TALK,
		OPEN_CHEST,
		BOW,
		NUM_STATE
	};

	Player(class Game* game);

	void ActorInput(const uint8_t* keys) override;
	void UpdateActor(float deltaTime) override;
	State GetState() { return mState; }
	void SetState(State state) { mState = state; }
	void WaitSeconds(uint32_t second);

	void SetPlayerRot(float rotate);

private:
	class FollowCamera* mFollowCamera;
	class SkinMeshComponent* mSkinMeshComp;
	class TextComponent* mDebugText;
	float mMoveSpeed;

	float mPlayerRot;
	State mPreviousState;
	State mState;
	bool mIsWaitSeconds;
	uint32_t mStopTime;
};
