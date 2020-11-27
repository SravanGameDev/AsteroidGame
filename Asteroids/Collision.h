#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include <DirectXMath.h>
#include <list>

using namespace DirectX;

class GameEntity;
class Game;
class Collider;

class Collision
{
public:
	Collision();
	~Collision();

	Collider *CreateCollider(GameEntity *entity);
	void DestroyCollider(Collider *collider);

	void UpdateColliderPosition(Collider *collider, const XMFLOAT3 &position);
	void UpdateColliderRadius(Collider *collider, float radius);
	void EnableCollider(Collider *collider);
	void DisableCollider(Collider *collider);

	void DoCollisions(Game *game) const;

private:

	typedef std::list<Collider *> ColliderList;

	static bool CollisionTest(Collider *a, Collider *b);

	ColliderList colliders_;

};

#endif // COLLISION_H_INCLUDED
