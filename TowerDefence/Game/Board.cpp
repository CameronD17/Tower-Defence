#include "Board.h"

Board::Board(void){}
	
void Board::setup(Engine &e, std::string game)
{
	engine = e;		
	map.init(engine, game);
	towerHandler.init(engine, map);
	enemyHandler.init(engine, map, game);
	bank.init(engine, 10000);
	debugMode = false;
}

void Board::getInput(input k, Cursor &cursor, Notification& notification)
{
	if (k.mouseDown)
	{
		if (cursor.getAction() == TOWER_1)
		{
			if (towerHandler.build(map, cursor, bank, notification))
			{
				enemyHandler.updatePaths(cursor.getX(), cursor.getY(), map);
			}
		}
		else if (cursor.getAction() == LAUNCH_ENEMY)
		{
			enemyHandler.launch(cursor, map);
		}
		else if (cursor.getAction() == CHANGE_TARGET)
		{
			map.targetX = cursor.getX(); map.targetY = cursor.getY();
			enemyHandler.updateTargets(map);
		}
		else
		{
			int x = cursor.getX() - BORDER_SIZE, y = cursor.getY() - BORDER_SIZE;

			if (map.hasTower(x, y) && !towerHandler.selected)
			{
				towerHandler.selected = true;

				for (std::vector<Tower*>::iterator t = towerHandler.towers.begin(); t != towerHandler.towers.end(); ++t)
				{
					if ((*t)->getID() == map.getTower(x, y))
					{
						towerHandler.selectedStats = (*t)->getStats();
						break;
					}
				}
			}
		}
	}
	else
	{
		cursor.setCoordinates((k.x - (k.x % TILE_SIZE)), (k.y - (k.y % TILE_SIZE)));
	}
}

void Board::update()
{
	towerHandler.update(map, bank, enemyHandler.enemies);
	enemyHandler.update(map);
}

void Board::draw()
{
	map.draw();
	towerHandler.draw();
	enemyHandler.draw();
	bank.draw();

	Uint8 red = 154, green = 0, blue = 0;
	engine.graphics.drawRectangle(0, 0, BORDER_SIZE, (BORDER_SIZE * 2) + BOARD_HEIGHT, red, green, blue); // Left
	engine.graphics.drawRectangle(BORDER_SIZE + BOARD_WIDTH, 0, BORDER_SIZE, (BORDER_SIZE * 2) + BOARD_HEIGHT, red, green, blue); //Right
	engine.graphics.drawRectangle(0, 0, BORDER_SIZE + BOARD_WIDTH, BORDER_SIZE, red, green, blue); // Top
	engine.graphics.drawRectangle(0, BORDER_SIZE + BOARD_HEIGHT, BORDER_SIZE + BOARD_WIDTH, BORDER_SIZE, red, green, blue);	// Bottom

	if (debugMode)
	{
		for (std::vector<boost::shared_ptr<Enemy>>::iterator e = enemyHandler.enemies.begin(); e != enemyHandler.enemies.end(); ++e)
		{
			std::stringstream idNo;
			idNo << (*e)->getID();
			engine.graphics.renderText((*e)->getX() + 20, (*e)->getY() + 20, idNo.str(), EXTRA_SMALL);
		}

		for (int x = 0; x < BOARD_WIDTH; x += TILE_SIZE)
		{
			for (int y = 0; y < BOARD_HEIGHT; y += TILE_SIZE)
			{
				if (map.hasEnemy(x, y))
				{
					engine.graphics.drawRectangleOL(x + BORDER_SIZE, y + BORDER_SIZE, TILE_SIZE, TILE_SIZE, 255, 255, 255);
					std::stringstream ID2;
					ID2 << map.getEnemy(x, y);
					engine.graphics.renderText(x + BORDER_SIZE + 4, y + BORDER_SIZE + 4, ID2.str(), EXTRA_SMALL, 255, 255, 0);
				}
			}
		}

		for (std::vector<Tower*>::iterator t = towerHandler.towers.begin(); t != towerHandler.towers.end(); ++t)
		{
			if ((*t)->hasEnemy)
			{
				engine.graphics.drawLine((*t)->getX() + (TILE_SIZE / 2), (*t)->getY() + (TILE_SIZE / 2), (*t)->enemy->getX() + (TILE_SIZE / 2), (*t)->enemy->getY() + (TILE_SIZE / 2));
			}
		}
	}
}