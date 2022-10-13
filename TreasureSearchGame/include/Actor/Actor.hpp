#pragma once

#include <glm.hpp>
#include <vector>

class Actor {
public:
	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime);
	// Actor���ۗL���Ă���Component���X�V
	void UpdateComponents(float deltaTime);
	// Actor�ŗL�̍X�V
	virtual void UpdateActor(float deltaTime);

	void ProcessInput(const uint8_t* keyState);
	// Acttor�ŗL�̓���
	virtual void ActorInput(const uint8_t* keyState);

	const glm::vec3& GetPosition() const { return mPosition; }
	void SetPosition(const glm::vec3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale;  mRecomputeWorldTransform = true; }
	const glm::mat4& GetRotation() const { return mRotation; }
	void SetRotation(const glm::mat4& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }

	void ComputeWorldTransform();
	const glm::mat4 GetWorldTransform() const { return mWorldTransform; };

	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;


	void RotateToNewForward(const glm::vec3& forward);


	class Game* GetGame() { return mGame; }


	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	glm::vec3 mPosition;
	glm::mat4 mRotation;
	float mScale;
	glm::mat4 mWorldTransform;
	bool mRecomputeWorldTransform;

	std::vector<class Component*> mComponents;
	class Game* mGame;
};