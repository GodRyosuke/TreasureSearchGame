#include "TextBox.hpp"
#include "Game.hpp"
#include "Sprite.hpp"

TextBox::TextBox(Game* game)
	:Actor(game)
{
	mSprite = new Sprite(this);
	mSprite->Load("./resources/TextBox.png", 0.7f);
}