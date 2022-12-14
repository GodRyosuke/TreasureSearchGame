#include "Actor.hpp"

// 文字を描画するテクストボックス
class TextBox : public Actor
{
public:
	TextBox(class Game* game);
	void UpdateActor(float deltatime) override;
	void ActorInput(const uint8_t* keyState) override;


private:
	class SpriteComponent* mSprite;
	bool mIsFrontCounter;
};