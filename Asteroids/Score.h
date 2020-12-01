#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include <string>

#include "GameEntity.h"

using namespace std;

class Score: GameEntity
{
public:
	Score();
	~Score();

	int playerScore_;

	void Update(System* system);
	void Render(Graphics* graphics) const;


	void ResetScore();
	void HighScore(int playerScore_);

private:

};


#endif // !SCORE_H_INCLUDED