#ifndef USERINTERFACE_H_INCLUDED
#define USERINTERFACE_H_INCLUDED

#include <string>

#include "GameEntity.h"

using namespace std;

class UserInterface: GameEntity
{
public:
	UserInterface();
	~UserInterface();

	int playerScore_;
	int highScore_;

	void Update(System* system);
	void Render(Graphics* graphics) const;

	void ResetScore();
	void UpdateScore();

private:

};


#endif // !USERINTERFACE_H_INCLUDED