#include "Game.h"

// *** CONSTRUCTORS AND INITIALISATION METHODS *** //

Game::Game(void){}

Game::Game(Engine &sc)
{
	engine = sc;												// The game relies on an "engine" in the form of the Engine
	cursor.init(engine.resources);								// This initialises the cursor with the engine
	startX = 24, startY = 24, targetX = 96, targetY = 72;
}
	
void Game::newGame(int m, int d)								// Setting up the game
{	
	map.init(0);
	srand(time(NULL));	
	eTimer = SDL_GetTicks() + 300;

	Enemy * e = new Enemy(					// Create a new enemy:	
		engine.resources,						// Resource manager
		startX, startY,							// Starting position of the enemy
		type,									// Enemy "type"
		targetX, targetY,						// Target for the enemy to attack
		100,									// Starting health of the enemy
		100,									// Value (in points) of the enemy
		8);
	e->pathToFollow = engine.astar.findPath(e->getX(), e->getY(), targetX, targetY, map, false);
	enemies.push_back(e);

	newLevel();													// Start a new level
}

void Game::loadMap()
{
	
}



// *** UPDATE METHODS *** //

int Game::run()
{
	int gameState = getInput();	// Get any input from the player. If the player pauses or quits, this is recorded
		
	update();					// Update the players cursor, the game board, and the bots		
	draw();						// Clear the screen and redraw the updates

	return gameState;
}

void Game::update()
{		
	if (SDL_GetTicks() > eTimer)
	{
		for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
		{
			if ((*e)->pathToFollow.size() > 0)
			{
				engine.physics.move((*e), (*e)->pathToFollow.back(), 2);
				(*e)->stepsTaken++;				
			}
			if ((*e)->stepsTaken == 12)
			{
				(*e)->pathToFollow.pop_back();
				(*e)->stepsTaken = 0;
			}
		}
		eTimer = SDL_GetTicks() + 10;
	}

	if (newPath)
	{
		for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
		{
			(*e)->setX((*e)->getX() - (*e)->getX() % BLOCK_SIZE);
			(*e)->setY((*e)->getY() - (*e)->getY() % BLOCK_SIZE);
			(*e)->stepsTaken = 0;

			(*e)->pathToFollow = engine.astar.findPath(
				(*e)->getX(), (*e)->getY(), 
				targetX, targetY, map, false);
		}
		newPath = false;
	}
}

void Game::newLevel()
{
	
}

void Game::cleanup()
{
	
}

void Game::setMessage(string m)
{

}



// *** DRAW METHODS *** //

void Game::draw()
{
	engine.graphics.clear();			// Clear the graphics
	
	drawBoardBackground();				// Draw the background to the game and non-gameplay elements
	
	drawBoardForeground();				// Draw the non-moving foreground elements

	drawGamePieces();					// Draw the entities of the game
		
	drawCursor();						// Draw the player's cursor					
		
	drawDebugFeatures();				// Draw debug features (if required)
			
	engine.graphics.update();			// Render the graphics
	engine.admin.countedFrames++;		// Capture the framerate
}

void Game::drawBoardBackground()
{
	for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x+=BLOCK_SIZE)
	{
		for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
		{
			if (map.getTerrain(x, y) == BLOCKEDTERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 0);
			}
			else if (map.getTerrain(x, y) == WATERTERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 0, 0, 255);
			}
			else if (map.getTerrain(x, y) == ROUGHTERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 0, 255, 0);
			}
			else if (map.getTerrain(x, y) == CLEARTERRAIN)
			{
				//engine.graphics.drawRectangleOL(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
			}
		}
	}
}

void Game::drawGamePieces()
{
	for (std::vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
	{
		//engine.graphics.drawRectangle((*t)->getX(), (*t)->getY(), BLOCK_SIZE*2, BLOCK_SIZE*2, 255, 0, 0);
		engine.graphics.drawRectangleOL((*t)->getX() - 1, (*t)->getY() - 1, (BLOCK_SIZE * 2) + 2, (BLOCK_SIZE * 2) + 2, 255, 255, 255);
	}

	for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		engine.graphics.drawRectangle((*e)->getX(), (*e)->getY(), BLOCK_SIZE, BLOCK_SIZE, 0, 0, 255);
	}	

	engine.graphics.drawRectangleOL(targetX, targetY, BLOCK_SIZE+2, BLOCK_SIZE+2, 255, 255, 255);
}

void Game::drawBoardForeground()
{
	Uint8 red, green, blue;

	red = 154; green = 0; blue = 0; 
	
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
}

void Game::drawCursor()
{	
	// *** Draw the cursor *** //
	if(cursor.getType() == 0)	// If there is no tower selected by the cursor, draw a plain white cursor. This is the default starting cursor.
	{	
		engine.graphics.drawRectangleOL(cursor.getX(), cursor.getY(), BLOCK_SIZE, BLOCK_SIZE, cursor.r, 0, cursor.b);
	}
	else
	{
		engine.graphics.drawRectangle(cursor.getX(), cursor.getY(), BLOCK_SIZE, BLOCK_SIZE, cursor.r, cursor.g, cursor.b);
	}	
}

void Game::drawDebugFeatures()
{
	if (debugMode)
	{
		// Enemy path
		if (!enemies.empty())
		{
			Enemy *e = enemies.front();

			int xp = e->getX() - (e->getX() % BLOCK_SIZE);	int yp = e->getY() - (e->getY() % BLOCK_SIZE);

			for (vector<int>::reverse_iterator i = e->pathToFollow.rbegin(); i != e->pathToFollow.rend(); ++i)
			{
				switch ((*i))
				{
				case UP:
					yp -= BLOCK_SIZE;
					break;

				case UPRIGHT:
					yp -= BLOCK_SIZE;
					xp += BLOCK_SIZE;
					break;

				case RIGHT:
					xp += BLOCK_SIZE;
					break;

				case DOWNRIGHT:
					xp += BLOCK_SIZE;
					yp += BLOCK_SIZE;
					break;

				case DOWN:
					yp += BLOCK_SIZE;
					break;

				case DOWNLEFT:
					yp += BLOCK_SIZE;
					xp -= BLOCK_SIZE;
					break;

				case LEFT:
					xp -= BLOCK_SIZE;
					break;

				case UPLEFT:
					xp -= BLOCK_SIZE;
					yp -= BLOCK_SIZE;
					break;

				default:
					break;
				}

				engine.graphics.drawRectangle(xp + (BLOCK_SIZE / 4), yp + (BLOCK_SIZE / 4), BLOCK_SIZE / 2, BLOCK_SIZE / 2, 0xd3, 0xd3, 0xd3);
			}
		}
		// Cursor Position
		stringstream cursorText;
		int x, y;
		SDL_GetMouseState(&x, &y);
		cursorText << "Cursor: (" << cursor.getX() << "," << cursor.getY() << ")  Actual: (" << x << "," << y << ")";
		engine.graphics.renderText(10, 10, cursorText.str(), 20, 255, 255, 255);


		// Enemy Statistics
		stringstream enemyStats;
		Enemy *e = enemies.back();
		enemyStats << "Path Length: " << e->pathToFollow.size() << " Target: (" << targetX << ", " << targetY << ") Path: (";
		for (std::vector<int>::iterator t = e->pathToFollow.begin(); t != e->pathToFollow.end(); ++t)
		{
			enemyStats << (*t) << ", ";
		}
		enemyStats << ")";
		engine.graphics.renderText(10, 750, enemyStats.str(), 20, 255, 255, 255);
		// Map Statistics
		for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x+=BLOCK_SIZE)
		{
			for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y+=BLOCK_SIZE)
			{
				string terrainType(1, map.getTerrain(x, y));
				engine.graphics.renderText(BORDER + x, BORDER + y, terrainType, 20, 255, 255, 255);
			}
		}
	}
}


// *** GAMEPLAY METHODS *** //

int Game::getInput()
{
	input k = engine.interfaces.getInput();	// Get input from the user

	if (k.keyPress)
	{
		if (k.key == SDLK_ESCAPE)	// Quit the game
		{
			return -1;
		}
		else if (k.key == SDLK_0)
		{
			cursor.changeTowerType(0);
		}
		else if (k.key == SDLK_1)
		{
			cursor.changeTowerType(1);
		}
		else if (k.key == SDLK_2)
		{
			cursor.changeTowerType(2);
		}
		else if (k.key == SDLK_3)
		{
			cursor.changeTowerType(3);
		}
		else if (k.key == SDLK_d)
		{
			debugMode ? debugMode = false : debugMode = true;
		}
		else
		{
			cursor.changeTowerType(1);
		}
	}
	else if (k.mouseDown)
	{
		if (cursor.getType() == 0)
		{
			targetX = cursor.getX(); targetY = cursor.getY();
			newPath = true;
		}
		else
		{
			placeTower();
		}
	}
	else
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE)) && y > BORDER && y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE)))
		{
			//cursor.setType(1);
			cursor.setX((x - (x%BLOCK_SIZE)));
			cursor.setY((y - (y%BLOCK_SIZE)));
		}
		else 
		{
			if (cursor.getType() != 0)
			{
				cursor.setType(0);
				if (x > BORDER && (y > BORDER && y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE))))
				{
					cursor.setX((BOARD_WIDTH*BLOCK_SIZE) - BLOCK_SIZE * 2);
				}
				else if (x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE)) && (y > BORDER && y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE))))
				{
					cursor.setX(BORDER - BLOCK_SIZE);
				}

				if (y > BORDER && (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE))))
				{
					cursor.setY((BOARD_HEIGHT*BLOCK_SIZE) - BLOCK_SIZE * 2);
				}
				else if (y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE))&& (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE))))
				{
					cursor.setY(BORDER - BLOCK_SIZE);
				}
			}
		}
			
	}

    return 0;
}

bool Game::placeTower()
{
	// Only build a tower if:  
	// a) The terrain can be built upon
	// b) The player has selected a tower type to build
	// c) The player has enough credit to build the tower
	// d) The tower is not blocking the spawn point of the enemies

	int xPos = cursor.getX() - BORDER, yPos = cursor.getY() - BORDER;

	if (	clearToBuild(xPos, yPos)
		//&& cursor.getType() != 0										
		//&& credit >= (10 * cursor.getType())								
		//&& !(cursor.getX() + ((1 + cursor.getType())*BLOCK_SIZE) > startX	
		//&&	cursor.getY() - ((cursor.getType())*BLOCK_SIZE) < startY))	
		)
	{
		if (pathAvailable(xPos, yPos))
		{		
			Tower * t = new Tower(cursor.getX(), cursor.getY(), cursor.getType());
			towers.push_back(t);
			credit -= (t->getCost());
			return true;
		}
		
	}
	else if (cursor.getType() != 0)
	{
		/*if (!passable[i])													setMessage("Location impassable!");
		else if (credit < (10 * cursor.getType()))							setMessage("Insufficient funds!");
		else if ((cursor.getX() + ((1 + cursor.getType())*BLOCK_SIZE) > startX
			&& cursor.getY() - ((cursor.getType())*BLOCK_SIZE) < startY))			setMessage("Too close to enemy base!");*/
	}
	return false;
}

bool Game::clearToBuild(int x, int y)
{
	char	s1 = map.getTerrain(x, y), 
			s2 = map.getTerrain(x + BLOCK_SIZE, y),
			s3 = map.getTerrain(x, y + BLOCK_SIZE), 
			s4 = map.getTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE);

	if (	(s1 == CLEARTERRAIN || s1 == ROUGHTERRAIN)
		&&	(s2 == CLEARTERRAIN || s2 == ROUGHTERRAIN)
		&&	(s3 == CLEARTERRAIN || s3 == ROUGHTERRAIN)
		&&	(s4 == CLEARTERRAIN || s4 == ROUGHTERRAIN))
	{
		return true;
	}

	return false;
}

bool Game::pathAvailable(int x, int y)
{
	char terrainReset[4];
	
	terrainReset[0] = map.getTerrain(x, y), terrainReset[1] = map.getTerrain(x + BLOCK_SIZE, y),
	terrainReset[2] = map.getTerrain(x, y + BLOCK_SIZE), terrainReset[3] = map.getTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE);

	map.setTerrain(x, y, BLOCKEDTERRAIN);
	map.setTerrain(x + BLOCK_SIZE, y, BLOCKEDTERRAIN);
	map.setTerrain(x, y + BLOCK_SIZE, BLOCKEDTERRAIN);
	map.setTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE, BLOCKEDTERRAIN);
	
	vector<int> test = engine.astar.findPath(startX, startY, targetX, targetY, map, false);

	if (!test.empty())
	{
		// The map has changed, so the enemies need to find a new path
		for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)	
		{
			(*e)->newPath = true;											
		}
		return true;
	}
	
	// If there is no path available, undo the map changes
	map.setTerrain(x, y, terrainReset[0]);
	map.setTerrain(x + BLOCK_SIZE, y, terrainReset[1]);
	map.setTerrain(x, y + BLOCK_SIZE, terrainReset[2]);
	map.setTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE, terrainReset[3]);

	setMessage("Invalid placement - path blocked");	
	return false;
}