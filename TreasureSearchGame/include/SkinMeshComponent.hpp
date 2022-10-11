#pragma once

#include "MeshComponent.hpp"
#include "glm.hpp"
#include <vector>

class SkinMeshComponent : public MeshComponent
{
public:
	SkinMeshComponent(class Actor* owner);
	void Update(float deltatime) override;
	void Draw(class Shader* shader) override;
	void SetSkinMesh(class SkinMesh* mesh) { mSkinMesh = mesh; }
	void SetAnimIdx(int idx) { mAnimIdx = idx; }
	void SetStopAnimation(bool stop) { mStopAnimation = stop; }
	void SetZeroTransFrag(bool frag) { mZeroTransFrag = frag; }
	void SetAnimTime(float time) { mAnimtime = time; }
	void SetIsOneAnim(bool one);


private:
	//void SetMatrixUniform(class Shader* shader) override;

	int mAnimIdx;
	bool mZeroTransFrag;
	bool mStopAnimation;
	float mAnimtime;
	class SkinMesh* mSkinMesh;
	std::vector<glm::mat4> mBoneMatrixPallete;
	bool mIsOneAnim;
	uint32_t mAnimStartClock;
};