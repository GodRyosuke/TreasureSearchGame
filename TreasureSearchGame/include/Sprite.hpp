#pragma once

#include <string>
#include "glm.hpp"
#include "Component.hpp"

class Sprite : public Component {
public:
	Sprite(class Actor* owner);
	bool Load(std::string filePath, float alpha = 1.0f);
	virtual void Draw(class Shader* shader);

	void SetAlpha(float alpha) { mAlpha = alpha; }
protected:
	virtual void SetUniforms(class Shader* shader);

	float mAlpha;
	unsigned int mVertexArray;

private:
	class Texture* mTexture;
};