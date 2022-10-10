#include "TalkTextComponent.hpp"
#include "Shader.hpp"
#include "Actor.hpp"
#include <fstream>


TalkTextComponent::TalkTextComponent(Actor* owner)
	:TextComponent(owner, TALK_TEXT)
	,mDrawSpeed(11)
	,mCurrentTextIdx(0)
	,mIsFnishDraw(false)
{
	int FontWidth = (mJapanTexChars.begin()->second.Advance >> 6) * mOwner->GetScale();
	
}

void TalkTextComponent::Input(const uint8_t* keyState)
{

}

void TalkTextComponent::InittextPos()
{
	const char16_t* str = mText.c_str();
	for (int i = 0; str[i] != '\0'; i++) {
		if ((str[i] != u'Å@') && (str[i] != '\n') && (str[i] != ' ')) {
			mCurrentTextIdx = i;
			break;
		}
	}
	mCurrentCursorPos = mCurrentTextIdx;
	mIsFnishDraw = false;
}

void TalkTextComponent::Update(float deltatime)
{
	if (mCurrentTextIdx == (mText.size() - 1)) {
		mIsFnishDraw = true;
		return;
	}
	mCurrentCursorPos += mDrawSpeed * deltatime;
	mCurrentTextIdx = static_cast<int>(mCurrentCursorPos);
}

void TalkTextComponent::Draw(Shader* shader)
{
	if (GetIsDraw() == false) {
		return;
	}
	shader->UseProgram();
	glBindVertexArray(mVertexArray);

	glm::vec3 FontCenter = glm::vec3(0.0f);
	// ï∂éöÇÃtexcharÇÃëÂÇ´Ç≥ÇéÊìæ
	//int FontWidth = (mJapanTexChars.begin()->second.Advance >> 6) * mOwner->GetScale();
	const int FontWidth = 48;
	FontCenter.x = FontWidth * 20 / 2.0f; // ï∂éöêîÇÕ20ï∂éö
	FontCenter.y = FontWidth / 2.0f;
	SetUniforms(shader);

	glActiveTexture(GL_TEXTURE0);

	int x2 = -FontCenter.x;
	int y2 = -FontCenter.y;
	int rowCount = 1;
	//float scale = 1.0f;
	const char16_t* str = mText.c_str();
	for (int i = 0; str[i] != '\0'; i++) {
		if (mCurrentTextIdx < i) {
			break;
		}
		// 3çsà»ì‡Ç…é˚ÇﬂÇÈ
		if ((rowCount < 3) && (str[i] == '\n')) {
			x2 = -FontCenter.x;
			y2 -= FontWidth;	// â¸çsÇÇ∑ÇÈ
			rowCount++;
			continue;
		}

		auto itr = mJapanTexChars.find(str[i]);
		TexChar ch;
		if (itr == mJapanTexChars.end()) {		// Ç‹Çæì«Ç›çûÇ‹ÇÍÇƒÇ¢Ç»Ç¢ï∂éöÇ»ÇÁ
			ch = LoadUTFChar(str[i]);
			mJapanTexChars.insert(std::make_pair(str[i], ch));
		}
		else {
			ch = itr->second;
		}

		float xpos = x2 + ch.Bearing.x * mOwner->GetScale();
		float ypos = y2 - (ch.Size.y - ch.Bearing.y) * mOwner->GetScale();
		float w = ch.Size.x * mOwner->GetScale();
		float h = ch.Size.y * mOwner->GetScale();


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
		x2 += (ch.Advance >> 6) * mOwner->GetScale();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}



//void TalkTextComponent::ShowTalkText(Shader* shader)
//{
//	SetText(GetText(mData["TalkCostomer"]["Welcome"]["Talk2"]));
//	mOwner->SetPosition(glm::vec3(0.0f, -150.0f, 0.f));
//	SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
//	DrawTalkText(shader);
//}


