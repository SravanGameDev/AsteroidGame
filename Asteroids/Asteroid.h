#ifndef ASTEROID_H_INCLUDED
#define ASTEROID_H_INCLUDED

#include "GameEntity.h"

class Asteroid : public GameEntity
{
public:
	Asteroid(XMVECTOR position,
		XMVECTOR velocity,
		int size);

	void Update(System *system);
	void Render(Graphics *graphics) const;

	XMVECTOR GetVelocity() const;
	int GetSize() const;

private:

	XMFLOAT3 velocity_;
	XMFLOAT3 axis_;
	float angle_;
	float angularSpeed_;
	int size_;

};

#endif // ASTEROID_H_INCLUDED
