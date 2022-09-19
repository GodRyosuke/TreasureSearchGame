#include "TextComponent.hpp"
#include "json.hpp"

namespace nl = nlohmann;

class TalkTextComponent : public TextComponent
{
public:
	TalkTextComponent(class Actor* owner);
	void Input(const uint8_t* keyState);
	void Draw(Shader* shader) override;
	void ShowTalkText(Shader* shader);


private:
	std::u16string GetText(nl::json data);

	nl::json mData;
	std::u16string mText;
};