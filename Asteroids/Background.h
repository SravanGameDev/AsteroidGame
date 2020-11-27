#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

#include "GameEntity.h"
#include "ImmediateModeVertex.h"

class Background : public GameEntity
{
public:
	Background(float width, float height);

	void Update(System *system);
	void Render(Graphics *graphics) const;

private:

	enum { NUM_STARS = 256 };

	ImmediateModeVertex stars_[NUM_STARS];

};

#endif // BACKGROUND_H_INCLUDED
