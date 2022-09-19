#pragma once

#include "Actor.hpp"
#include <string>

class UserAssistText : public Actor
{
public:
	UserAssistText(class Game* game);
	void UpdateActor(float deltaTime) override;


private:
	class TextComponent* mTextComp;
};