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

	alienShootTimer = 0;
	barricadeTimer = 0;
	alienMoveSpeed = 1.0f;
	alienAnimTimer = 0;
}


gamestate::~gamestate()
{
}
