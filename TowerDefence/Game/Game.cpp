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
				engine.graphics.drawRectangle(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 0);
			}
			else if (board.map.getTerrain(x, y) == WATER_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 0, 0, 255);
			}
			else if (board.map.getTerrain(x, y) == ROUGH_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 0, 255, 0);
			}
		}
	}
}

void Game::drawGamePieces()
{
	tStats tS = board.selectedTowerStats;
	for (std::vector<Tower*>::iterator t = board.towerHandler.towers.begin(); t != board.towerHandler.towers.end(); ++t)
	{
		if (board.towerSelected && (*t)->getID() == tS.id)
		{
			engine.graphics.drawRectangleOL((*t)->getX() - 1, (*t)->getY() - 1, BLOCK_SIZE + 2, BLOCK_SIZE + 2, 255, 255, 255);
		}

		if ((*t)->hasEnemy)
		{
			engine.graphics.drawRectangle((*t)->getX(), (*t)->getY(), BLOCK_SIZE, BLOCK_SIZE, 255, 0, 255);
		}
		else
		{
			engine.graphics.drawRectangle((*t)->getX(), (*t)->getY(), BLOCK_SIZE, BLOCK_SIZE, 39, 64, 139);
		}

		for (std::vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end(); ++b)
		{
			engine.graphics.drawRectangle((*b)->getX(), (*b)->getY(), 2,2, 255, 255, 255);
		}
	}

	eStats eS = board.selectedEnemyStats;
	for (std::vector<Enemy*>::iterator e = board.enemyHandler.enemies.begin(); e != board.enemyHandler.enemies.end(); ++e)
	{
		if (board.enemySelected && (*e)->getID() == eS.id)
		{
			engine.graphics.drawRectangleOL((*e)->getX() - 1, (*e)->getY() - 1, BLOCK_SIZE + 2, BLOCK_SIZE + 2, 255, 255, 255);
		}
	
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
	engine.graphics.drawRectangle(0, 0, BORDER_SIZE, ((BORDER_SIZE * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue);
	
	// Right
	engine.graphics.drawRectangle(BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), 0, BORDER_SIZE, ((BORDER_SIZE * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue);

	// Top
    engine.graphics.drawRectangle(0, 0, BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), BORDER_SIZE, red, green, blue);

	// Bottom
    engine.graphics.drawRectangle(0, BORDER_SIZE + (BOARD_HEIGHT*BLOCK_SIZE), BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), BORDER_SIZE, red, green, blue);

	// Outline
	engine.graphics.drawRectangleOL(BORDER_SIZE,BORDER_SIZE,BOARD_WIDTH*BLOCK_SIZE,BOARD_HEIGHT*BLOCK_SIZE,0,0,0);
}

void Game::drawCursor()
{	
	if (cursor.getX() > BORDER_SIZE && cursor.getX() < (BORDER_SIZE + (BOARD_WIDTH*BLOCK_SIZE)) && cursor.getY() > BORDER_SIZE && cursor.getY() < (BORDER_SIZE + (BOARD_HEIGHT*BLOCK_SIZE)))
	{
		engine.graphics.drawRectangleOL(cursor.getX(), cursor.getY(), BLOCK_SIZE, BLOCK_SIZE, cursor.r, cursor.g, cursor.b);
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
					engine.graphics.drawRectangleOL(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
					stringstream ID2;
					ID2 << board.map.getEnemy(x, y);
					engine.graphics.renderText(x + BORDER_SIZE + 4, y + BORDER_SIZE + 4, ID2.str(), 10, 255, 255, 0);
				}
			}
		}

		for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
		{
			for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
			{
				if (board.map.getTerrain(x, y) == HAS_ENEMY)
				{
					engine.graphics.drawRectangleOL(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
					stringstream ID2;
					ID2 << board.map.getEnemy(x, y);
					engine.graphics.renderText(x + BORDER_SIZE + 4, y + BORDER_SIZE + 4, ID2.str(), 10, 255, 255, 0);
				}
			}
		}

		if (board.enemyHandler.enemies.size() > 0)
		{
			for (std::vector<Enemy*>::iterator e = board.enemyHandler.enemies.begin(); e != board.enemyHandler.enemies.end(); ++e)
			{
				int thisX = board.map.targetX + (BLOCK_SIZE / 2);
				int thisY = board.map.targetY + (BLOCK_SIZE / 2);

				for (int i = 0; i < (*e)->getPath().getPathSize(); i++)
				{
					int nextX = ((*e)->getPath().getXAt(i) * BLOCK_SIZE) + (BLOCK_SIZE / 2) + BORDER_SIZE;
					int nextY = ((*e)->getPath().getYAt(i) * BLOCK_SIZE) + (BLOCK_SIZE / 2) + BORDER_SIZE;

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
	//engine.graphics.drawRectangleOL(SIDEBAR_X + 2, 2, SIDEBAR_WIDTH - 4, STATS_Y - 4, 255, 255, 255);

	stringstream creditText;
	creditText << "Credit: " << board.bank.getCredit();
	engine.graphics.renderText(SIDEBAR_X + BLOCK_SIZE, BORDER_SIZE/2, creditText.str(), 40, 255, 255, 255, "bombardier");

	stringstream scoreText;
	scoreText << "Score: " << board.getScore();
	engine.graphics.renderText(SIDEBAR_X + BLOCK_SIZE, 180, scoreText.str(), 40, 255, 255, 255, "bombardier");

	// Stats bottom area
	if (sidebar.buttonSelected)
	{
		sidebar.findButtonByName("Sell")->hide();
		drawSidebarSelectedButton();
		engine.graphics.drawRectangleOL(SIDEBAR_X + 2, STATS_Y + 2, SIDEBAR_WIDTH - 4, STATS_HEIGHT - 4, 0, 255, 0);
	}
	else if (board.enemySelected)
	{
		drawSidebarEnemyStats();
		sidebar.findButtonByName("Sell")->hide();
		engine.graphics.drawRectangleOL(SIDEBAR_X + 2, STATS_Y + 2, SIDEBAR_WIDTH - 4, STATS_HEIGHT - 4, 255, 0, 0);
	}
	else if (board.towerSelected)
	{
		drawSidebarTowerStats();
		sidebar.findButtonByName("Sell")->show();
		engine.graphics.drawRectangleOL(SIDEBAR_X + 2, STATS_Y + 2, SIDEBAR_WIDTH - 4, STATS_HEIGHT - 4, 255, 255, 255);
	}
	else
	{
		sidebar.findButtonByName("Sell")->hide();
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
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY, IDText.str(), 20, 255, 0, 0, "anonymous");
	healthText << "Health: " << e.currentHealth;
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + BLOCK_SIZE, healthText.str(), 20, 255, 0, 0, "anonymous");
	valueText << "Value: " << e.value << " points.";
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + (BLOCK_SIZE * 2), valueText.str(), 20, 255, 0, 0, "anonymous");
	bountyText << "Bounty: " << e.bounty << " credits.";
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + (BLOCK_SIZE * 3), bountyText.str(), 20, 255, 0, 0, "anonymous");
}

void Game::drawSidebarTowerStats()
{
	stringstream IDText, damageText, rangeText, killsText;
	tStats t = board.selectedTowerStats;
	int towerStatsStartX = SIDEBAR_X + BLOCK_SIZE;
	int towerStatsStartY = STATS_Y + BLOCK_SIZE;

	IDText << "Tower ID: " << t.id;
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY, IDText.str(), 20, 255, 255, 255, "anonymous");
	damageText << "Damage: " << t.damage;
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY + BLOCK_SIZE, damageText.str(), 20, 255, 255, 255, "anonymous");
	rangeText << "Range: " << (t.range/BLOCK_SIZE) << " tiles.";
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY + (BLOCK_SIZE * 2), rangeText.str(), 20, 255, 255, 255, "anonymous");
	killsText << "Total Kills: " << t.kills;
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY + (BLOCK_SIZE * 3), killsText.str(), 20, 255, 255, 255, "anonymous");
}

void Game::drawSidebarSelectedButton()
{
	stringstream buttonText;
	int enemyStatsStartX = SIDEBAR_X + BLOCK_SIZE;
	int enemyStatsStartY = STATS_Y + BLOCK_SIZE;

	buttonText << "Button Selected: " << sidebar.selectedButton->text;
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY, buttonText.str(), 20, 255, 0, 0, "anonymous");
}

void Game::drawButtons()
{
	for (std::vector<Button*>::iterator b = sidebar.buttons.begin(); b != sidebar.buttons.end(); ++b)
	{
		if ((*b)->isVisible())
		{
			if ((*b)->isHovered())
			{
				engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 0, 255);
				engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 0, 255);
			}
			else if ((*b)->isSelected())
			{
				engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 255, 0);
				engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 255, 0);
			}
			else
			{
				engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 255, 255);
				engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 255, 255);
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
	
	if (k.x > BORDER_SIZE && k.x < (BORDER_SIZE + (BOARD_WIDTH*BLOCK_SIZE)) && k.y > BORDER_SIZE && k.y < (BORDER_SIZE + (BOARD_HEIGHT*BLOCK_SIZE)))
	{
		return handleBoardInput(k);
	}
	else
	{
		return handleSidebarInput(k);
	}

	return 0;
}

int Game::handleBoardInput(input k)
{
	if (k.mouseDown)
	{
		if (cursor.getAction() == 1)
		{
			if (board.towerHandler.buildTower(&board.map, cursor, &board.bank))
			{
				board.enemyHandler.updateEnemyPaths(cursor.getX(), cursor.getY(), &board.map);
			}
		}
		else if (cursor.getAction() == 2)
		{
			board.enemyHandler.launchEnemy(cursor, &board.map);
		}
		else if (cursor.getAction() == 3)
		{
			board.map.targetX = cursor.getX(); board.map.targetY = cursor.getY();
			board.enemyHandler.updateEnemyTargets(&board.map);
		}
		else
		{
			board.selectObject(cursor);
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
		int button = 0;
		for (std::vector<Button*>::iterator b = sidebar.buttons.begin(); b != sidebar.buttons.end(); ++b)
		{
			if ((*b)->isHovered())
			{
				button = (*b)->id;
				break;
			}
		}

		switch (button)
		{
		case 1:		// SELL TOWER
			board.deselectObject();
			board.towerHandler.sellTower(&board.map, board.selectedTowerStats.id, &board.bank);
			break;

		case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: // BUY TOWER
			board.deselectObject();
			sidebar.selectButton(button);
			cursor.setAction(1);
			break;

		case 12: // CHANGE TARGET
			board.deselectObject();
			sidebar.selectButton(button);
			cursor.setAction(3);
			break;

		case 13: // LAUNCH ENEMY
			board.deselectObject();
			sidebar.selectButton(button);
			cursor.setAction(2);
			break;

		case 14: // TOGGLE DEBUG
			if (board.debugMode)
			{
				sidebar.deselectButton(button);
				board.debugMode = false;
			}
			else
			{
				sidebar.selectButton(button);
				board.debugMode = true;
			}
			break;

		case 15:
			board.deselectObject();
			sidebar.deselectAllButtons();
			cursor.setAction(0);
			break;
		
		case 16:
			return -1;

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