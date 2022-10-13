#include "Actor.hpp"

class Roof : public Actor
{
public:
	Roof(class Game* game);

private:
	class MeshComponent* mMeshComp;
};