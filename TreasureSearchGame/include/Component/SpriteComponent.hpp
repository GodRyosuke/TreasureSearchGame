#pragma once

#include <string>
#include "glm.hpp"
#include "Component.hpp"

class SpriteComponent : public Component {
public:
	enum Type {
		SPRITE,
		TEXT,
		TALK_TEXT,
		NUM_TYPE
	};

	SpriteComponent(class Actor* owner, Type type = SPRITE);
	~SpriteComponent();
	bool Load(std::string filePath, float alpha = 1.0f);
	virtual void Draw(class Shader* shader);

	void SetAlpha(float alpha) { mAlpha = alpha; }
	void SetIsDraw(bool isdraw) { mIsDraw = isdraw; }
	bool GetIsDraw() { return mIsDraw; }
	Type GetType() { return mType; }

protected:
	virtual void SetUniforms(class Shader* shader);

	float mAlpha;
	unsigned int mVertexArray;
	unsigned int mVertexBuffer;
	unsigned int mIndexBuffer;
	Type mType;

private:

	class Texture* mTexture;
	bool mIsDraw;
};