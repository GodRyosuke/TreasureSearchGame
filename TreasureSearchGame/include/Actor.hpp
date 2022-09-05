#pragma once

#include <glm.hpp>
#include <vector>

class Actor {
public:
	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// Updates all the components attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);

	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const uint8_t* keyState);
	// Any actor-specific input code (overridable)
	virtual void ActorInput(const uint8_t* keyState);

	// Getters/setters
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