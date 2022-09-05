#include "Text.hpp"
#include "json.hpp"

namespace nl = nlohmann;

class TalkText : public Text
{
public:
	TalkText();
	void Input(const uint8_t* keyState);
	void Draw(Shader* shader) override;
	void ShowTalkText(Shader* shader);


private:
	std::u16string GetText(nl::json data);

	nl::json mData;
	std::u16string mText;
};