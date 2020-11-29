#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include <string>

#include "GameState.h"

using namespace std;

class Score:public GameState
{
public:
	Score();
	~Score();

	int score_;
	string player;

	int highScore_;
	string players;

	void OnActivate(System* system, StateArgumentMap& args);
	void OnUpdate(System* system);
	void OnRender(System* system);
	void OnDeactivate(System* system);

private:

};


#endif // !SCORE_H_INCLUDED