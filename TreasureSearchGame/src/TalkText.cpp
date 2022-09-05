#include "TalkText.hpp"
#include "Shader.hpp"
#include <fstream>
#include <codecvt>


TalkText::TalkText()
	:Text()
{
	// ��b�Ɏg��������ǂݍ���
	{
		std::string filePath = "./resources/TextData.json";
		std::ifstream ifs(filePath.c_str());
		if (ifs.good())
		{
			ifs >> mData;
		}
		ifs.close();
	}
}

void TalkText::Input(const uint8_t* keyState)
{

}

void TalkText::Draw(Shader* shader)
{
	shader->UseProgram();
	glBindVertexArray(mVertexArray);

	glm::vec3 FontCenter = glm::vec3(0.0f);
	// ������texchar�̑傫�����擾
	int FontWidth = 0;
	{
		FontWidth = (mJapanTexChars.begin()->second.Advance >> 6) * mScale;
		FontCenter.x = FontWidth * 20 / 2.0f; // ��������20����
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
		// 3�s�ȓ��Ɏ��߂�
		if ((rowCount < 3) && (str[i] == '\n')) {
			x2 = -FontCenter.x;
			y2 -= FontWidth;	// ���s������
			rowCount++;
			continue;
		}

		auto itr = mJapanTexChars.find(str[i]);
		TexChar ch;
		if (itr == mJapanTexChars.end()) {		// �܂��ǂݍ��܂�Ă��Ȃ������Ȃ�
			ch = LoadUTFChar(str[i]);
			mJapanTexChars.insert(std::make_pair(str[i], ch));
		}
		else {
			ch = itr->second;
		}

		float xpos = x2 + ch.Bearing.x * mScale;
		float ypos = y2 - (ch.Size.y - ch.Bearing.y) * mScale;
		float w = ch.Size.x * mScale;
		float h = ch.Size.y * mScale;


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
		x2 += (ch.Advance >> 6) * mScale;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

std::u16string TalkText::GetText(nl::json data)
{
	std::string str;
	data.get_to(str);
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	return convert.from_bytes(str);
}

void TalkText::ShowTalkText(Shader* shader)
{
	SetText(GetText(mData["TalkCostomer"]["Welcome"]["Talk2"]));
	SetPos(glm::vec3(0.0f, -150.0f, 0.f));
	SetTextColor(glm::vec3(0.f, 0.f, 0.2f));
	DrawTalkText(shader);
}

