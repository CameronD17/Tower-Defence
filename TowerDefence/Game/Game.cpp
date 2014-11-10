#include "Game.h"

Game::Game(void){}

Game::Game(Engine &e)
{
	engine = e;
	cursor.init(engine.resources);				
	startX = 24, startY = 24, targetX = 384, targetY = 408;
}
	
void Game::newGame()								// Setting up the game
{	
	map.init(0);
	//srand((unsigned int)time(NULL));	
	eTimer = SDL_GetTicks() + 300;
	enemyCount = 1;

	Enemy * e = new Enemy(engine.resources,	startX, startY,	type, targetX, targetY, 1, &map, enemyCount);
	enemies.push_back(e);
}


// *** DRAW METHODS *** //

void Game::draw(float interpolation)
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
			else if (map.getTerrain(x, y) == HASENEMY)
			{
				engine.graphics.drawRectangleOL(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
				stringstream ID2;
				ID2 << map.getEnemy(x, y);
				engine.graphics.renderText(x + BORDER + 4, y + BORDER + 4, ID2.str(), 10, 255, 255, 0);
			}
		}
	}
}

void Game::drawGamePieces()
{
	for (std::vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
	{
		if ((*t)->checkForEnemies(&map))
		{
			engine.graphics.drawRectangle((*t)->getX(), (*t)->getY(), BLOCK_SIZE, BLOCK_SIZE, 255, 0, 255);
			engine.graphics.drawRectangleOL((*t)->getX() - 1, (*t)->getY() - 1, (BLOCK_SIZE) + 2, (BLOCK_SIZE) + 2, 255, 255, 255);
		}
		else
		{
			engine.graphics.drawRectangle((*t)->getX(), (*t)->getY(), BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
			engine.graphics.drawRectangleOL((*t)->getX() - 1, (*t)->getY() - 1, (BLOCK_SIZE) + 2, (BLOCK_SIZE) + 2, 255, 255, 255);
		}
	}

	for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		engine.graphics.drawRectangle((*e)->getX(), (*e)->getY(), BLOCK_SIZE, BLOCK_SIZE, 255, 0, 0);
		engine.graphics.drawRectangleOL((*e)->getX()-1, (*e)->getY()-1, BLOCK_SIZE+2, BLOCK_SIZE+2, 255, 255, 255);
		stringstream idNo;
		idNo << (*e)->getID();
		engine.graphics.renderText((*e)->getX() + 4, (*e)->getY() + 4, idNo.str(), 20);
	}	

	engine.graphics.drawRectangleOL(targetX - 1, targetY - 1, BLOCK_SIZE + 2, BLOCK_SIZE + 2, 0, 255, 255);
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
		/*if (!enemies.empty())
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
		}*/
		// Cursor Position
		stringstream cursorText;
		int x, y;
		SDL_GetMouseState(&x, &y);
		cursorText << "Cursor: (" << cursor.getX() << "," << cursor.getY() << ")  Actual: (" << x << "," << y << ")";
		engine.graphics.renderText(10, 10, cursorText.str(), 20, 255, 255, 255);

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


// *** UPDATE METHODS *** //

void Game::update()
{
	if (SDL_GetTicks() > eTimer)
	{
		for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
		{
			if ((*e)->canWalk(&map))
			{
				engine.physics.move((*e), (*e)->nextMove(), (*e)->getStepSize());
			}
			else
			{
				//(*e)->holdPosition(&map);
			}
		}

		eTimer = SDL_GetTicks() + 10;
	}
}

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
			for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
			{
				(*e)->updateTarget(targetX, targetY);
				(*e)->updatePath(&map);
			}
		}
		else if (cursor.getType() == 3)
		{
			launchEnemy();
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

void Game::launchEnemy()
{
	enemyCount++;
	Enemy * e = new Enemy(engine.resources, cursor.getX(), cursor.getY(), 0, targetX, targetY, 1, &map, enemyCount);
	enemies.push_back(e);
}

void Game::placeTower()
{    
	/* Only build a tower if:  
	 * a) The terrain can be built upon
	 * b) The player has enough credit to build the tower
	 * c) The tower is not blocking the spawn point of the enemies
	 * d) The tower does not completely block a route from the enemy spawn point to the base
	 */
	int xPos = cursor.getX() - BORDER, yPos = cursor.getY() - BORDER;

	if (clearToBuild(xPos, yPos))
	{
		if (pathAvailable(xPos, yPos))
		{
			//if (credit >= (10 * cursor.getType()))
			//{
				Tower * t = new Tower(cursor.getX(), cursor.getY(), cursor.getType());
				towers.push_back(t);
				//credit -= (t->getCost());
			//}
			//else
			//{
			//	setMessage("Insufficient funds available to build.");
			//}
		}
		else
		{
			setMessage("Invalid placement - you cannot block the route to your base.");
		}
	}
	else
	{
		setMessage("Invalid placement - this terrain is obstructed.");
	}
}

bool Game::clearToBuild(int x, int y)
{
	char	s1 = map.getTerrain(x, y);
			/*s2 = map.getTerrain(x + BLOCK_SIZE, y),
			s3 = map.getTerrain(x, y + BLOCK_SIZE), 
			s4 = map.getTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE);*/

	if (	(s1 == CLEARTERRAIN || s1 == ROUGHTERRAIN)/*
		&&	(s2 == CLEARTERRAIN || s2 == ROUGHTERRAIN)
		&&	(s3 == CLEARTERRAIN || s3 == ROUGHTERRAIN)
		&&	(s4 == CLEARTERRAIN || s4 == ROUGHTERRAIN)*/)
	{
		return true;
	}

	return false;
}

bool Game::pathAvailable(int x, int y)
{
	// Check that the new tower wouldn't be blocking the spawn point of enemies
	if ((cursor.getX() + ((1 + cursor.getType())*BLOCK_SIZE) > startX &&	cursor.getY() - ((cursor.getType())*BLOCK_SIZE) < startY))
	{
		return false;
	}
	
	char terrainReset[4];
	bool pathAvailable = true;
	
	terrainReset[0] = map.getTerrain(x, y)/*, terrainReset[1] = map.getTerrain(x + BLOCK_SIZE, y),
	terrainReset[2] = map.getTerrain(x, y + BLOCK_SIZE), terrainReset[3] = map.getTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE)*/;

	map.setTerrain(x, y, BLOCKEDTERRAIN);
	/*map.setTerrain(x + BLOCK_SIZE, y, BLOCKEDTERRAIN);
	map.setTerrain(x, y + BLOCK_SIZE, BLOCKEDTERRAIN);
	map.setTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE, BLOCKEDTERRAIN);*/
	
	/*
	This method may well cause bugs later on in development; it may be possible to place a tower behind the last enemy
	in a wave, blocking off all future waves but none of the existing enemies. MAY REQUIRE A REWRITE (test enemy made and
	destroyed, perhaps?)
	*/

	for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		if (!(*e)->checkPathFromBase(&map))
		{
			pathAvailable = false;

			map.setTerrain(x, y, terrainReset[0]);
			/*map.setTerrain(x + BLOCK_SIZE, y, terrainReset[1]);
			map.setTerrain(x, y + BLOCK_SIZE, terrainReset[2]);
			map.setTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE, terrainReset[3]);*/

			setMessage("Invalid placement - path blocked");
		}		
		(*e)->updatePath(&map);
	}

	return pathAvailable;
}

void Game::cleanup()
{

}

void Game::setMessage(string m)
{

}