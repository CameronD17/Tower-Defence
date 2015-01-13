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
	engine.graphics.clear();
	drawBoardBackground();
	drawBoardForeground();
	drawGamePieces();		
	drawCursor();		
	drawSidebar();
	if (board.debugMode)	drawDebugFeatures();
	engine.graphics.update();
	engine.admin.countedFrames++;
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

		if (board.enemies.size() > 0)
		{
			for (std::vector<Enemy*>::iterator e = board.enemies.begin(); e != board.enemies.end(); ++e)
			{
				int thisX = board.targetX + (BLOCK_SIZE / 2);
				int thisY = board.targetY + (BLOCK_SIZE / 2);

				for (unsigned i = 0; i < (*e)->astar.pathToFollow.size(); i++)
				{
					int nextX = ((*e)->astar.xCoordinates.at(i) * BLOCK_SIZE) + (BLOCK_SIZE / 2) + BORDER;
					int nextY = ((*e)->astar.yCoordinates.at(i) * BLOCK_SIZE) + (BLOCK_SIZE / 2) + BORDER;

					engine.graphics.drawLine(thisX, thisY, nextX, nextY);

					thisX = nextX;
					thisY = nextY;
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

void Game::drawSidebar()
{
	// Set constants (move to Constants.h?)
	int startXOfSidebar = (BORDER * 2) + (BOARD_WIDTH * BLOCK_SIZE);
	int widthOfSidebar = WINDOW_WIDTH - startXOfSidebar;

	int startYOfStats = ((BOARD_HEIGHT*BLOCK_SIZE) / 3) * 2;
	int heightOfStats = WINDOW_HEIGHT - startYOfStats;

	// Static top area
	engine.graphics.drawRectangleOL(startXOfSidebar + 2, 2, widthOfSidebar - 4, startYOfStats - 4, 255, 255, 255);

	// Stats bottom area
	if (board.enemySelected)
	{
		stringstream IDText, healthText, valueText, bountyText;
		eStats e = board.pullEnemyStats();
		int enemyStatsStartX = startXOfSidebar + BLOCK_SIZE;
		int enemyStatsStartY = startYOfStats + BLOCK_SIZE;

		IDText << "Enemy ID: " << e.id;
		engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY, IDText.str(), 20, 255, 0, 0, "Anonymous_Pro");
		healthText << "Health: " << e.currentHealth;
		engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + BLOCK_SIZE, healthText.str(), 20, 255, 0, 0, "Anonymous_Pro");
		valueText << "Value: " << e.value << " points.";
		engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + (BLOCK_SIZE * 2), valueText.str(), 20, 255, 0, 0, "Anonymous_Pro");
		bountyText << "Bounty: " << e.bounty << " credits.";
		engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + (BLOCK_SIZE * 3), bountyText.str(), 20, 255, 0, 0, "Anonymous_Pro");

		engine.graphics.drawRectangleOL(startXOfSidebar + 2, startYOfStats + 2, widthOfSidebar - 4, heightOfStats - 4, 255, 0, 0);
	}
	else if (board.towerSelected)
	{
		stringstream IDText, damageText, rangeText, killsText;
		tStats t = board.pullTowerStats();
		int towerStatsStartX = startXOfSidebar + BLOCK_SIZE;
		int towerStatsStartY = startYOfStats + BLOCK_SIZE;

		IDText << "Tower ID: " << t.id;
		engine.graphics.renderText(towerStatsStartX, towerStatsStartY, IDText.str(), 20, 255, 255, 255, "Anonymous_Pro");
		damageText << "Damage: " << t.damage;
		engine.graphics.renderText(towerStatsStartX, towerStatsStartY + BLOCK_SIZE, damageText.str(), 20, 255, 255, 255, "Anonymous_Pro");
		rangeText << "Range: " << t.range << " tiles.";
		engine.graphics.renderText(towerStatsStartX, towerStatsStartY + (BLOCK_SIZE * 2), rangeText.str(), 20, 255, 255, 255, "Anonymous_Pro");
		killsText << "Total Kills: " << t.kills;
		engine.graphics.renderText(towerStatsStartX, towerStatsStartY + (BLOCK_SIZE * 3), killsText.str(), 20, 255, 255, 255, "Anonymous_Pro");

		engine.graphics.drawRectangleOL(startXOfSidebar + 2, startYOfStats + 2, widthOfSidebar - 4, heightOfStats - 4, 255, 255, 255);
	}
	else
	{
		engine.graphics.drawRectangleOL(startXOfSidebar + 2, startYOfStats + 2, widthOfSidebar - 4, heightOfStats - 4, 0, 0, 255);
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
	setCursorPosition(x, y);
	/*	
	 *	Three different types of input need to be handled - mouse movement, mouse clicks, and key presses.
	 *	Key presses will probably be removed in the final version, as the game should be playable using only the mouse.
	*/

	if (k.keyPress)
	{
		switch (k.key)
		{
		case SDLK_ESCAPE:
			if (board.objectSelected)
			{
				board.objectSelected = false;
				board.towerSelected = false;
				board.enemySelected = false;
			}
			else
			{
				return -1;
			}
			break;
		case SDLK_0:
			cursor.changeTowerType(0);
			break;
		case SDLK_1:
			cursor.changeTowerType(1);
			break;
		case SDLK_2:
			cursor.changeTowerType(2);
			break;
		case SDLK_3:
			cursor.changeTowerType(3);
			break;
		case SDLK_4:
			cursor.changeTowerType(4);
			break;
		case SDLK_5:
			cursor.changeTowerType(5);
			break;
		case SDLK_6:
			cursor.changeTowerType(6);
			break;
		case SDLK_d:
			board.debugMode ? board.debugMode = false : board.debugMode = true;
			break;
		default:
			break;
		}
	}
	else if (k.mouseDown)
	{
		if (cursorPosition == CURSORONBOARD)
		{
			return board.getInput(cursor);
		}
		else if (cursorPosition == CURSORONSIDEBAR)
		{
			//return sidebar.getInput();
		}
	}
	else
	{
		if (cursorPosition == CURSORONBOARD)
		{
			//cursor.setType(1);
			cursor.setX((x - (x%BLOCK_SIZE)));
			cursor.setY((y - (y%BLOCK_SIZE)));
		}
		else
		{
			cursor.setX(x);
			cursor.setY(y);
			/*if (cursor.getTowerType() != 0)
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
			}*/
		}
	}

	return 0;
}

void Game::setCursorPosition(int x, int y)
{
	if (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE)) && y > BORDER && y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE)))
	{
		cursorPosition = CURSORONBOARD;
	}
	else
	{
		cursorPosition = CURSORONSIDEBAR;
	}
}