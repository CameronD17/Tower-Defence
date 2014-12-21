#include "Game.h"

Game::Game()
{
	engine.init();
	cursor.init(engine.resources);
	board.setup(engine);
	//sidebar.setup(engine);
	engine.admin.start();
}

Game::~Game()
{
	close();
}

void Game::close()
{
	engine.close();
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
			if (board.map.getTerrain(x, y) == BLOCKEDTERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 0);
			}
			else if (board.map.getTerrain(x, y) == WATERTERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 0, 0, 255);
			}
			else if (board.map.getTerrain(x, y) == ROUGHTERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 0, 255, 0);
			}
		}
	}
}

void Game::drawGamePieces()
{
	for (std::vector<Tower*>::iterator t = board.towers.begin(); t != board.towers.end(); ++t)
	{
		if ((*t)->hasEnemy)
		{
			engine.graphics.drawRectangle((*t)->getX(), (*t)->getY(), BLOCK_SIZE, BLOCK_SIZE, 255, 0, 255);
			//engine.graphics.drawLine((*t)->getX() + (BLOCK_SIZE / 2), (*t)->getY() + (BLOCK_SIZE / 2), (*t)->enemy->getX() + (BLOCK_SIZE / 2), (*t)->enemy->getY() + (BLOCK_SIZE / 2));
		}
		else
		{
			engine.graphics.drawRectangle((*t)->getX(), (*t)->getY(), BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
			engine.graphics.drawRectangleOL((*t)->getX() - 1, (*t)->getY() - 1, (BLOCK_SIZE) + 2, (BLOCK_SIZE) + 2, 255, 255, 255);
		}

		for (std::vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end(); ++b)
		{
			engine.graphics.drawRectangle((*b)->getX(), (*b)->getY(), 1, 1, 255, 255, 255);
		}
	}

	for (std::vector<Enemy*>::iterator e = board.enemies.begin(); e != board.enemies.end(); ++e)
	{
		engine.graphics.drawRectangle((*e)->getX(), (*e)->getY(), BLOCK_SIZE, BLOCK_SIZE, 255, 0, 0);
	}

	engine.graphics.drawRectangleOL(board.targetX - 1, board.targetY - 1, BLOCK_SIZE + 2, BLOCK_SIZE + 2, 0, 255, 255);
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
	if(cursor.getTowerType() == 0)	// If there is no tower selected by the cursor, draw a plain white cursor. This is the default starting cursor.
	{	
		engine.graphics.drawRectangleOL(cursor.getX(), cursor.getY(), BLOCK_SIZE, BLOCK_SIZE, cursor.r, cursor.g, cursor.b);
	}
	else
	{
		engine.graphics.drawRectangle(cursor.getX(), cursor.getY(), BLOCK_SIZE, BLOCK_SIZE, cursor.r, cursor.g, cursor.b);
	}	
}

void Game::drawDebugFeatures()
{
	if (board.debugMode)
	{
		// Cursor Position
		stringstream cursorText;
		int x, y;
		SDL_GetMouseState(&x, &y);

		int totBullets = 0;
		for (std::vector<Tower*>::iterator t = board.towers.begin(); t != board.towers.end(); ++t){ totBullets += (*t)->bullets.size();}

		cursorText << "Cursor: (" << cursor.getX() << "," << cursor.getY() << ")  Actual: (" << x << "," << y << ")         "
			<< "Enemies: " << board.enemies.size() << " Towers: " << board.towers.size() << " Bullets: " << totBullets << "   FPS: " << (int)engine.admin.avgFPS;;
		
		engine.graphics.renderText(10, 10, cursorText.str(), 20, 255, 255, 255);

		//// Map Statistics
		//for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x+=BLOCK_SIZE)
		//{
		//	for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y+=BLOCK_SIZE)
		//	{
		//		string terrainType(1, board.map.getTerrain(x, y));
		//		engine.graphics.renderText(BORDER + x, BORDER + y, terrainType, 20, 255, 255, 255);
		//	}
		//}

		for (std::vector<Enemy*>::iterator e = board.enemies.begin(); e != board.enemies.end(); ++e)
		{
			stringstream idNo;
			idNo << (*e)->getID();
			engine.graphics.renderText((*e)->getX() + 4, (*e)->getY() + 4, idNo.str(), 20);
		}

		for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
		{
			for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
			{
				if (board.map.getTerrain(x, y) == HASENEMY)
				{
					engine.graphics.drawRectangleOL(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
					stringstream ID2;
					ID2 << board.map.getEnemy(x, y);
					engine.graphics.renderText(x + BORDER + 4, y + BORDER + 4, ID2.str(), 10, 255, 255, 0);
				}
			}
		}

		for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
		{
			for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
			{
				if (board.map.getTerrain(x, y) == HASENEMY)
				{
					engine.graphics.drawRectangleOL(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
					stringstream ID2;
					ID2 << board.map.getEnemy(x, y);
					engine.graphics.renderText(x + BORDER + 4, y + BORDER + 4, ID2.str(), 10, 255, 255, 0);
				}
			}
		}

		for (std::vector<Tower*>::iterator t = board.towers.begin(); t != board.towers.end(); ++t)
		{
			if ((*t)->hasEnemy)
			{
				engine.graphics.drawLine((*t)->getX() + (BLOCK_SIZE / 2), (*t)->getY() + (BLOCK_SIZE / 2), (*t)->enemy->getX() + (BLOCK_SIZE / 2), (*t)->enemy->getY() + (BLOCK_SIZE / 2));
			}
		}
	}
}


// *** UPDATE METHODS *** //

void Game::update()
{
	engine.admin.updateFPS();
	board.update();
}

int Game::getInput()
{
	input k = engine.interfaces.getInput();

	int x, y;
	SDL_GetMouseState(&x, &y);

	if (k.keyPress)
	{
		if (k.key == SDLK_ESCAPE)	// Quit the Board
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
			board.debugMode ? board.debugMode = false : board.debugMode = true;
		}
		else
		{
			cursor.changeTowerType(1);
		}
	}
	else if (k.mouseDown)
	{
		if (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE)) && y > BORDER && y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE)))
		{
			return board.getInput(cursor);
		}
		else if (x > (BORDER + (BOARD_WIDTH*BLOCK_SIZE)))
		{
			//return sidebar.getInput();
		}
	}
	else
	{
		if (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE)) && y > BORDER && y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE)))
		{
			//cursor.setType(1);
			cursor.setX((x - (x%BLOCK_SIZE)));
			cursor.setY((y - (y%BLOCK_SIZE)));
		}
		else
		{
			if (cursor.getTowerType() != 0)
			{
				cursor.changeTowerType(0);
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
				else if (y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE)) && (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE))))
				{
					cursor.setY(BORDER - BLOCK_SIZE);
				}
			}
		}
	}

	return 0;
}