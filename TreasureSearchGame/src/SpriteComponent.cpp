#include "Actor.hpp"
#include "SpriteComponent.hpp"
#include "Shader.hpp"
#include "Game.hpp"
#include "Texture.hpp"

static float SpriteVertices[] =
{ //     COORDINATES     /        Normals      /   TexCoord  //
	-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f, // top left
	0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f,	// top right
	0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f,	// bottom right
	-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f	// bottom left
};

static unsigned int SpriteIndices[] =
{
	0, 1, 2,
	2, 3, 0
};

SpriteComponent::SpriteComponent(Actor* owner)
	:Component(owner)
	,mIsDraw(true)
{
	mOwner->GetGame()->AddSpriteComp(this);
}

bool SpriteComponent::Load(std::string filePath, float alpha)
{
	mAlpha = alpha;
	// Texture“Ç‚Ýž‚Ý
	mTexture = new Texture(filePath);

	// Vertex Array Objectì¬
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertices), SpriteVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SpriteIndices), SpriteIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind cube vertex arrays
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}


void SpriteComponent::SetUniforms(Shader* shader)
{
	glm::mat4 texScalingMat = glm::scale(glm::mat4(1.0f), glm::vec3(mTexture->getWidth(), mTexture->getHeight(), 1.f));
	glm::mat4 world = mOwner->GetWorldTransform() * texScalingMat;
	shader->SetMatrixUniform("ModelTransform", world);
	shader->SetFloatUniform("gSpriteAlpha", mAlpha);
}

void SpriteComponent::Draw(Shader* shader)
{
	if (!mIsDraw) {
		return;
	}
	shader->UseProgram();
	glBindVertexArray(mVertexArray);

	// sprite‚Ì•`‰æˆÊ’u‚ðÝ’è
	SetUniforms(shader);	// TransformÝ’è
	mTexture->BindTexture();
	glDrawElements(GL_TRIANGLES, sizeof(SpriteIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	mTexture->UnBindTexture();
}

