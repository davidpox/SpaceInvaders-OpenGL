#include "gamestate.h"



gamestate::gamestate()
{
	alienMoveTimer = 0;
	alienMoveCounter = 0;
	isGameRunning = false;
	isMovingLeft = false;
	playerscore = 0;
	playerlives = 3;
	gameover = false;
}


gamestate::~gamestate()
{
}
