#include "Actor.hpp"

class Counter : public Actor
{
public:
	Counter(class Game* game);

};

class CounterBox : public Actor
{
public:
	CounterBox(class Game* game);
	enum Type {
		WHITE,
		BLACK,
		NUM_TYPE
	};
	void SetBoxType(const Type type) { mType = type; }
	void UpdateActor(float deltatime) override;

private:
	Type mType;
	class MeshComponent* mMeshComp;
};