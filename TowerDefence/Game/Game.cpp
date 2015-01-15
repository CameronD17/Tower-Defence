#include "Game.h"

Game::Game()
{
	engine.init();
	cursor.init(engine.resources);
	board.setup(engine);
	sidebar.setup(engine);
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
	drawSidebar();
	drawButtons();
	drawCursor();
	drawDebugFeatures();	
	engine.graphics.update();
	engine.admin.countedFrames++;
}

void Game::drawBoardBackground()
{
	for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x+=BLOCK_SIZE)
	{
		for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
		{
			if (board.map.getTerrain(x, y) == BLOCKED_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 0);
			}
			else if (board.map.getTerrain(x, y) == WATER_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 0, 0, 255);
			}
			else if (board.map.getTerrain(x, y) == ROUGH_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 0, 255, 0);
			}
		}
	}
}

void Game::drawGamePieces()
{
	for (std::vector<Tower*>::iterator t = board.towerHandler.towers.begin(); t != board.towerHandler.towers.end(); ++t)
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

	for (std::vector<Enemy*>::iterator e = board.enemyHandler.enemies.begin(); e != board.enemyHandler.enemies.end(); ++e)
	{
		engine.graphics.drawRectangle((*e)->getX(), (*e)->getY(), BLOCK_SIZE, BLOCK_SIZE, 255, 0, 0);
	}

	engine.graphics.drawRectangleOL(board.map.targetX - 1, board.map.targetY - 1, BLOCK_SIZE + 2, BLOCK_SIZE + 2, 0, 255, 255);
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
		for (std::vector<Tower*>::iterator t = board.towerHandler.towers.begin(); t != board.towerHandler.towers.end(); ++t){ totBullets += (*t)->bullets.size(); }

		cursorText << "Cursor: (" << cursor.getX() << "," << cursor.getY() << ")  Actual: (" << x << "," << y << ")         "
			<< "Enemies: " << board.enemyHandler.enemies.size() << " Towers: " << board.towerHandler.towers.size() << " Bullets: " << totBullets << "   FPS: " << (int)engine.admin.avgFPS;;
		
		engine.graphics.renderText(10, 10, cursorText.str(), 20, 255, 255, 255);

		for (std::vector<Enemy*>::iterator e = board.enemyHandler.enemies.begin(); e != board.enemyHandler.enemies.end(); ++e)
		{
			stringstream idNo;
			idNo << (*e)->getID();
			engine.graphics.renderText((*e)->getX() + 4, (*e)->getY() + 4, idNo.str(), 20);
		}

		for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
		{
			for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
			{
				if (board.map.getTerrain(x, y) == HAS_ENEMY)
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
				if (board.map.getTerrain(x, y) == HAS_ENEMY)
				{
					engine.graphics.drawRectangleOL(x + BORDER, y + BORDER, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
					stringstream ID2;
					ID2 << board.map.getEnemy(x, y);
					engine.graphics.renderText(x + BORDER + 4, y + BORDER + 4, ID2.str(), 10, 255, 255, 0);
				}
			}
		}

		if (board.enemyHandler.enemies.size() > 0)
		{
			for (std::vector<Enemy*>::iterator e = board.enemyHandler.enemies.begin(); e != board.enemyHandler.enemies.end(); ++e)
			{
				int thisX = board.map.targetX + (BLOCK_SIZE / 2);
				int thisY = board.map.targetY + (BLOCK_SIZE / 2);

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
		

		for (std::vector<Tower*>::iterator t = board.towerHandler.towers.begin(); t != board.towerHandler.towers.end(); ++t)
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
	// Static top area
	engine.graphics.drawRectangleOL(SIDEBAR_X + 2, 2, SIDEBAR_WIDTH - 4, STATS_Y - 4, 255, 255, 255);

	// Stats bottom area
	if (board.enemySelected)
	{
		drawSidebarEnemyStats();
		sidebar.findButtonByName("Sell")->visible = false;
		engine.graphics.drawRectangleOL(SIDEBAR_X + 2, STATS_Y + 2, SIDEBAR_WIDTH - 4, STATS_HEIGHT - 4, 255, 0, 0);
	}
	else if (board.towerSelected)
	{
		drawSidebarTowerStats();
		sidebar.findButtonByName("Sell")->visible = true;
		engine.graphics.drawRectangleOL(SIDEBAR_X + 2, STATS_Y + 2, SIDEBAR_WIDTH - 4, STATS_HEIGHT - 4, 255, 255, 255);
	}
	else
	{
		sidebar.findButtonByName("Sell")->visible = false;
		engine.graphics.drawRectangleOL(SIDEBAR_X + 2, STATS_Y + 2, SIDEBAR_WIDTH - 4, STATS_HEIGHT - 4, 0, 0, 255);
	}
}

void Game::drawSidebarEnemyStats()
{
	stringstream IDText, healthText, valueText, bountyText;
	eStats e = board.selectedEnemyStats;
	int enemyStatsStartX = SIDEBAR_X + BLOCK_SIZE;
	int enemyStatsStartY = STATS_Y + BLOCK_SIZE;

	IDText << "Enemy ID: " << e.id;
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY, IDText.str(), 20, 255, 0, 0, "Anonymous_Pro");
	healthText << "Health: " << e.currentHealth;
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + BLOCK_SIZE, healthText.str(), 20, 255, 0, 0, "Anonymous_Pro");
	valueText << "Value: " << e.value << " points.";
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + (BLOCK_SIZE * 2), valueText.str(), 20, 255, 0, 0, "Anonymous_Pro");
	bountyText << "Bounty: " << e.bounty << " credits.";
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + (BLOCK_SIZE * 3), bountyText.str(), 20, 255, 0, 0, "Anonymous_Pro");
}

void Game::drawSidebarTowerStats()
{
	stringstream IDText, damageText, rangeText, killsText;
	tStats t = board.selectedTowerStats;
	int towerStatsStartX = SIDEBAR_X + BLOCK_SIZE;
	int towerStatsStartY = STATS_Y + BLOCK_SIZE;

	IDText << "Tower ID: " << t.id;
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY, IDText.str(), 20, 255, 255, 255, "Anonymous_Pro");
	damageText << "Damage: " << t.damage;
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY + BLOCK_SIZE, damageText.str(), 20, 255, 255, 255, "Anonymous_Pro");
	rangeText << "Range: " << (t.range/BLOCK_SIZE) << " tiles.";
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY + (BLOCK_SIZE * 2), rangeText.str(), 20, 255, 255, 255, "Anonymous_Pro");
	killsText << "Total Kills: " << t.kills;
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY + (BLOCK_SIZE * 3), killsText.str(), 20, 255, 255, 255, "Anonymous_Pro");
}

void Game::drawButtons()
{
	for (std::vector<Button*>::iterator b = sidebar.buttons.begin(); b != sidebar.buttons.end(); ++b)
	{
		if ((*b)->visible)
		{
			if ((*b)->hovered)
			{
				engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 0, 255);
				engine.graphics.renderText((*b)->getX() + 15, (*b)->getY() + 15, (*b)->text, 30, 255, 0, 255);
			}
			else
			{
				engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 255, 255);
				engine.graphics.renderText((*b)->getX() + 15, (*b)->getY() + 15, (*b)->text, 30);
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
	else if (k.x > BORDER && k.x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE)) && k.y > BORDER && k.y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE)))
	{
		handleBoardInput(k);
	}
	else
	{
		handleSidebarInput(k);
	}

	return 0;
}

int Game::handleBoardInput(input k)
{
	if (k.mouseDown)
	{
		if (cursor.getTowerType() == 0)
		{
			board.map.targetX = cursor.getX(); board.map.targetY = cursor.getY();
			board.enemyHandler.updateEnemyTargets(&board.map);
		}
		else if (cursor.getTowerType() == 1)
		{
			if (board.towerHandler.buildTower(cursor, &board.map))
			{
				board.enemyHandler.updateEnemyPaths(cursor.getX(), cursor.getY(), &board.map);
			}
		}
		else if (cursor.getTowerType() == 2)
		{
			board.enemyHandler.launchEnemy(cursor, &board.map);
		}
		else
		{
			board.checkForObject(cursor);
		}
	}
	else
	{
		cursor.setX((k.x - (k.x%BLOCK_SIZE)));
		cursor.setY((k.y - (k.y%BLOCK_SIZE)));
	}

	return 0;
}

int Game::handleSidebarInput(input k)
{
	if (k.mouseDown)
	{
		int button;
		for (std::vector<Button*>::iterator b = sidebar.buttons.begin(); b != sidebar.buttons.end(); ++b)
		{
			if ((*b)->hovered)
			{
				button = (*b)->id;
				break;
			}
		}

		switch (button)
		{
		case 1:		//SELL TOWER
			board.objectSelected = false;
			board.towerSelected = false;
			board.towerHandler.sellTower(&board.map, board.selectedTowerStats.id);
			break;
		default:
			break;
		}		
	}
	else
	{
		cursor.setX(k.x);
		cursor.setY(k.y);

		for (std::vector<Button*>::iterator b = sidebar.buttons.begin(); b != sidebar.buttons.end(); ++b)
		{
			(*b)->cursorOnButton(cursor);
		}
	}

	return 0;
}