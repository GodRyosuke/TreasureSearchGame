#pragma once

#include "Component.hpp"
#include <string>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner, bool isSkeletal = false);
	~MeshComponent(){}
	void Draw(class Shader* shader);
	void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTexture(std::string filePath);
	bool GetIsSkeltal() { return mIsSkeletal; }


protected:
	virtual void SetMatrixUniform(class Shader* shader);
	bool mIsSkeletal;

private:
	class Mesh* mMesh;
};