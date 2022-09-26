#include "Actor.hpp"
#include "json.hpp"

namespace nl = nlohmann;


class TalkText : public Actor
{
public:
	TalkText(class Game* game);
	void UpdateActor(float deltaTime) override;

private:
	std::u16string JsonToString(nl::json data);

	class TalkTextComponent* mTalkTextComp;
	nl::json mData;
};