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
	bool gameover;
	int playerscore;
	int playerlives;


};

