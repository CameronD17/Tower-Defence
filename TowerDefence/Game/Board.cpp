#include "Board.h"

Board::Board(void){}
	
void Board::setup(Engine &e, std::string game)
{
	engine = e;		
	map.init(engine, game);
	towerHandler.init(engine, map);
	enemyHandler.init(engine, map, game);
	bank.init(engine, 10000);
	eTimer = SDL_GetTicks();
	debugMode = false;
}

int Board::getInput(input k, Cursor &cursor, Notification& notification)
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

			if (map.hasEnemy(x, y))
			{
				enemyHandler.selected = true;
				towerHandler.selected = false;

				for (std::vector<Enemy*>::iterator e = enemyHandler.enemies.begin(); e != enemyHandler.enemies.end(); ++e)
				{
					if ((*e)->getID() == map.getEnemy(x, y))
					{
						enemyHandler.selectedStats = (*e)->getStats();
						break;
					}
				}
			}
			else if (map.hasTower(x, y))
			{
				enemyHandler.selected = false;
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
		cursor.setCoordinates((k.x - (k.x % BLOCK_SIZE)), (k.y - (k.y % BLOCK_SIZE)));
	}

	return UNCHANGED_STATE;
}

void Board::update()
{
	// No point in updating towers that have been sold
	towerHandler.destroy();

	// Update all remaining towers
	for (std::vector<Tower*>::iterator t = towerHandler.towers.begin(); t != towerHandler.towers.end(); ++t)
	{
		(*t)->update(map, enemyHandler.enemies);

		for (std::vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end(); ++b)
		{
			(*b)->update((*t)->enemy);
			engine.physics.nonUniformMove((*b), (*b)->getDX(), (*b)->getDY());

			if ((*b)->hasHit())
			{
				if ((*t)->enemy->reduceHealth((*t)->getStats().damage, map))
				{
					(*t)->incrementKills();
					bank.increaseScore((*t)->enemy->getStats().bounty);
				}
			}
		}

		if (towerHandler.selected)
		{
			if ((*t)->getID() == towerHandler.selectedStats.id)
			{
				towerHandler.selectedStats = (*t)->getStats();
			}
		}
	}

	// Destroy any enemies that have been killed
	enemyHandler.destroy();

	// Launch new enemies
	if (SDL_GetTicks() > eTimer)
	{		
		enemyHandler.launch(map);
		eTimer = SDL_GetTicks() + 1000;
	}

	// Update all remaining enemies
	for (std::vector<Enemy*>::iterator e = enemyHandler.enemies.begin(); e != enemyHandler.enemies.end(); ++e)
	{
		if ((*e)->leftBase)
		{
			if ((*e)->canWalk(map))
			{
				engine.physics.move((*e), (*e)->getNextMove(), (*e)->getSpeed());
			}

			if (enemyHandler.selected)
			{
				if ((*e)->getID() == enemyHandler.selectedStats.id)
				{
					if ((*e)->isDeleted())
					{
						deselectObject();
					}
					else
					{
						enemyHandler.selectedStats = (*e)->getStats();
					}
				}
			}
		}
	}
}

void Board::draw()
{
	map.draw();
	towerHandler.draw();
	enemyHandler.draw();
	bank.draw();

	Uint8 red = 154, green = 0, blue = 0;
	engine.graphics.drawRectangle(0, 0, BORDER_SIZE, ((BORDER_SIZE * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue); // Left
	engine.graphics.drawRectangle(BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), 0, BORDER_SIZE, ((BORDER_SIZE * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue); //Right
	engine.graphics.drawRectangle(0, 0, BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), BORDER_SIZE, red, green, blue); // Top
	engine.graphics.drawRectangle(0, BORDER_SIZE + (BOARD_HEIGHT*BLOCK_SIZE), BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), BORDER_SIZE, red, green, blue);	// Bottom

	if (debugMode)
	{
		for (std::vector<Enemy*>::iterator e = enemyHandler.enemies.begin(); e != enemyHandler.enemies.end(); ++e)
		{
			std::stringstream idNo;
			idNo << (*e)->getID();
			engine.graphics.renderText((*e)->getX() + 20, (*e)->getY() + 20, idNo.str(), EXTRA_SMALL);
		}

		for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
		{
			for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
			{
				if (map.hasEnemy(x, y))
				{
					engine.graphics.drawRectangleOL(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 255);
					std::stringstream ID2;
					ID2 << map.getEnemy(x, y);
					engine.graphics.renderText(x + BORDER_SIZE + 4, y + BORDER_SIZE + 4, ID2.str(), EXTRA_SMALL, 255, 255, 0);
				}
			}
		}
		
		/*if (enemyHandler.enemies.size() > 0)
		{
			for (std::vector<Enemy*>::iterator e = enemyHandler.enemies.begin(); e != enemyHandler.enemies.end(); ++e)
			{
				int thisX = map.targetX + (BLOCK_SIZE / 2);
				int thisY = map.targetY + (BLOCK_SIZE / 2);

				for (int i = 0; i < (*e)->getPath().getPathSize(); i++)
				{
					int nextX = ((*e)->getPath().getXAt(i) * BLOCK_SIZE) + (BLOCK_SIZE / 2) + BORDER_SIZE;
					int nextY = ((*e)->getPath().getYAt(i) * BLOCK_SIZE) + (BLOCK_SIZE / 2) + BORDER_SIZE;

					engine.graphics.drawLine(thisX, thisY, nextX, nextY);

					thisX = nextX;
					thisY = nextY;
				}
			}
		}*/

		for (std::vector<Tower*>::iterator t = towerHandler.towers.begin(); t != towerHandler.towers.end(); ++t)
		{
			if ((*t)->hasEnemy)
			{
				engine.graphics.drawLine((*t)->getX() + (BLOCK_SIZE / 2), (*t)->getY() + (BLOCK_SIZE / 2), (*t)->enemy->getX() + (BLOCK_SIZE / 2), (*t)->enemy->getY() + (BLOCK_SIZE / 2));
			}
		}
	}
}

void Board::deselectObject()
{
	towerHandler.selected = false;
	enemyHandler.selected = false;
}