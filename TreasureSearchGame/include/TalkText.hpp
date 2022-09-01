#include "Text.hpp"
#include "json.hpp"

namespace nl = nlohmann;

class TalkText : public Text
{
public:
	TalkText();
	void Input(const uint8_t* keyState);
	void Draw(Shader* shader) override;


private:
	nl::json mData;
	std::u16string mText;
};