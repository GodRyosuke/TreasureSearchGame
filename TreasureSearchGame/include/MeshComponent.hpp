#pragma once

#include "Component.hpp"
#include <string>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner, bool isSkeletal = false);
	~MeshComponent(){}
	virtual void Draw(class Shader* shader);
	void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTexture(std::string filePath) { mTexturePath = filePath; }
	bool GetIsSkeltal() { return mIsSkeletal; }
	void SetAlpha(float alpha) { mAlpha = alpha; }

protected:
	//virtual void SetMatrixUniform(class Shader* shader);
	bool mIsSkeletal;
	float mAlpha;

private:
	class Mesh* mMesh;
	std::string mTexturePath;
};