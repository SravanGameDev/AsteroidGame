#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "GameEntity.h"

class Bullet : public GameEntity
{
public:
	Bullet(XMVECTOR position,
		XMVECTOR direction);

	void Update(System *system);
	void Render(Graphics *graphics) const;

private:

	XMFLOAT3 velocity_;

};

#endif // BULLET_H_INCLUDED
