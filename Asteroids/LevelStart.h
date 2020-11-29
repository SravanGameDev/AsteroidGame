#ifndef LEVELSTART_H_INCLUDE
#define LEVELSTART_H_INCLUDE

#include "GameState.h"

class LevelStart : public GameState
{
public:
	LevelStart();

	void OnActivate(System *system, StateArgumentMap &args);
	void OnUpdate(System *system);
	void OnRender(System *system);
	void OnDeactivate(System *system);

private:
	int level_;
	int delay_;
};

#endif // LEVELSTART_H_INCLUDE
