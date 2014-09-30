#include "Game.h"

// *** CONSTRUCTORS AND INITIALISATION METHODS *** //

Game::Game(void){}

Game::Game(SystemController &sc)
{
	engine = sc;												// The game relies on an "engine" in the form of the SystemController
	cursor.init(engine.resources);								// This initialises the cursor with the engine

	targetX = BORDER;											// This is the X-Coordinate of the enemy target				
	targetY = (BORDER+(BOARD_HEIGHT-1)*BLOCK_SIZE);				// This is the Y-Coordinate of the enemy target
	startX = (BORDER+(BOARD_WIDTH-1)*BLOCK_SIZE);				// This is the X-Coordinate of the enemy "base"
	startY = BORDER;											// This is the Y-Coordinate of the enemy "base"

	for(int ch = 0; ch < BLOCK_COUNT; ch++) {ignore[ch] = true;}// Initialise the blank passable array
}
	
void Game::newGame(int m, int d)								// Setting up the game
{	
	srand(time(NULL));											// Initialise randomisation

	// Clean up any pre-existing game features from a previous game
	towers.clear();												// Empty the towers vector
	enemies.clear();											// Empty the enemies vector
	cursor.reset();												// Start the cursor at the top. 
	for(int i = 0; i < BLOCK_COUNT; i++) passable[i] = true;	// Clear the impassable blocks
	initials = "";												// Reset the player name
	message = "";												// Reset the information message
	enemySpeed = 500;											// Initial enemy speed

	// Set up a new game
	map = m; difficulty = (d+1);								// Set the user-specified parameters
	engine.admin.start();										// Start the FPS timer 
	level = 0;													// Start off at the first level... obviously
	type = 1;													// The first enemy is always the same
	eCount = 2;													// The first level will launch 2 enemies
	paused = false;												// Start the game unpaused
	gameOver = false;											// Game is playable
	debugMode = false;											// Initialling, debugging should be off. This can be toggled on/off with the TAB key
	debugPath = debugWalkability = false;						// As debug mode is off, no debug paths should be drawn
	slowMode = false;											// Enemies are initially normal speed
	score = 0;													// The player starts with an initial score of zero. The score increases by destroying enemies
	credit = 120;												// The player starts with 120 credits. Credits can be gained by destroying enemies, and are used to build new towers
	startHealth = currentHealth = 100;							// The target has a health bar that depletes if enemies reach it	

	eTimer = SDL_GetTicks() + INITIAL_TICK_LENGTH;				// This timer makes sure that the enemies don't go too fast
	wTimer = pTimer = sTimer = eTimer;							// Set the release timers	
	loadMap();													// Load the map selected
	newLevel();													// Start a new level
}

void Game::loadMap()
{
	// Read in the map data 
	ifstream mapData;

	switch(map)
	{
	case 0: // Custom map	
		mapData.open("Game/Inputs/customMap.txt", ios::in);
		break;
	case 1: // Grassland map
		mapData.open("Game/Inputs/grassMap.txt", ios::in);
		break;
	case 2: // Desert map
		mapData.open("Game/Inputs/desertMap.txt", ios::in);
		break;
	case 3: // Tundra map
		mapData.open("Game/Inputs/tundraMap.txt", ios::in);
		break;
	default:
		break;
	}

    int nextCheck;
	int i = 0;
	
    while(mapData >> nextCheck)
    {
		passable[i] = nextCheck;
		i++;
    }
    mapData.close();
}



// *** UPDATE METHODS *** //

int Game::run()
{
	int gameState = getInput();	// Get any input from the player. If the player pauses or quits, this is recorded
		
	while(gameState == 1)		// If the player pauses the game, stop updates
	{		
		gameState = pause();	// Get input from the player. This only updates if the player unpauses
	}

	update();					// Update the players cursor, the game board, and the bots		
	draw();						// Clear the screen and redraw the updates

	if(gameOver) gameState = -1;

	return gameState;
}

void Game::update()
{	
	if(debugMode)	engine.admin.updateFPS();	// Only check FPS if in debug mode
	newLevel();			// Check if the player has moved up a level (this occurs when all enemies in the level are destroyed)
	moveEnemies();		// Move the enemies on the board, and check if any are within range of any of the towers. If they are, tell the tower to fire at them
	moveBullets();		// Move any bullets that have been fired, and check if any of the bullets have hit their targets	
	cleanup();			// Remove any destroyed enemies or spent bullets, refresh any messages
}

void Game::newLevel()
{
	pTimer = (float)((wTimer-SDL_GetTicks())/1000);	

	if(enemies.empty() || (wTimer < SDL_GetTicks() && enemies.size() < 20))	// Create a new level
	{		
		wTimer = SDL_GetTicks() + WAVE_TICK_LENGTH;	// Reset wave timer
		level++;									// Increment the level	
		eCount += 0.2;								// Increase the number of enemies launching every five levels
		int value	= 150 + (level*difficulty*20);	// Set the value (in points) of the new enemies	
		int health	= (level*difficulty*15);		// Set the health of the new enemies
		setMessage("New Level!");					// Set the message
		nextType = (rand() % 2)+1;					// Pick which enemy to launch on the next wave

		for(int i = 0; i < (int)eCount; i++)	{	// Create however many enemies are required					
			Enemy * e = new Enemy(					// Create a new enemy:	
				engine.resources,						// Resource manager
				startX, startY,							// Starting position of the enemy
				type,									// Enemy "type"
				targetX, targetY,						// Target for the enemy to attack
				health,									// Starting health of the enemy
				value,									// Value (in points) of the enemy
				8);									// Value (in credits) of the enemy
			enemies.push_back(e);					// Add the enemy to the game's list of existing enemies. 
		}	
		type = nextType;							// Update the enemy to be launched on the next wave
	}
}

void Game::moveEnemies()
{
	// *** Move the enemies if possible *** //
	if (SDL_GetTicks() > eTimer)		// Only move if enough time has elapsed since the last move											 
	{	
		for(vector<Enemy*>::iterator i = enemies.begin(); i != enemies.end(); ++i)	// Repeat this for every enemy in the game
		{		
			if ((*i)->leftBase)											// Only move if the enemy has left the base										 
			{	
				if((*i)->newPath)											// Don't update the path unless the field has changed
				{		
					if(((*i)->getType()) != 2)										// Flying enemies (type 2) can ignore obstacles
					{
						(*i)->pathToFollow = engine.physics.aStar(						// Find the new best path for the enemy to take across the map	
						(*i)->getX()/BLOCK_SIZE, (*i)->getY()/BLOCK_SIZE,					// starting at the enemy's current position, towards the
						(*i)->targetX/BLOCK_SIZE, (*i)->targetY/BLOCK_SIZE, passable);	// target position, avoiding impassable areas (e.g. towers, rocks, water)
						(*i)->newPath = false;
					}
					else
					{
						(*i)->pathToFollow = engine.physics.aStar(						// Find the new best path for the enemy to take across the map	
						(*i)->getX()/BLOCK_SIZE, (*i)->getY()/BLOCK_SIZE,					// starting at the enemy's current position, towards the
						(*i)->targetX/BLOCK_SIZE, (*i)->targetY/BLOCK_SIZE, ignore);	// target position, ignoring impassable areas (e.g. towers, rocks, water)
						(*i)->newPath = false;
					}
				}

				if((*i)->pathToFollow.size() >= 1)						// If the enemy has a path to follow (from the A* pathfinder)
				{							
					engine.physics.move((*i), (*i)->pathToFollow.back(), BLOCK_SIZE);		// Move the enemy one space, using the next direction given by the pathfinder

					(*i)->pathToFollow.pop_back();								// Remove the direction that has just been moved from the enemy's planned path
	
					if(((*i)->getX() == targetX) && ((*i)->getY() == targetY))		// If the enemy has reached the target
					{			
						currentHealth -= 10;										// Decrease the target health
						(*i)->setDeleted(true);										// Remove the enemy		
					}
				}
			}
			else														// Maybe release the next enemy
			{
				if((rand() % 3) == 1) (*i)->leftBase = true;				// One in three times, release a new enemy
				break;														// Exit the loop until the next timer cycle		
			}
		}
		if(currentHealth <= 0)	
		{
			gameOver = true;			// The player has lost :(
			debugMode = false;			// Exit debug mode
		}
		eTimer = SDL_GetTicks() + enemySpeed;		// Reset the movement timer	
	}

	if(SDL_GetTicks() > sTimer)
	{
		slowMode = false;
		enemySpeed = 500;
	}


	// *** Fire at the enemies if possible *** //
	for(vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)		// Repeat this for every tower in the game	
	{
		for(vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)	// Repeat this for every enemy in the game
		{
			if (SDL_GetTicks() > (*t)->timer)								// Only perform a type check if the tower hasn't fired recently
			{	
				if(((*t)->antiAir && (*e)->getType() == 2) || ((*t)->antiGround && (*e)->getType() != 2))	// Only perform a distance check if the tower is able to attack
				{
					if(engine.physics.checkDistance((*t), (*e)))				// Check the distance between the tower and the enemy
					{
						(*t)->fire(engine.physics.getDirection((*t), (*e)));	// If the enemy is within range of the tower, fire at the enemy
						(*t)->timer = SDL_GetTicks() + (*t)->reloadRate;		// Reset the reload timer
						if(engine.audio.music)
							engine.audio.playSound("Game/Audio/gun.ogg");		// Play the "fire" sound
					}
				}
			}
		}
	}	
}

void Game::moveBullets()
{
	for(vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)							// Repeat this for every tower in the game
	{
		for(vector<Bullet*>::iterator b = ((*t)->bullets).begin(); b != ((*t)->bullets).end(); ++b)	// Repeat this for every bullet that has fired			
		{
			if((*b)->direction != 0 && engine.physics.checkDistance((*t), (*b)))	// Check the bullet has a direction (i.e. has been fired), and the bullet is still within range of the tower that fired it	
			{	
				if(!engine.physics.move((*b), (*b)->direction, BULLET_SIZE)) (*b)->setDeleted(true);		// Move the bullet along its current trajectory	if possible, otherwise delete it	
			}
			else (*b)->setDeleted(true);

			for(vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)				// Repeat this for every enemy in the game
			{			
				if(engine.physics.impreciseCollision((*e), (*b)))	// Check if there has been a collision between the bullet and the enemy
				{			
					(*b)->setDeleted(true);							// If a collision occurs, delete the bullet
					(*e)->health -= (*t)->damage;					// Lower the enemy health by the tower's damage rating that hit it
					if((*e)->health <= 0)							// Check if the enemy has been destroyed by the hit
					{
						(*t)->levelUp();							// Increase the tower level
						(*e)->setDeleted(true);						// Delete the enemy

						score += (*e)->value;						// Increase the player's score by the value of the enemy destroyed
						credit += (*e)->bounty;						// Increase the player's credit by the bounty on the head of the enemy
						if(engine.audio.music)						// Play the explosion sound
							engine.audio.playSound("Game/Audio/explosion.wav");		
					}
				}
			}
		}
	}
}

void Game::cleanup()
{
	// Clean up any enemies that have been destroyed or reached the base
	for(vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end();) 
	{
		if ((*e)->isDeleted())
		{
			delete *e;
			e = enemies.erase(e);
		}
		else e++;
	}

	// Clean up any bullets that are spent
	for(vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
	{
		for(vector<Bullet*>::iterator b = ((*t)->bullets).begin(); b != ((*t)->bullets).end();) 
		{
			if ((*b)->isDeleted())
			{
				delete *b;
				b = ((*t)->bullets).erase(b);
			}
			else ++b;
		}
	}

	// Clean up any towers that have been sold
	for(vector<Tower*>::iterator t = towers.begin(); t != towers.end();)
	{
		if((*t)->isDeleted())
		{
			(*t)->bullets.clear();
			delete *t;
			t = towers.erase(t);
		}
		else ++t;
	}

	// Refresh the error message
	if(SDL_GetTicks() > mTimer)
	{
		message = "";
	}
}

void Game::setMessage(string m)
{
	message = m;									// Set the message for the user
	mTimer = SDL_GetTicks() + MESSAGE_TICK_LENGTH;	// Set the message timer
}



// *** DRAW METHODS *** //

void Game::draw()
{
	engine.graphics.clear();			// Clear the graphics
	
	drawBoardBackground();				// Draw the background to the game and non-gameplay elements
	
	drawBoardForeground();				// Draw the non-moving foreground elements

	drawGamePieces();					// Draw the entities of the game
		
	drawCursor();						// Draw the player's cursor					
		
	if(debugMode) drawDebugFeatures();	// Draw debug features (if required)
			
	engine.graphics.update();			// Render the graphics
	engine.admin.countedFrames++;		// Capture the framerate
}

void Game::drawBoardBackground()
{
	// *** Draw the speakers *** //

	if(engine.audio.music)	engine.graphics.renderImage(750, 590, "Game/Images/speakerOn.png");
	else					engine.graphics.renderImage(750, 590, "Game/Images/speakerOff.png");

	// *** Draw the map *** //

	switch(map)
	{
	case 0: // Custom map	
		for(int i = 0; i < 360; i++)
		{
			if(passable[i])	engine.graphics.drawRectangle((((int)(i / BOARD_HEIGHT)) * BLOCK_SIZE) + BORDER, (((int)(i % BOARD_HEIGHT)) * BLOCK_SIZE) + BORDER, BLOCK_SIZE, BLOCK_SIZE, 79, 98, 40);
			else			engine.graphics.drawRectangle((((int)(i / BOARD_HEIGHT)) * BLOCK_SIZE) + BORDER, (((int)(i % BOARD_HEIGHT)) * BLOCK_SIZE) + BORDER, BLOCK_SIZE, BLOCK_SIZE, 151, 71, 6);
		}
		break;
	case 1: // Grass map
		engine.graphics.renderImage(BORDER, BORDER, "Game/Images/Maps/grassmap.png");
		break;
	case 2: // Desert map
		engine.graphics.renderImage(BORDER, BORDER, "Game/Images/Maps/desertmap.png");
		break;
	case 3:
		engine.graphics.renderImage(BORDER, BORDER, "Game/Images/Maps/tundramap.png");
		break;
	}

	// *** Draw the game stats *** //
		
	int xPos = BORDER + ((2+BOARD_WIDTH) * BLOCK_SIZE);

	engine.graphics.drawRectangleOL(xPos-BLOCK_SIZE-5, BORDER+(2*BLOCK_SIZE)-10,300,65,255,255,255);

	stringstream s;
	s << "Score: " << score;
	engine.graphics.renderText(xPos-BLOCK_SIZE, BORDER + 2 * BLOCK_SIZE, s.str(), 20);

	stringstream c;
	c << "Credit: " << credit;
	engine.graphics.renderText(xPos-BLOCK_SIZE, BORDER + 3 * BLOCK_SIZE, c.str(), 20);

	engine.graphics.drawRectangleOL(xPos-BLOCK_SIZE-5, BORDER+(5*BLOCK_SIZE)-10,300,65,255,255,255);

	stringstream l;
	l << "Level: " << level;
	engine.graphics.renderText(xPos-BLOCK_SIZE, BORDER + 5 * BLOCK_SIZE, l.str(), 20);

	int health = (int)(((float)currentHealth/(float)startHealth) * 100);
	stringstream h;
	h << "Health: " << health << "%";
	engine.graphics.renderText(xPos-BLOCK_SIZE, BORDER + 6 * BLOCK_SIZE, h.str(), 20);


	// *** Draw the next enemy to launch *** //

	if(pTimer <= 15)
	{
		engine.graphics.renderText(xPos, BORDER + (8 * BLOCK_SIZE), "Next Wave In", 15);
		stringstream time;
		time << pTimer << " seconds";
		engine.graphics.renderText(xPos, BORDER + (8.6 * BLOCK_SIZE), time.str(), 15);
	}
	else
	{
		engine.graphics.renderText(xPos, BORDER + (8 * BLOCK_SIZE), "WAVE QUEUED", 20);
	}

	switch(nextType)
	{
	case 1:
		engine.graphics.renderImage(xPos - BLOCK_SIZE, BORDER + 8 * BLOCK_SIZE, "Game/Images/Sprites/enemyUp.png");
		break;
	case 2:
		engine.graphics.renderImage(xPos - BLOCK_SIZE, BORDER + 8 * BLOCK_SIZE, "Game/Images/Sprites/jetUp.png");
		break;
	default:
		break;
	}

	// *** Draw the samples of the tower type *** // 	

	int xPosStats = BORDER + ((BOARD_WIDTH+1) * BLOCK_SIZE);

	// Draw a sample of the Machine Gun
	engine.graphics.renderImage(xPosStats, BORDER + (11 * BLOCK_SIZE), "Game/Images/Sprites/tower1.png");
	engine.graphics.renderText(xPosStats + BLOCK_SIZE, BORDER + (11 * BLOCK_SIZE), "Damage: 5", 10);
	engine.graphics.renderText(xPosStats + BLOCK_SIZE, BORDER + (11.5 * BLOCK_SIZE), "Range: 1", 10);
	engine.graphics.renderText(xPosStats + BLOCK_SIZE, BORDER + (12 * BLOCK_SIZE), "Price: 10", 10);
	engine.graphics.renderText(xPosStats + (3 * BLOCK_SIZE), BORDER + (11 * BLOCK_SIZE), "Targets: Air/Ground", 10);
	engine.graphics.renderText(xPosStats + (3 * BLOCK_SIZE), BORDER + (11.5 * BLOCK_SIZE), "Reload: 0.4", 10);
	engine.graphics.renderText(xPosStats + (3 * BLOCK_SIZE), BORDER + (12 * BLOCK_SIZE), "Key: 1", 10);

	// Draw a sample of the Cannon
	engine.graphics.renderImage(xPosStats, BORDER + (13 * BLOCK_SIZE), "Game/Images/Sprites/tower2.png");
	engine.graphics.renderText(xPosStats + BLOCK_SIZE, BORDER + (13 * BLOCK_SIZE), "Damage: 10", 10);
	engine.graphics.renderText(xPosStats + BLOCK_SIZE, BORDER + (13.5 * BLOCK_SIZE), "Range: 2", 10);
	engine.graphics.renderText(xPosStats + BLOCK_SIZE, BORDER + (14 * BLOCK_SIZE), "Price: 20", 10);
	engine.graphics.renderText(xPosStats + (3 * BLOCK_SIZE), BORDER + (13 * BLOCK_SIZE), "Targets: Air", 10);
	engine.graphics.renderText(xPosStats + (3 * BLOCK_SIZE), BORDER + (13.5 * BLOCK_SIZE), "Reload: 0.8", 10);
	engine.graphics.renderText(xPosStats + (3 * BLOCK_SIZE), BORDER + (14 * BLOCK_SIZE), "Key: 2", 10);

	// Draw a sample of the Laser
	engine.graphics.renderImage(xPosStats, BORDER + (15 * BLOCK_SIZE), "Game/Images/Sprites/tower3.png");
	engine.graphics.renderText(xPosStats + BLOCK_SIZE, BORDER + (15 * BLOCK_SIZE), "Damage: 15", 10);
	engine.graphics.renderText(xPosStats + BLOCK_SIZE, BORDER + (15.5 * BLOCK_SIZE), "Range: 3", 10);
	engine.graphics.renderText(xPosStats + BLOCK_SIZE, BORDER + (16 * BLOCK_SIZE), "Price: 30", 10);
	engine.graphics.renderText(xPosStats + (3 * BLOCK_SIZE), BORDER + (15 * BLOCK_SIZE), "Targets: Ground", 10);
	engine.graphics.renderText(xPosStats + (3 * BLOCK_SIZE), BORDER + (15.5 * BLOCK_SIZE), "Reload: 1.2", 10);
	engine.graphics.renderText(xPosStats + (3 * BLOCK_SIZE), BORDER + (16 * BLOCK_SIZE), "Key: 3", 10);
}

void Game::drawGamePieces()
{
	// *** Draw the towers and their bullets on the game board *** //
	for(std::vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t) 
	{	
		engine.graphics.renderTexture((*t)->getX(), (*t)->getY(), (*t)->texture);
		if((*t)->level > 0)	engine.graphics.renderText((*t)->getX()+(BLOCK_SIZE*0.7), (*t)->getY(), to_string((long double)(*t)->level), 10);

		for(std::vector<Bullet*>::iterator b = ((*t)->bullets).begin(); b != ((*t)->bullets).end(); ++b)
		{
			engine.graphics.drawRectangle( 
				(*b)->getX(), 
				(*b)->getY(),
				BULLET_SIZE, BULLET_SIZE, 0xFF, 0xFF, 0xFF);
		}
	}

	// *** Draw the enemies on the game board *** //
	for(std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e) 
	{
		if((*e)->health > 0						// Don't draw a destroyed enemy
			&& (*e)->pathToFollow.size() >= 1	// or an enemy that has reached the the player base
			&& (*e)->leftBase)					// or hasn't left their own base
		{
			// This draws the enemy in its current position, facing the correct direction
			switch((*e)->pathToFollow.back())
			{
			case UP: 
				engine.graphics.renderTexture((*e)->getX(), (*e)->getY(), (*e)->sprites.at(0));
				break;
			case DOWN:
				engine.graphics.renderTexture((*e)->getX(), (*e)->getY(), (*e)->sprites.at(1));
				break;
			case LEFT: 
				engine.graphics.renderTexture((*e)->getX(), (*e)->getY(), (*e)->sprites.at(2));
				break;
			case RIGHT:
				engine.graphics.renderTexture((*e)->getX(), (*e)->getY(), (*e)->sprites.at(3));
				break;
			default:
				break;
			}
					
			// This draws the health bar above the enemy (in lime green)
			engine.graphics.drawRectangle((*e)->getX(), (*e)->getY()-1, BLOCK_SIZE * ((float)(*e)->health/(float)(*e)->maxHealth), 2, 0x00, 0xFF, 0x00);
		}
	}

}

void Game::drawBoardForeground()
{
	Uint8 red, green, blue;
	string text = "";

	// *** Draw the start and end points *** //
	engine.graphics.renderImage(startX, startY, "Game/Images/Sprites/enemybase.png");		// Starting point for the enemies (base)
	engine.graphics.renderImage(targetX, targetY, "Game/Images/Sprites/playerbase.png");	// End point for the enemies (target)		
	engine.graphics.drawRectangle(targetX, targetY-2, BLOCK_SIZE * ((float)currentHealth/(float)startHealth), 2, 0x00, 0xFF, 0x00);			// Target health bar

	// *** Draw the map border *** //
	switch(difficulty)
	{
	case 1:		// Easy (green)
		red = 0; green = 153; blue = 0; text = "Easy";		
		break;
	case 2:		// Medium (orange)
		red = 204; green = 102; blue = 0; text = "Medium";
		break;
	case 3:		// Hard (red)
		red = 255; green = 51; blue = 51; text = "Hard";
		break;
	case 4:		// Expert (dark red)
		red = 154; green = 0; blue = 0; text = "Expert";
		break;
	default:
		red = 70; green = 80; blue = 90;
		break;
	}

	// *** Draw the board sides *** // 
	// Left
	engine.graphics.drawRectangle(0, 0, BORDER, ((BORDER * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue);
	
	// Right
	engine.graphics.drawRectangle(BORDER + (BOARD_WIDTH * BLOCK_SIZE), 0, BORDER, ((BORDER * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue);

	// Top
    engine.graphics.drawRectangle(0, 0, BORDER + (BOARD_WIDTH * BLOCK_SIZE), BORDER, red, green, blue);

	// Bottom
    engine.graphics.drawRectangle(0, BORDER + (BOARD_HEIGHT*BLOCK_SIZE), BORDER + (BOARD_WIDTH * BLOCK_SIZE), BORDER, red, green, blue);

	// Outline
	engine.graphics.drawRectangleOL(BORDER,BORDER,BOARD_WIDTH*BLOCK_SIZE,BOARD_HEIGHT*BLOCK_SIZE,0,0,0);

	// *** Draw the difficulty *** //
	engine.graphics.renderText(625, 590, text, 20);

	// *** Draw a logo in the corner *** //
	engine.graphics.renderImage(585, 20, "Game/Images/logoSmall.png"); 

	// *** Draw any error messages for the user *** //
	if(message != "")	engine.graphics.renderText(20, 3, message, 20);
	if(slowMode)		engine.graphics.renderText(250, 3, "Slow", 20);
}

void Game::drawCursor()
{	
	// *** Draw the cursor *** //
	if(cursor.getType() == 0)	// If there is no tower selected by the cursor, draw a plain white cursor. This is the default starting cursor.
	{	
		engine.graphics.drawRectangleOL((BORDER+cursor.getX()), (BORDER+cursor.getY()), BLOCK_SIZE, BLOCK_SIZE, cursor.r, cursor.g, cursor.b);
	}
	else	// If there is a tower selected by the cursor, draw the cursor in the tower's texture
	{
		if(!passable[((int)(cursor.getX()/BLOCK_SIZE) * BOARD_HEIGHT) + (int)cursor.getY()/BLOCK_SIZE]
		|| (cursor.getX()+((1+cursor.getType())*BLOCK_SIZE) > startX && cursor.getY()-((cursor.getType())*BLOCK_SIZE) <= startY))
		{
			// If there is a tower already in the cursors position, change the cursor red to indicate to the player that no tower can be placed here
			engine.graphics.drawRectangle((BORDER+cursor.getX()), (BORDER+cursor.getY()), BLOCK_SIZE, BLOCK_SIZE, 0xFF, 0x00, 0x00);

			// Also draw a larger rectangle (in red too) around the cursor to indicate the radius of the tower type selected
			engine.graphics.drawRectangleOL(
				(BORDER+cursor.getX()-(BLOCK_SIZE * cursor.getType())), 
				(BORDER+cursor.getY()-(BLOCK_SIZE * cursor.getType())), 
				(BLOCK_SIZE * cursor.getType() * 2) + BLOCK_SIZE, 
				(BLOCK_SIZE * cursor.getType() * 2) + BLOCK_SIZE, 
				0xFF, 0x00, 0x00);
		}
		else
		{
			// If the player has selected a tower type, draw the cursor in the tower's colour
			if(cursor.texture != NULL) engine.graphics.renderTexture((BORDER + cursor.getX()),(BORDER+cursor.getY()),cursor.texture);
			else engine.graphics.drawRectangle((BORDER+ cursor.getX()), (BORDER+cursor.getY()), BLOCK_SIZE, BLOCK_SIZE, cursor.r, cursor.g, cursor.b);
		
			// Also draw a larger rectangle around the cursor to indicate the radius of the tower type selected
			engine.graphics.drawRectangleOL(
				(BORDER+cursor.getX()-(BLOCK_SIZE * cursor.getType())), 
				(BORDER+cursor.getY()-(BLOCK_SIZE * cursor.getType())), 
				(BLOCK_SIZE * cursor.getType() * 2) + BLOCK_SIZE, 
				(BLOCK_SIZE * cursor.getType() * 2) + BLOCK_SIZE, 
				cursor.r, cursor.g, cursor.b);
		}	
	}
	// *** Draw the paused overlay if the player has paused the game *** //
	if(paused)	engine.graphics.renderText(100, 300, "PAUSED - Press 'P' to continue", 30);
	
	// *** Draw the game-over overlay if the player has lost the game *** //
	if(gameOver)
	{
		engine.graphics.drawRectangle(100, 100, 600, 400, 20, 20, 20);					// Draw box over gameboard
		engine.graphics.renderText(250, 110, "GAME OVER", 50, 220, 20, 60);				// Draw "GAME OVER" text
		engine.graphics.renderText(150, 160, "Please enter your initials", 30);			// Draw instructions

		if(initials.length() != 0)	engine.graphics.renderText(350, 200, initials, 30, 34, 139, 34);				// Draw initials if they've been entered
		if(initials.length() != 3) engine.graphics.drawRectangle(350+(initials.length()*25),225,25,2,34,139,34);	// Draw cursor if the player can add more initials
	
		engine.graphics.renderText(220, 250, "Hit 'Enter' to submit", 30);				// Draw instructions

		stringstream s;
		s << "Final Score: " << score;
		engine.graphics.renderText(250, 400, s.str(), 40);								// Draw the final score
		engine.graphics.renderImage(120, 350, "Game/Images/nuclear-explosion.png");
	}
}

void Game::drawDebugFeatures()
{
	engine.graphics.renderText(400, 3, "DEBUG MODE ON", 20);

	// *** Draw the path for the foremost enemy *** //
	if(debugPath)
	{
		if(!enemies.empty())
		{
			Enemy *e = enemies.front();

			int xP = e->getX();	int yP = e->getY();

			for(vector<int>::reverse_iterator i = e->pathToFollow.rbegin(); i != e->pathToFollow.rend(); ++i)
			{
				switch((*i))
				{
				case UP: 
					yP -= BLOCK_SIZE;			
					break;

				case DOWN:
					yP += BLOCK_SIZE;
					break;

				case LEFT: 
					xP -= BLOCK_SIZE;
					break;

				case RIGHT: 
					xP += BLOCK_SIZE;
					break;
				default:
					break;
				}

				engine.graphics.drawRectangle(xP + (BLOCK_SIZE / 4), yP + (BLOCK_SIZE / 4), BLOCK_SIZE/2, BLOCK_SIZE/2, 0xD3, 0xD3, 0xD3);
			}
		}
	}
	else
	{
		int xP = startX;	int yP = startY;
		vector<int> path = engine.physics.aStar(xP/BLOCK_SIZE, yP/BLOCK_SIZE, targetX/BLOCK_SIZE, targetY/BLOCK_SIZE, passable);		

		for(vector<int>::reverse_iterator i = path.rbegin(); i != path.rend(); ++i)
		{
			switch((*i))
			{
			case UP: 
				yP -= BLOCK_SIZE;			
				break;

			case DOWN:
				yP += BLOCK_SIZE;
				break;

			case LEFT: 
				xP -= BLOCK_SIZE;
				break;

			case RIGHT: 
				xP += BLOCK_SIZE;
				break;
			default:
				break;
			}

			engine.graphics.drawRectangle(xP + (BLOCK_SIZE / 4), yP + (BLOCK_SIZE / 4), BLOCK_SIZE/2, BLOCK_SIZE/2, 0xD3, 0xD3, 0xD3);
		}
	}

	// *** Draw the statistics for the current game
	// Total towers
	int ts = towers.size();
	stringstream t;
	t << "Towers: " << ts;
	engine.graphics.renderText(BORDER, BORDER + (BOARD_HEIGHT * BLOCK_SIZE) + 4,  t.str(), 15);

	// Total enemies
	int es = enemies.size();
	stringstream e;
	e << "Enemies: " << es;
	engine.graphics.renderText(BORDER + (4*BLOCK_SIZE), BORDER + (BOARD_HEIGHT * BLOCK_SIZE) + 4,  e.str(), 15);

	// Total bullets
	int bs = 0;
	for(vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t) bs += (*t)->bullets.size();
	stringstream b;
	b << "Bullets: " << bs;
	engine.graphics.renderText(BORDER + (8*BLOCK_SIZE), BORDER + (BOARD_HEIGHT * BLOCK_SIZE) + 4,  b.str(), 15);

	// Framerate
	stringstream f;
	f << "FPS: " << (int)engine.admin.avgFPS;
	engine.graphics.renderText(BORDER + (12*BLOCK_SIZE), BORDER + (BOARD_HEIGHT * BLOCK_SIZE) + 4,  f.str(), 15);


	// *** Draw the passability of each block on the game board *** //
	if(debugWalkability)
	{
		for(int i = 0; i < 360; i++)
		{
			if(passable[i])
			{
				engine.graphics.drawRectangle((((int)(i / BOARD_HEIGHT)) * BLOCK_SIZE) + BORDER + (BLOCK_SIZE/2), (((int)(i % BOARD_HEIGHT)) * BLOCK_SIZE) + BORDER + (BLOCK_SIZE/2), BLOCK_SIZE/4, BLOCK_SIZE/4,173,255,47);
			}
			else
			{
				engine.graphics.drawRectangle((((int)(i / BOARD_HEIGHT)) * BLOCK_SIZE) + BORDER + (BLOCK_SIZE/2), (((int)(i % BOARD_HEIGHT)) * BLOCK_SIZE) + BORDER + (BLOCK_SIZE/2), BLOCK_SIZE/4, BLOCK_SIZE/4,139,0,0);
			}
		}
	}
}



// *** GAMEPLAY METHODS *** //

int Game::getInput()
{
	SDL_Keycode k = engine.interfaces.getInput();	// Get input from the user

	if(k == SDLK_m)			// Pause the music
	{
		engine.audio.pauseMusic();
	}
	else if (k == SDLK_ESCAPE)	// Quit the game
	{
		return -1;		
	}
	else				// If the player is in their game
	{
		if (k == SDLK_p)	// Pause the game
		{
			return 1;		
		}
		else					// Handle gameplay
		{
			switch (k)
			{
			case SDLK_UP: 							// Move cursor up
				engine.physics.move(&cursor, UP, BLOCK_SIZE);
				break;

			case SDLK_DOWN:							// Move cursor down
				engine.physics.move(&cursor, DOWN, BLOCK_SIZE);
				break;

			case SDLK_LEFT:							// Move cursor left
				engine.physics.move(&cursor, LEFT, BLOCK_SIZE);
				break;

			case SDLK_RIGHT:						// Move cursor right
				engine.physics.move(&cursor, RIGHT, BLOCK_SIZE);
				break;

			case SDLK_0:							// Clear tower type
				cursor.changeTowerType(0);
				break;

			case SDLK_1:							// Change tower type to 1
				cursor.changeTowerType(1);
				break;

			case SDLK_2:							// Change tower type to 2
				cursor.changeTowerType(2);
				break;

			case SDLK_3:							// Change tower type to 3
				cursor.changeTowerType(3);
				break;			

			case SDLK_SPACE:						// Place a tower
				if(!placeTower()) 
				{
					if(!deleteTower())
					{
						engine.audio.playSound("Game/Audio/error.wav");
					}
				}
				break;
			
			case SDLK_s:
				if(credit >= 50 && !slowMode)
				{
					slowMode = true;
					sTimer = SDL_GetTicks() + SLOW_TIMER;
					credit -= 50;
					enemySpeed = 1200;
				}

			case SDLK_DELETE:
				break;

			case SDLK_TAB:								// Toggle debug mode
				debugMode ? debugMode = false : debugMode = true;
				break;
		
			// The following cases can only be triggered when in debug mode
		
			case SDLK_o:		// Trigger an immediate gameover
				if(debugMode)	currentHealth = 0;
				break;

			case SDLK_b:		// View walkability
				if(debugMode) debugWalkability ? debugWalkability = false : debugWalkability = true;
				break;

			case SDLK_r:		// Show A* path
				if(debugMode) debugPath ? debugPath = false : debugPath = true;
				break;

			case SDLK_c:		// Free Cash!
				if(debugMode) credit += 50;
				break;

			default:
				break;
			}   

		} 
	}
    return 0;
}

bool Game::placeTower()
{
	int i = ((cursor.getX()/BLOCK_SIZE) * BOARD_HEIGHT) + cursor.getY()/BLOCK_SIZE;				// This is to find the right location in the "passable" array
	
	if(																// Only build a tower if: 
		passable[i]													// a) There isn't already a tower in the cursors current position
	&&	cursor.getType() != 0										// b) A tower type has been selected by the player, and
	&&	credit >= (10 * cursor.getType())							// c) The player has enough credit to build the tower	
	&&	!(cursor.getX()+((1+cursor.getType())*BLOCK_SIZE) > startX	// d) The tower won't be able to fire at
	&&	cursor.getY()-((cursor.getType())*BLOCK_SIZE) < startY))	//	  the base of the enemies	
	{
		passable[i] = false; 			// Set the value of the current location to impassable
		vector<int> test = engine.physics.aStar(startX/BLOCK_SIZE, startY/BLOCK_SIZE, targetX/BLOCK_SIZE, targetY/BLOCK_SIZE, passable);

		if(!test.empty())				// Is there is still a valid path available from the start position?	
		{
			for(vector<Enemy*>::iterator f = enemies.begin(); f != enemies.end(); ++f)	{
				(*f)->newPath = true;											// Tell the enemies they need to find a new path
			}
			Tower * t = new Tower(cursor.getX() + BORDER,	cursor.getY() + BORDER,	// Create a new tower in the correct location...								
				cursor.getType(), cursor.texture);									// ...with the correct type and texture.

			t->timer = SDL_GetTicks() + BULLET_TICK_LENGTH;						// Set the timer for the tower
			towers.push_back(t);												// Add the tower to the game's list of existing towers. 

			credit	-= (10 * cursor.getType());										// Remove the appropriate amount of credit from the player's cash supply
			if(engine.audio.music)
				engine.audio.playSound("Game/Audio/placeTower.ogg");			// Play the "building tower" sound
			return true;
		}
		else
		{
			passable[i] = true;								// Reset the value as passable if it's the only possible path
			setMessage("Invalid placement - path blocked");	// Set the error message for the user
			return false;
		}		
	}
	else if(cursor.getType() != 0)
	{
		if(!passable[i])													setMessage("Location impassable!");
		else if(credit < (10 * cursor.getType()))							setMessage("Insufficient funds!");
		else if((cursor.getX()+((1+cursor.getType())*BLOCK_SIZE) > startX 
			&& cursor.getY()-((cursor.getType())*BLOCK_SIZE) < startY))			setMessage("Too close to enemy base!");
	}
	return false;
}

bool Game::deleteTower()
{
	int i = ((cursor.getX()/BLOCK_SIZE) * BOARD_HEIGHT) + cursor.getY()/BLOCK_SIZE;				// This is to find the right location in the "passable" array

	if(cursor.getType() == 0)	// Only delete a tower if no other tower has been selected
	{
		if(!passable[i])		// Just check the towers if it's possible for one to be there
		{
			for(vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
			{
				if((cursor.getX() + BORDER) == (*t)->getX() && (cursor.getY() + BORDER) == (*t)->getY())
				{
					(*t)->setDeleted(true);			// Remove the tower
					passable[i] = true;				// Reset the value as passable					
					credit += (5 * (*t)->getType());// Reimburse the player half the towers value
					setMessage("Tower Sold!");		// Set the success message for the user
					return true;
				}
			}
		}
		setMessage("No tower selected!");
	}	
	return false;
}

int Game::pause()
{
	paused = true;
	SDL_Keycode k = engine.interfaces.getInput();
	if (k == SDLK_p) 
	{
		paused = false;
		return 0;
	}	
	draw();		// Clear the screen and redraw
	return 1;
}



// *** GAME OVER METHODS *** //

int Game::over()
{
	SDL_Keycode k = engine.interfaces.getInput();

	if (k == SDLK_RETURN && initials.length() != 0) 
	{
		saveScore();
		return 0;
	}
	else if(k == SDLK_BACKSPACE)
	{
		if(initials.length() != 0)
		{
			initials.pop_back();
		}
	}
	else if(initials.length() < 3)	// Maximum number of initials
	{
		switch(k)
		{
		case SDLK_a:
			initials += 'A';
			break;
		case SDLK_b:
			initials += 'B';
			break;
		case SDLK_c:
			initials += 'C';
			break;
		case SDLK_d:
			initials += 'D';
			break;
		case SDLK_e:
			initials += 'E';
			break;
		case SDLK_f:
			initials += 'F';
			break;
		case SDLK_g:
			initials += 'G';
			break;
		case SDLK_h:
			initials += 'H';
			break;
		case SDLK_i:
			initials += 'I';
			break;
		case SDLK_j:
			initials += 'J';
			break;
		case SDLK_k:
			initials += 'K';
			break;
		case SDLK_l:
			initials += 'L';
			break;
		case SDLK_m:
			initials += 'M';
			break;
		case SDLK_n:
			initials += 'N';
			break;
		case SDLK_o:
			initials += 'O';
			break;
		case SDLK_p:
			initials += 'P';
			break;
		case SDLK_q:
			initials += 'Q';
			break;
		case SDLK_r:
			initials += 'R';
			break;
		case SDLK_s:
			initials += 'S';
			break;
		case SDLK_t:
			initials += 'T';
			break;
		case SDLK_u:
			initials += 'U';
			break;
		case SDLK_v:
			initials += 'V';
			break;
		case SDLK_w:
			initials += 'W';
			break;
		case SDLK_x:
			initials += 'X';
			break;
		case SDLK_y:
			initials += 'Y';
			break;
		case SDLK_z:
			initials += 'Z';
			break;
		default:
			break;
		}
	}
	return 1;
}

void Game::saveScore()
{
	vector<string> allScores;									// Hold the scores in the highscores table 
	ifstream currentScores("Game/highscores.txt", ios::in);		// Current highscores
    string initText;
    int scoreInt, i = 0;
	bool printed = false;
	
	// Current score & initials
	stringstream ss;
	ss << initials << " " << score << "\n";
	string thisScore = ss.str();


	// Read in the highscores, inserting the current score where appropriate
    while((currentScores >> initText >> scoreInt) && i < 7)
    {
		i++;
        if(scoreInt <= score && !printed)
		{
			i++;
			allScores.push_back(thisScore);			
			printed = true;
		}
		stringstream s;
		s << initText << " " << scoreInt << "\n";
		allScores.push_back(s.str());
    }
	currentScores.close();

	// If there are less highscores than the maximum, add the score to the bottom
	if(!printed && i < 6)
	{
		allScores.push_back(thisScore);
	}
    
	// Write the new highscores table to a file
	ofstream scores("Game/highscores.txt", ios::out);
	if(scores.is_open())
	{
		for(vector<string>::iterator it=allScores.begin(); it!=allScores.end(); it++)
		{
			scores << *it;
		}
		scores.close();
	}
}