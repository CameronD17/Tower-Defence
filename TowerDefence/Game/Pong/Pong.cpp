#include "Pong.h"
#include <iostream>
#include <fstream>
#include <sstream>

// *** CONSTRUCTORS AND INITIALISATION METHODS *** //

Pong::Pong(SystemController &e)
{
	engine = e;
}

Pong::~Pong()
{

}
	
void Pong::init()	
{
	engine.admin.start();
	SDL_ShowCursor(0);
	mouse = true;
	
	leftPaddle.setX(40);
	leftPaddle.setY(SCREEN_HEIGHT / 2 - 30);
	rightPaddle.setX(760);
	rightPaddle.setY(SCREEN_HEIGHT / 2 - 30);

	playing = false;   
	speed = 8;
	angle = 0.0f;
	bounce = false;

	score1 = 0;
	score2 = 0;
}


// *** UPDATE METHODS *** //

int Pong::run()
{
	int state = getInput();
	update();
	draw();
	return state;
}

float Pong::getGradient()
{
	return (ball.getY() - ball.getY()+ball.getDY())/(ball.getX() - ball.getX()+ball.getDX());
}

void Pong::updateAngle(float dir)
{
	int relativeAngle;

	if(dir == LEFT)
	{
		relativeAngle = (ball.getY() - leftPaddle.getY() + BALL_SIZE);
		ball.setDX(speed*cos(angle*M_PI/180.0f)); 
	}
	else if(dir == RIGHT)
	{
		relativeAngle = (ball.getY() - rightPaddle.getY() + BALL_SIZE);
		ball.setDX(-speed*cos(angle*M_PI/180.0f));	
	}
	
	angle = (2.14f * relativeAngle - 75.0f);	// Angle formed between ball and paddle after collision
	ball.setDY(speed*sin(angle*M_PI/180.0f));	// convert angle to radian, find its sin() and multiply by the speed
	bounce = false;								// finished bouncing
}

int Pong::predict() 
{
    // Distance between paddles
    int distance = rightPaddle.getX() - (leftPaddle.getX()+PADDLE_WIDTH); 

    // Prediction without taking into consideration wall bounces
    int prediction = abs(getGradient() * -(distance) + ball.getY());
	
	// Count ball bounces and update prediction
    if ((prediction / SCREEN_HEIGHT) % 2 == 0) 
        prediction = prediction % SCREEN_HEIGHT;
    else 
        prediction = SCREEN_HEIGHT - (prediction % SCREEN_HEIGHT);

    return prediction;
}

void Pong::update() 
{
	engine.admin.updateFPS();

	movePaddles();

    if (!playing) return;

	// Collision between ball and left paddle
	 
	// Collision between ball and right paddle
	else if (engine.physics.preciseCollision(&rightPaddle, &ball, ball.getDX())) 
	{
		if (bounce) updateAngle(RIGHT);
		ball.setX(rightPaddle.getX() - BALL_SIZE);		// deposit ball on surface right paddle surface (smooth collision)
		bounce = true;									// bounce ball on next frame

		finalPrediction = predict();					// predict ball position for AI to intercept
	}
	// Collision between ball and upper or lower boundary
	else if ((ball.getY() + ball.getDY() < 0) || (ball.getY() + BALL_SIZE + ball.getDY() >= SCREEN_HEIGHT)) 
	{			
		ball.setDY(ball.getDY() * -1);	// Change the ball direction
	}
	else  // No collision
	{
		// Move the ball in the direction it is heading
		ball.setX(ball.getX() + ball.getDX());
		ball.setY(ball.getY() + ball.getDY());

		// Update the score if necessary
		score();
	}		
}

void Pong::draw() 
{
	engine.graphics.clear();
	
	// Draw game components
	engine.graphics.drawRectangle(leftPaddle.getX(), leftPaddle.getY(), PADDLE_WIDTH, PADDLE_HEIGHT,255,255,255);

    engine.graphics.drawRectangle(rightPaddle.getX(), rightPaddle.getY(), PADDLE_WIDTH, PADDLE_HEIGHT,255,255,255);

    engine.graphics.drawRectangle(ball.getX() - BALL_SIZE / 2, ball.getY(), BALL_SIZE, BALL_SIZE,255,255,255);


	// Draw score and framerate
	stringstream s1;
	s1 << "AI:  " << score1;
    engine.graphics.renderText(10,10,s1.str(), 20);

	stringstream s2;
	s2 << "Player:   " << score2;
    engine.graphics.renderText(10,30,s2.str(), 20);

	stringstream fps;
	fps << "FPS: " << (int)engine.admin.avgFPS;
	engine.graphics.renderText(600, 10, fps.str(), 20);

	engine.graphics.update();
	engine.admin.countedFrames++;		// Capture the framerate
}


// *** GAMEPLAY METHODS *** //

int Pong::getInput() 
{
	SDL_Event e = engine.interfaces.getGeneralInput();	

	if(score1 > 5 || score2 > 5) return -1;			// Quit after a player reaches 5

	if (e.type == SDL_KEYDOWN) 
	{
		SDL_Keycode k = e.key.keysym.sym;
		if (k == SDLK_ESCAPE)
		{
			return -1;
		}
		else if(k == SDLK_SPACE)
		{
			launchBall();
		}
	}
	else if(e.type == SDL_MOUSEMOTION)
	{
		SDL_GetMouseState(&mouseX, &mouseY);
	}	
}

void Pong::movePaddles()
{
	// Move player paddle 
	(mouseY + PADDLE_HEIGHT > SCREEN_HEIGHT ) ? rightPaddle.setY(SCREEN_HEIGHT - PADDLE_HEIGHT) : rightPaddle.setY(mouseY);
	
	// Move AI paddle
    if (ball.getX() < SCREEN_WIDTH*3/5 && ball.getDX() < 0)		 // If the ball is going left, follow the ball
	{ 
        if (leftPaddle.getY() + (PADDLE_HEIGHT - BALL_SIZE)/2 < finalPrediction-2)
            leftPaddle.setY(leftPaddle.getY() + 5);
        else if (leftPaddle.getY() + (PADDLE_HEIGHT - BALL_SIZE)/2 > finalPrediction+2)
            leftPaddle.setY(leftPaddle.getY() - 5);
    }
    else if (ball.getDX() >= 0)	// If the ball is going right, return to the centre
	{		
        if (leftPaddle.getY() + PADDLE_HEIGHT / 2 < SCREEN_HEIGHT/2)
            leftPaddle.setY(leftPaddle.getY() + 2);
        else if (leftPaddle.getY() + PADDLE_HEIGHT / 2 > SCREEN_HEIGHT/2) 
            leftPaddle.setY(leftPaddle.getY() - 2);
    }

    // Keep the AI paddle on the screen
    if (leftPaddle.getY() < 0)      leftPaddle.setY(0);
    else if (leftPaddle.getY() + PADDLE_HEIGHT > SCREEN_HEIGHT)   leftPaddle.setY(SCREEN_HEIGHT - PADDLE_HEIGHT);
}

void Pong::launchBall()
{
	if (!playing) 
	{
		int direction = 1+(-2)*(rand()%2);						// Pick a direction
		angle = rand()%100-40;									// Pick an angle to launch at

		ball.setDX(direction*speed*cos(angle*M_PI/180.0f));		
		ball.setDY(speed*sin(angle*M_PI/180.0f));				

		// Predicting where the left paddle should go if ball is launched left
		finalPrediction = abs(getGradient() * -((SCREEN_WIDTH/2 - (leftPaddle.getX() + PADDLE_WIDTH))) + ball.getY());

		playing = true;
	}
}

void Pong::score()
{
	// If ball goes out...
	if (ball.getX() > SCREEN_WIDTH || ball.getX() < 0) 
	{
		// Increment score
		(ball.getX() > SCREEN_WIDTH) ? score1++ : score2++;
		
		// Reset ball 
		ball.reset();
		playing = false;
	}
}