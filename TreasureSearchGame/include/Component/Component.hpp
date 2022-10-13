#pragma once
#include <cstdint>

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	// �ۗLActor�X�V���ɌĂ΂��
	virtual void Update(float deltaTime);
	// Component�ŗL�̓��͏���
	virtual void ProcessInput(const uint8_t* keyState) {}
	virtual void OnUpdateWorldTransform() { }

	class Actor* GetOwner() { return mOwner; }
	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	// �ۗLActor
	class Actor* mOwner;
	// Update order of component
	int mUpdateOrder;
};
