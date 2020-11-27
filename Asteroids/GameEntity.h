#ifndef GAMEENTITY_H_INCLUDED
#define GAMEENTITY_H_INCLUDED

#include <DirectXMath.h>

using namespace DirectX;

class System;
class Graphics;
class Collision;
class Collider;

class GameEntity
{
public:
	GameEntity();
	virtual ~GameEntity();

	virtual void Update(System *system);
	virtual void Render(Graphics *graphics) const;

	XMVECTOR GetPosition() const;
	void SetPosition(XMVECTOR position);

	void EnableCollisions(Collision *collisionSystem, float radius);
	void DisableCollisions();

private:

	bool HasValidCollider() const;
	void DestroyCollider();

	XMFLOAT3 position_;
	Collision *collisionSystem_;
	Collider *collider_;

};

#endif // GAMEENTITY_H_INCLUDED
