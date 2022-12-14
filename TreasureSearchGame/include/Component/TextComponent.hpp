#pragma once

#include "SpriteComponent.hpp"
#include <map>
#include "ft2build.h"
#include "glew.h"

#include FT_FREETYPE_H


class TextComponent : public SpriteComponent {
public:
	TextComponent(class Actor* owner, Type type = TEXT);
	~TextComponent();
	virtual void Draw(class Shader* shader) override;
	void DrawTalkText(class Shader* shader);
	virtual void Update(float deltaTime) override;
	void StartDrawing();
	void SetTextColor(glm::vec3 textColor) { mTextColor = textColor; }
	void SetText(std::u16string text) { mText = text; }
	void SetDrawSpeed(float speed) { mDrawSpeed = speed; }

protected:
	struct TexChar {
		GLuint texID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
	};

	TexChar	LoadUTFChar(char16_t c);
	std::map<char16_t, TexChar> mJapanTexChars;
	void SetUniforms(class Shader* shader) override;


private:

	FT_Face mFontFace;
	FT_Library mFTlibrary;
	glm::vec3 mTextColor;
	std::u16string mText;
	float mDrawSpeed;
	glm::vec3 mCurrentTextPos;
};