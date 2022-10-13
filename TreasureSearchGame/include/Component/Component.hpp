#pragma once
#include <cstdint>

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	// 保有Actor更新時に呼ばれる
	virtual void Update(float deltaTime);
	// Component固有の入力処理
	virtual void ProcessInput(const uint8_t* keyState) {}
	virtual void OnUpdateWorldTransform() { }

	class Actor* GetOwner() { return mOwner; }
	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	// 保有Actor
	class Actor* mOwner;
	// Update order of component
	int mUpdateOrder;
};
