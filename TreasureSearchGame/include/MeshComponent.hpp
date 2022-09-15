#pragma once

#include "Component.hpp"
#include <string>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent(){}
	void Draw(class Shader* shader);
	void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTexture(std::string filePath);


protected:
	virtual void SetMatrixUniform(class Shader* shader);

private:
	class Mesh* mMesh;
};