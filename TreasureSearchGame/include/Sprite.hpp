#pragma once

#include <string>
#include <Texture.hpp>
#include "glm.hpp"


class Sprite {
public:
	Sprite(){}
	bool Load(std::string filePath, float alpha = 1.0f);
	virtual void Draw(class Shader* shader);
	void SetPos(glm::vec3 pos) { mPos = pos; }
	void SetRotate(glm::mat4 rot) { mRotate = rot; }
	void SetScale(float scale) { mScale = scale; }

	void SetAlpha(float alpha) { mAlpha = alpha; }
protected:
	virtual void SetUniforms(class Shader* shader);

	glm::vec3 mPos;
	glm::mat4 mRotate;
	float mScale;
	float mAlpha;
	unsigned int mVertexArray;

private:
	Texture* mTexture;
};