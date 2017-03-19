#pragma once
class gamestate
{
public:
	gamestate();
	~gamestate();

	int alienMoveTimer;
	int alienMoveCounter;
	bool isMovingLeft;
	bool isGameRunning;
	int playerscore;


};

