#include "TextComponent.hpp"

class TalkTextComponent : public TextComponent
{
public:
	TalkTextComponent(class Actor* owner);
	void Input(const uint8_t* keyState);
	virtual void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	void SetText(std::u16string text) { mText = text; }
	void InittextPos();
	bool GetIsFinishDraw() { return mIsFnishDraw; }


private:
	std::u16string mText;
	float mDrawSpeed;
	float mCurrentCursorPos;
	int mCurrentTextIdx;
	bool mIsFnishDraw;
};