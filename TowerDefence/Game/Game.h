#pragma once

#include "../Engine/Engine.h"
#include "Constants.h"
#include "Cursor.h"
#include "Tower.h"
#include "Enemy.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <fstream>	

using namespace std;

class Game 
{

private:
	// *** VARIABLES *** //

	// Initial values for game setup
	int startHealth, currentHealth;			// Start health of the target, current health of the target
	unsigned int eTimer, bTimer,			// Uints for timing (enemy, bullet)
		mTimer, wTimer, pTimer, sTimer;		// Uints for timing (message, wave, print-wave, slowed enemies)
	int targetX, targetY,					// Coordinates of the enemy's target	
		startX, startY;						// Coordinates of the enemy's base	
	int type;								// Type of enemy to launch
	float eCount;							// Number of enemies to launch

	// Per-game values
	int map, difficulty;					// Player-set map and difficulty
	bool passable[BOARD_WIDTH*BOARD_HEIGHT];// Array that covers the entire board of "tiles", marking whether or not it's passable for enemies.	
	bool ignore[BOARD_WIDTH*BOARD_HEIGHT];	// Array that covers the entire board of "tiles", ignoring any obstacles.						
	int score, credit;						// Current score, remaining credits
	int level;								// Current level the player has reached
	string message;							// Hold a message for the user
	string initials;						// Players initials for the highscore
	int nextType;							// The next enemy to launch
	int enemySpeed;							// The time length in milliseconds between enemy moves

	// Game state trackers
	bool paused;							// Track if the game is paused			
	bool newPath;							// Track if the enemies need to calculate a new path
	bool slowMode;							// Track if the player has enabled 'slow-mode'	

	// DEBUG - TO REMOVE
	bool debugMode;							// Track if debug mode is enabled. Debug features are not part of the game
	bool debugWalkability, debugPath;
	
	// Game pieces
	Cursor cursor;							// Players in-game cursor 
	vector<Tower*>  towers;
	vector<Enemy*>  enemies;

	Map mapm;
	
	// *** METHODS *** //
	
	// Draw methods
	void drawBoardBackground();
	void drawGamePieces();
	void drawBoardForeground();
	void drawCursor();
	void drawDebugFeatures();

	// Gameplay methods
	void loadMap();
	void newLevel();
	bool placeTower();
	bool deleteTower();
	void moveEnemies();
	void moveBullets();
	void cleanup();
	void saveScore();
	void setMessage(string m);

public:	
	Game(void);					// Default Constructor
	Game(Engine &sc);	// Constructor	
	Engine engine;	// Engine access

	bool gameOver;				// Track if the player has lost the game (damn it...)
	
	void newGame(int m, int d);	// Initialise the game to be played		
	int run();
    int getInput();				// Get any input from the player (input can also be quitting or pausing the game)
	void update();				// Update the players cursor, the game board, and the bots		
	void draw();				// Redraw
};
