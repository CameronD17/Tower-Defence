#include "GameScreen.h"

GameScreen::GameScreen()
{
	
}

GameScreen::~GameScreen()
{
	close();
}

void GameScreen::init(Engine &e, Cursor &c)
{
	engine = e;
	cursor = c;
	board.setup(engine);
	sidebar.setup(engine);
	pauseMenu.setup(engine, cursor);
	engine.admin.start();
	transition.init(engine);
}

void GameScreen::close()
{

}

void GameScreen::draw()
{
	engine.graphics.clear();

	drawBoardMap();
	drawBoardPieces();
					
	drawSidebar();
	drawSidebarButtons();

	drawCursor();
	drawDebugFeatures();	

	drawPauseMenu();

	engine.graphics.update();
	engine.admin.countedFrames++;
}

void GameScreen::drawBoardMap()
{
	// TODO: update this method with actual imagery
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

	Uint8 red = 154, green = 0, blue = 0;
	// Left
	engine.graphics.drawRectangle(0, 0, BORDER_SIZE, ((BORDER_SIZE * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue);

	// Right
	engine.graphics.drawRectangle(BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), 0, BORDER_SIZE, ((BORDER_SIZE * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue);

	// Top
	engine.graphics.drawRectangle(0, 0, BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), BORDER_SIZE, red, green, blue);

	// Bottom
	engine.graphics.drawRectangle(0, BORDER_SIZE + (BOARD_HEIGHT*BLOCK_SIZE), BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), BORDER_SIZE, red, green, blue);

	// Outline
	engine.graphics.drawRectangleOL(BORDER_SIZE, BORDER_SIZE, BOARD_WIDTH*BLOCK_SIZE, BOARD_HEIGHT*BLOCK_SIZE, 0, 0, 0);
}

void GameScreen::drawBoardPieces()
{
	tStats tS = board.selectedTowerStats;
	for (vector<Tower*>::iterator t = board.towerHandler.towers.begin(); t != board.towerHandler.towers.end(); ++t)
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

		for (vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end(); ++b)
		{
			engine.graphics.drawRectangle((*b)->getX(), (*b)->getY(), 2,2, 255, 255, 255);
		}
	}

	eStats eS = board.selectedEnemyStats;
	for (vector<Enemy*>::iterator e = board.enemyHandler.enemies.begin(); e != board.enemyHandler.enemies.end(); ++e)
	{
		if (board.enemySelected && (*e)->getID() == eS.id)
		{
			engine.graphics.drawRectangleOL((*e)->getX() - 1, (*e)->getY() - 1, BLOCK_SIZE + 2, BLOCK_SIZE + 2, 255, 255, 255);
		}
	
		engine.graphics.drawRectangle((*e)->getX(), (*e)->getY(), BLOCK_SIZE, BLOCK_SIZE, 255, 0, 0);
		
	}

	engine.graphics.drawRectangleOL(board.map.targetX - 1, board.map.targetY - 1, BLOCK_SIZE + 2, BLOCK_SIZE + 2, 0, 255, 255);
}

void GameScreen::drawCursor()
{	
	if (cursor.getX() >= BORDER_SIZE && cursor.getX() < (BORDER_SIZE + (BOARD_WIDTH*BLOCK_SIZE)) && cursor.getY() >= BORDER_SIZE && cursor.getY() < (BORDER_SIZE + (BOARD_HEIGHT*BLOCK_SIZE)))
	{
		engine.graphics.drawRectangleOL(cursor.getX(), cursor.getY(), BLOCK_SIZE, BLOCK_SIZE, cursor.r, cursor.g, cursor.b);
	}
}

void GameScreen::drawDebugFeatures()
{
	if (board.debugMode)
	{
		// Cursor Position
		stringstream cursorText;
		int x, y;
		SDL_GetMouseState(&x, &y);

		int totBullets = 0;
		for (vector<Tower*>::iterator t = board.towerHandler.towers.begin(); t != board.towerHandler.towers.end(); ++t){ totBullets += (*t)->bullets.size(); }

		cursorText << "Cursor: (" << cursor.getX() << "," << cursor.getY() << ")  Actual: (" << x << "," << y << ")         "
			<< "Enemies: " << board.enemyHandler.enemies.size() << " Towers: " << board.towerHandler.towers.size() << " Bullets: " << totBullets << "   FPS: " << (int)engine.admin.avgFPS;;
		
		engine.graphics.renderText(10, 10, cursorText.str(), SMALL, 255, 255, 255);

		for (vector<Enemy*>::iterator e = board.enemyHandler.enemies.begin(); e != board.enemyHandler.enemies.end(); ++e)
		{
			stringstream idNo;
			idNo << (*e)->getID();
			engine.graphics.renderText((*e)->getX() + 4, (*e)->getY() + 4, idNo.str(), SMALL);
		}

		for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
		{
			for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
			{
				if (board.map.hasEnemy(x, y))
				{
					engine.graphics.drawRectangleOL(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
					stringstream ID2;
					ID2 << board.map.getEnemy(x, y);
					engine.graphics.renderText(x + BORDER_SIZE + 4, y + BORDER_SIZE + 4, ID2.str(), EXTRA_SMALL, 255, 255, 0);
				}
			}
		}

		for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
		{
			for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
			{
				if (board.map.hasEnemy(x, y))
				{
					engine.graphics.drawRectangleOL(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
					stringstream ID2;
					ID2 << board.map.getEnemy(x, y);
					engine.graphics.renderText(x + BORDER_SIZE + 4, y + BORDER_SIZE + 4, ID2.str(), EXTRA_SMALL, 255, 255, 0);
				}
			}
		}

		if (board.enemyHandler.enemies.size() > 0)
		{
			for (vector<Enemy*>::iterator e = board.enemyHandler.enemies.begin(); e != board.enemyHandler.enemies.end(); ++e)
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

		for (vector<Tower*>::iterator t = board.towerHandler.towers.begin(); t != board.towerHandler.towers.end(); ++t)
		{
			if ((*t)->hasEnemy)
			{
				engine.graphics.drawLine((*t)->getX() + (BLOCK_SIZE / 2), (*t)->getY() + (BLOCK_SIZE / 2), (*t)->enemy->getX() + (BLOCK_SIZE / 2), (*t)->enemy->getY() + (BLOCK_SIZE / 2));
			}
		}
	}
}

void GameScreen::drawPauseMenu()
{
	if (pauseMenu.paused)
	{
		engine.graphics.drawRectangle(PAUSE_Menu_X, PAUSE_Menu_Y, PAUSE_Menu_WIDTH, PAUSE_Menu_HEIGHT, 0, 0, 0);
		engine.graphics.drawRectangleOL(PAUSE_Menu_X - 1, PAUSE_Menu_Y - 1, PAUSE_Menu_WIDTH + 2, PAUSE_Menu_HEIGHT + 2, 255, 255, 255);
		engine.graphics.renderText(PAUSE_Menu_X + 150, PAUSE_Menu_Y+30, "Pause", LARGE, 255, 255, 255, "bombardier");

		for (vector<Button*>::iterator b = pauseMenu.buttonHandler.buttons.begin(); b != pauseMenu.buttonHandler.buttons.end(); ++b)
		{
			if ((*b)->isVisible())
			{
				if ((*b)->isHovered())
				{
					engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 0, 255);
					engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 0, 255, "anonymous");
				}
				else if ((*b)->isSelected())
				{
					engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 255, 0);
					engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 255, 0, "anonymous");
				}
				else
				{
					engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 255, 255);
					engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 255, 255, "anonymous");
				}
			}
		}
	}
}

void GameScreen::drawSidebar()
{
	stringstream creditText, scoreText;

	// Top area
	creditText << "Credit: " << board.bank.getCredit();
	engine.graphics.renderText(SIDEBAR_X + BLOCK_SIZE, BORDER_SIZE/2, creditText.str(), LARGE, 255, 255, 255, "bombardier");

	scoreText << "Score: " << board.getScore();
	engine.graphics.renderText(SIDEBAR_X + BLOCK_SIZE, 180, scoreText.str(), LARGE, 255, 255, 255, "bombardier");

	// Divider
	engine.graphics.drawLine(SIDEBAR_X, STATS_Y, WINDOW_WIDTH, STATS_Y);

	// Bottom area
	if (sidebar.buttonHandler.getButtonSelected())
	{
		drawSidebarSelectedButton();
	}
	else if (board.enemySelected)
	{
		drawSidebarEnemyStats();
	}
	else if (board.towerSelected)
	{
		drawSidebarTowerStats();
	}
}

void GameScreen::drawSidebarEnemyStats()
{
	stringstream IDText, healthText, valueText, bountyText;
	eStats e = board.selectedEnemyStats;
	int enemyStatsStartX = SIDEBAR_X + BLOCK_SIZE;
	int enemyStatsStartY = STATS_Y + BLOCK_SIZE;

	IDText << "Enemy ID: " << e.id;
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY, IDText.str(), SMALL, 255, 0, 0, "anonymous");
	healthText << "Health: " << e.currentHealth;
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + BLOCK_SIZE, healthText.str(), SMALL, 255, 0, 0, "anonymous");
	valueText << "Value: " << e.value << " points.";
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + (BLOCK_SIZE * 2), valueText.str(), SMALL, 255, 0, 0, "anonymous");
	bountyText << "Bounty: " << e.bounty << " credits.";
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY + (BLOCK_SIZE * 3), bountyText.str(), SMALL, 255, 0, 0, "anonymous");
}

void GameScreen::drawSidebarTowerStats()
{
	stringstream IDText, damageText, rangeText, killsText;
	tStats t = board.selectedTowerStats;
	int towerStatsStartX = SIDEBAR_X + BLOCK_SIZE;
	int towerStatsStartY = STATS_Y + BLOCK_SIZE;

	IDText << "Tower ID: " << t.id;
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY, IDText.str(), SMALL, 255, 255, 255, "anonymous");
	damageText << "Damage: " << t.damage;
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY + BLOCK_SIZE, damageText.str(), SMALL, 255, 255, 255, "anonymous");
	rangeText << "Range: " << (t.range/BLOCK_SIZE) << " tiles.";
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY + (BLOCK_SIZE * 2), rangeText.str(), SMALL, 255, 255, 255, "anonymous");
	killsText << "Total Kills: " << t.kills;
	engine.graphics.renderText(towerStatsStartX, towerStatsStartY + (BLOCK_SIZE * 3), killsText.str(), SMALL, 255, 255, 255, "anonymous");
}

void GameScreen::drawSidebarSelectedButton()
{
	stringstream buttonText;
	int enemyStatsStartX = SIDEBAR_X + BLOCK_SIZE;
	int enemyStatsStartY = STATS_Y + BLOCK_SIZE;

	buttonText << "Button Selected: " << sidebar.buttonHandler.getSelectedButton()->text;
	engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY, buttonText.str(), SMALL, 255, 0, 0, "anonymous");
}

void GameScreen::drawSidebarButtons()
{
	for (vector<Button*>::iterator b = sidebar.buttonHandler.buttons.begin(); b != sidebar.buttonHandler.buttons.end(); ++b)
	{
		if ((*b)->isVisible())
		{
			if ((*b)->isHovered())
			{
				engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 0, 255);
				engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 0, 255, "anonymous");
			}
			else if ((*b)->isSelected())
			{
				engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 255, 0);
				engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 255, 0, "anonymous");
			}
			else
			{
				engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 255, 255);
				engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 255, 255, "anonymous");
			}
		}
	}
}


// *** UPDATE METHODS *** //

int GameScreen::update()
{
	if(transition.isClosed) openTransition();

	int state = UNCHANGED_STATE;

	state = getInput();
	
	while (pauseMenu.paused)
	{
		state = pauseMenu.getInput();
		pauseMenu.update();
		draw();
	}

	board.update();
	sidebar.update(board.towerSelected);
	draw();

	engine.admin.updateFPS();

	if (state == EXIT_CURRENT_STATE) closeTransition();

	return state;
}

int GameScreen::getInput()
{
	input k = engine.interfaces.getInput();

	if (k.keyPress)
	{
		switch (k.key)
		{
		case SDLK_ESCAPE:
			return EXIT_CURRENT_STATE;

		case SDLK_q:
			return EXIT_APPLICATION;

		case SDLK_p:
			pauseMenu.pause();

		default:
			break;
		}
	}
	
	if (k.x > BORDER_SIZE && k.x < (BORDER_SIZE + (BOARD_WIDTH*BLOCK_SIZE)) && k.y > BORDER_SIZE && k.y < (BORDER_SIZE + (BOARD_HEIGHT*BLOCK_SIZE)))
	{
		return handleBoardInput(k);
	}
	else
	{
		return handleSidebarInput(k);
	}

	return UNCHANGED_STATE;
}

int GameScreen::handleBoardInput(input k)
{
	if (k.mouseDown)
	{
		if (cursor.getAction() == 1)
		{
			if (board.towerHandler.buildTower(board.map, cursor, board.bank))
			{
				board.enemyHandler.updateEnemyPaths(cursor.getX(), cursor.getY(), board.map);
			}
		}
		else if (cursor.getAction() == 2)
		{
			board.enemyHandler.launchEnemy(cursor, board.map);
		}
		else if (cursor.getAction() == 3)
		{
			board.map.targetX = cursor.getX(); board.map.targetY = cursor.getY();
			board.enemyHandler.updateEnemyTargets(board.map);
		}
		else
		{
			board.selectObject(cursor);
		}
	}
	else
	{
		cursor.setCoordinates((k.x - (k.x % BLOCK_SIZE)), (k.y - (k.y % BLOCK_SIZE)));
	}

	return UNCHANGED_STATE;
}

int GameScreen::handleSidebarInput(input k)
{
	if (k.mouseDown)
	{
		int button = sidebar.buttonHandler.getHoveredButtonId();

		switch (button)
		{
		case 1:		// SELL TOWER
			board.deselectObject();
			board.towerHandler.sellTower(board.map, board.selectedTowerStats.id, board.bank);
			break;

		case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: // BUY TOWER
			board.deselectObject();
			sidebar.buttonHandler.selectButton(button);
			cursor.setAction(1);
			break;

		case 12: // CHANGE TARGET
			board.deselectObject();
			sidebar.buttonHandler.selectButton(button);
			cursor.setAction(3);
			break;

		case 13: // LAUNCH ENEMY
			board.deselectObject();
			sidebar.buttonHandler.selectButton(button);
			cursor.setAction(2);
			break;

		case 14: // TOGGLE DEBUG
			if (board.debugMode)
			{
				sidebar.buttonHandler.deselectButton(button);
				board.debugMode = false;
			}
			else
			{
				sidebar.buttonHandler.selectButton(button);
				board.debugMode = true;
			}
			break;

		case 15:
			board.deselectObject();
			sidebar.buttonHandler.deselectAllButtons();
			cursor.setAction(0);
			break;
		
		case 16:
			pauseMenu.pause();
			break;

		case 17:
			return EXIT_CURRENT_STATE;

		default:
			break;
		}		
	}
	else
	{
		cursor.setCoordinates(k.x, k.y);
		sidebar.buttonHandler.setHoveredButton(cursor);
	}

	return UNCHANGED_STATE;
}

void GameScreen::closeTransition()
{
	transition.closing = true;
	while (transition.closing)
	{
		transition.close();

		engine.graphics.clear();
		drawBoardMap();
		drawBoardPieces();
		drawSidebar();
		drawSidebarButtons();
		drawCursor();
		drawDebugFeatures();
		drawPauseMenu();		
		transition.draw();
		engine.graphics.update();
	}
}

void GameScreen::openTransition()
{
	transition.opening = true;
	while (transition.opening)
	{
		transition.open();

		engine.graphics.clear();
		drawBoardMap();
		drawBoardPieces();
		drawSidebar();
		drawSidebarButtons();
		drawCursor();
		drawDebugFeatures();
		drawPauseMenu();
		transition.draw();
		engine.graphics.update();
	}
}