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

class TimerText : public Actor
{
public:
	TimerText(class Game* game);
	void UpdateActor(float deltatime) override;

private:
	class TextComponent* mTextComp;
	uint32_t mStartTime;
	int mTime;
	int mMaxTime;
};