#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include "GameEntity.h"

class Graphics;

class Ship : public GameEntity
{
public:
	Ship();

	void SetControlInput(float acceleration,
		float rotation);

	void Update(System *system);
	void Render(Graphics *graphics) const;

	XMVECTOR GetForwardVector() const;
	XMVECTOR GetVelocity() const;

	void Reset();

private:

	float accelerationControl_;
	float rotationControl_;

	XMFLOAT3 velocity_;
	XMFLOAT3 forward_;
	float rotation_;

};

#endif // SHIP_H_INCLUDED
