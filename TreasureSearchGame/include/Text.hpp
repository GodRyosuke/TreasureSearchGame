#pragma once

#include "Sprite.hpp"
#include <map>
#include "ft2build.h"

#include FT_FREETYPE_H


class Text : public Sprite {
public:
	Text();
	virtual void Draw(Shader* shader) override;
	void DrawTalkText(Shader* shader);
	void Update(float deltaTime);
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
	void SetUniforms(Shader* shader) override;

	unsigned int mVertexBuffer;

private:



	FT_Face mFontFace;
	glm::vec3 mTextColor;
	std::u16string mText;
	float mDrawSpeed;
	glm::vec3 mCurrentTextPos;

};