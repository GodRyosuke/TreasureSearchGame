#pragma once

#include "Sprite.hpp"
#include <map>
#include "ft2build.h"
#include FT_FREETYPE_H

class Text : public Sprite {
public:
	Text(std::string ttfPath);
	void Draw(Shader* shader) override;
	void SetTextColor(glm::vec3 textColor) { mTextColor = textColor; }
	void SetText(std::u16string text) { mText = text; }

private:
	struct TexChar {
		GLuint texID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
	};

	void SetUniforms(Shader* shader) override;

	TexChar	LoadUTFChar(char16_t c);
	std::map<char16_t, TexChar> mJapanTexChars;
	FT_Face mFontFace;
	glm::vec3 mTextColor;
	std::u16string mText;
	unsigned int mVertexBuffer;
};