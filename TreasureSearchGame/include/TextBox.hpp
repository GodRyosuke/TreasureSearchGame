#include "Actor.hpp"

class TextBox : public Actor
{
public:
	TextBox(class Game* game);

private:
	class Sprite* mSprite;
};