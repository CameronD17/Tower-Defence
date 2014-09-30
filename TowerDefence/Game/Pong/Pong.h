#pragma once
#include "../../Engine/SystemController.h"
#include "../Constants.h"
#include "Ball.h"
#include <cmath>                        
#include <ctime>      

using namespace std;

class Pong 
{
private:
	
	SystemController engine;			// Engine access

	Ball ball;
	Object leftPaddle;
	Object rightPaddle;
	bool mouse;
	int mouseX, mouseY;					// Mouse coordinates	
	bool playing;						// Ball launched?	
	int xDir, yDir;						// Ball movement
	int speed;							// Ball speed 
	float angle;						// angle on collision with paddle
	bool bounce;						// true when next frame renders ball after collision impact (ball has bounced)
	int score1, score2;					// Match score
	int finalPrediction;				// predicted ball position on y-axis after right paddle collision (used for paddle AI)
		
	int predict();	
	float getGradient();
	void updateAngle(float dir);
	void movePaddles();
	void launchBall();
	void score();

public:
    Pong(SystemController &e);
    ~Pong();

	int run();
	void init();
    int getInput();	
	void draw();
	void update();
};
