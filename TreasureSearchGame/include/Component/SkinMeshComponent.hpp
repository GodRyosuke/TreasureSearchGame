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
	void SetAnimTime(float time) { mAnimtime = time; mStopAnimation = true; }
	void SetIsOneAnim(bool one);
	bool GetIsOneAnim() const { return mIsOneAnim; }
	bool GetIsFinishOneAnim();


private:
	int mAnimIdx;
	bool mZeroTransFrag;
	bool mStopAnimation;
	float mAnimtime;
	class SkinMesh* mSkinMesh;
	std::vector<glm::mat4> mBoneMatrixPallete;
	bool mIsOneAnim;
	bool mIsFinishOneAnim;
	uint32_t mAnimStartClock;
};