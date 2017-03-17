#include "gamestate.h"



gamestate::gamestate()
{
	alienMoveTimer = 0;
	alienMoveCounter = 0;
	isGameRunning = false;
	isMovingLeft = false;
}


gamestate::~gamestate()
{
}
