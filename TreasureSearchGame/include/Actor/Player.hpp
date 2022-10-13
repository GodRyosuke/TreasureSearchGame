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
	void WaitSeconds(uint32_t second);	// ���ꂪ�Ă΂�Ă���ԁA���͂��󂯕t���Ȃ�
	bool GetIsWaitSeconds() { return mIsWaitSeconds; }
	void OpenChest();	// �󔠂��󂯂�A�j���[�V����
	void SetPlayerRot(float rotate);

private:
	class FollowCamera* mFollowCamera;
	class SkinMeshComponent* mSkinMeshComp;
	float mMoveSpeed;

	float mPlayerRot;
	State mPreviousState;
	State mState;
	bool mIsWaitSeconds;
	bool mIsOneAnim;
	uint32_t mStopTime;

	// Debug�p
	class TextComponent* mDebugText;
	class TextComponent* mGamePhaseText;
};
