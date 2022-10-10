#include "Actor.hpp"
#include "json.hpp"
#include "Player.hpp"

namespace nl = nlohmann;


class TalkText : public Actor
{
public:
	TalkText(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
	bool GetIsFinishDraw();	// ï∂éöÇï`âÊÇµèIÇÌÇ¡ÇΩÇ©ÅH
	bool GetIsSelectText() { return mIsSelectText; }

private:

	std::u16string JsonToString(nl::json data);

	class TalkTextComponent* mTalkTextComp;
	Player::State mPreviousPlayerState;
	nl::json mData;
	std::vector<nl::json> mTalkData;
	bool mIsSelectText;
	int mTalkIdx;
	int mResultIdx;
};