#include "TextComponent.hpp"
#include <iostream>
#include "Shader.hpp"
#include "Actor.hpp"
#include "Game.hpp"

TextComponent::TextComponent(Actor* owner)
	:SpriteComponent(owner)
{
	mOwner->GetGame()->AddTextComp(this);

	// TTF“Ç‚İ‚İ
	FT_Library library;
	//FT_Face face;
	FT_GlyphSlot slot;
	// Load Font
	FT_Init_FreeType(&library);
	FT_New_Face(library, "./resources/arialuni.ttf", 0, &mFontFace);
	FT_Select_Charmap(mFontFace, ft_encoding_unicode);
	FT_Set_Pixel_Sizes(mFontFace, 0, 48);
	slot = mFontFace->glyph;

	// ‰Šú‚Ì•¶š—ñ‚ğ“Ç‚İ‚Ş
	{
		const char16_t str[] = u"’©‰iUˆê˜Y";

		for (int i = 0; str[i] != '\0'; i++) {
			TexChar tc = LoadUTFChar(str[i]);
			mJapanTexChars.insert(std::make_pair(str[i], tc));
		}
	}


	// Vertex Array ì¬
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

TextComponent::TexChar TextComponent::LoadUTFChar(char16_t c)
{
	if (FT_Load_Glyph(mFontFace, FT_Get_Char_Index(mFontFace, c), FT_LOAD_RENDER)) {
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		exit(-1);
	}
	GLuint tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, mFontFace->glyph->bitmap.width, mFontFace->glyph->bitmap.rows,
		0, GL_RED, GL_UNSIGNED_BYTE, mFontFace->glyph->bitmap.buffer);

	TexChar	 tc = {
		tex,
		glm::ivec2(mFontFace->glyph->bitmap.width, mFontFace->glyph->bitmap.rows),
		glm::ivec2(mFontFace->glyph->bitmap_left, mFontFace->glyph->bitmap_top),
		mFontFace->glyph->advance.x
	};

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(tex, 0);		// unbind

	return tc;
}

void TextComponent::SetUniforms(Shader* shader)
{
	// translate rotate alpha colorİ’è

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), mOwner->GetPosition());
	shader->SetMatrixUniform("gTranslate", translate);
	shader->SetMatrixUniform("gRotate", mOwner->GetRotation());
	shader->SetFloatUniform("gSpriteAlpha", mAlpha);
	shader->SetVectorUniform("textColor", mTextColor);
}

void TextComponent::Draw(Shader* shader)
{
	shader->UseProgram();
	glBindVertexArray(mVertexArray);

	float scale = mOwner->GetScale();
	glm::vec3 FontCenter = glm::vec3(0.0f);
	// •¶š‚Ìtexchar‚Ì‘å‚«‚³‚ğæ“¾
	{
		int width = (mJapanTexChars.begin()->second.Advance >> 6) * scale;
		FontCenter.x = (width * mText.length()) / 2.0f;
		FontCenter.y = width / 2.0f;
	}
	SetUniforms(shader);

	glActiveTexture(GL_TEXTURE0);

	int x2 = -FontCenter.x;
	int y2 = -FontCenter.y;
	//float scale = 1.0f;
	const char16_t* str = mText.c_str();
	for (int i = 0; str[i] != '\0'; i++) {
		auto itr = mJapanTexChars.find(str[i]);
		TexChar ch;
		if (itr == mJapanTexChars.end()) {		// ‚Ü‚¾“Ç‚İ‚Ü‚ê‚Ä‚¢‚È‚¢•¶š‚È‚ç
			ch = LoadUTFChar(str[i]);
			mJapanTexChars.insert(std::make_pair(str[i], ch));
		}
		else {
			ch = itr->second;
		}

		float xpos = x2 + ch.Bearing.x * scale;
		float ypos = y2 - (ch.Size.y - ch.Bearing.y) * scale;
		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;


		float textVertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.texID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textVertices), textVertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x2 += (ch.Advance >> 6) * scale;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextComponent::StartDrawing()
{
	glm::vec3 FontCenter = glm::vec3(0.0f);
	// •¶š‚Ìtexchar‚Ì‘å‚«‚³‚ğæ“¾
	{
		int width = (mJapanTexChars.begin()->second.Advance >> 6) * mOwner->GetScale();
		FontCenter.x = (width * mText.length()) / 2.0f;
		FontCenter.y = width / 2.0f;
	}
	mCurrentTextPos = -FontCenter;
}

void TextComponent::DrawTalkText(Shader* shader)
{
	shader->UseProgram();
	glBindVertexArray(mVertexArray);

	float scale = mOwner->GetScale();
	glm::vec3 FontCenter = glm::vec3(0.0f);
	// •¶š‚Ìtexchar‚Ì‘å‚«‚³‚ğæ“¾
	int FontWidth = 0;
	{
		FontWidth = (mJapanTexChars.begin()->second.Advance >> 6) * scale;
		FontCenter.x = FontWidth * 20 / 2.0f; // •¶š”‚Í20•¶š
		FontCenter.y = FontWidth / 2.0f;
	}
	SetUniforms(shader);

	glActiveTexture(GL_TEXTURE0);

	int x2 = -FontCenter.x;
	int y2 = -FontCenter.y;
	int rowCount = 1;
	//float scale = 1.0f;
	const char16_t* str = mText.c_str();
	for (int i = 0; str[i] != '\0'; i++) {
		// 3sˆÈ“à‚Éû‚ß‚é
		if ((rowCount < 3) && (str[i] == '\n')) {
			x2 = -FontCenter.x;
			y2 -= FontWidth;	// ‰üs‚ğ‚·‚é
			rowCount++;
			continue;
		}
		
		auto itr = mJapanTexChars.find(str[i]);
		TexChar ch;
		if (itr == mJapanTexChars.end()) {		// ‚Ü‚¾“Ç‚İ‚Ü‚ê‚Ä‚¢‚È‚¢•¶š‚È‚ç
			ch = LoadUTFChar(str[i]);
			mJapanTexChars.insert(std::make_pair(str[i], ch));
		}
		else {
			ch = itr->second;
		}

		float xpos = x2 + ch.Bearing.x * scale;
		float ypos = y2 - (ch.Size.y - ch.Bearing.y) * scale;
		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;


		float textVertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.texID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textVertices), textVertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x2 += (ch.Advance >> 6) * scale;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextComponent::Update(float deltatime)
{
	mCurrentTextPos += mDrawSpeed * deltatime;
}