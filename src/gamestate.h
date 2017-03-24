#pragma once
class gamestate
{
public:
	gamestate();
	~gamestate();

	int alienMoveTimer;
	int alienShootTimer;
	int barricadeTimer;
	int alienMoveCounter;
	bool isMovingLeft;
	bool isGameRunning;
	bool gameover;
	int playerscore;
	int playerlives;
	int rndShot;
	float alienMoveSpeed;
	int alienAnimTimer;

	int windowHeight, windowWidth;

};

