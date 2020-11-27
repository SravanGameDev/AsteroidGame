#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include <DirectXMath.h>

using namespace DirectX;

class GameEntity;

class Collider
{
	friend class Collision;
private:
	XMFLOAT3 position;
	float radius;
	GameEntity *entity;
	bool enabled;
};

#endif // COLLIDER_H_INCLUDED
