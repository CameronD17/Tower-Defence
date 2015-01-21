#include "Board.h"

Board::Board(void){}
	
void Board::setup(Engine &e)								// Setting up the Board
{
	engine = e;			
	map.init(0);
	towerHandler.init(&map);
	enemyHandler.init(engine, &map);
	bank.init(10000);
	eTimer = SDL_GetTicks() + 300;
}

int Board::getScore()
{
	return score;
}

int Board::getLevel()
{
	return level;
}

void Board::update()
{
	if (SDL_GetTicks() > eTimer)
	{
		for (std::vector<Tower*>::iterator t = towerHandler.towers.begin(); t != towerHandler.towers.end(); ++t)
		{
			(*t)->update(&map, &enemyHandler.enemies);

			for (std::vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end(); ++b)
			{
				(*b)->update((*t)->enemy);
				engine.physics.nonUniformMove((*b), (*b)->getDX(), (*b)->getDY());
				
				if ((*b)->hasHit())
				{
					if ((*t)->enemy->reduceHealth((*t)->getStats().damage, &map)) (*t)->incrementKills();
				}
			}

			if (towerSelected)
			{
				if ((*t)->getID() == selectedTowerStats.id)
				{
					selectedTowerStats = (*t)->getStats();
				}
			}
		}	

		for (std::vector<Enemy*>::iterator e = enemyHandler.enemies.begin(); e != enemyHandler.enemies.end(); ++e)
		{
			if ((*e)->canWalk(&map))
			{
				engine.physics.move((*e), (*e)->getNextMove(), (*e)->getSpeed());
			}

			if (enemySelected)
			{
				if ((*e)->getID() == selectedEnemyStats.id)
				{
					if ((*e)->isDeleted())
					{
						deselectObject();
					}
					else
					{
						selectedEnemyStats = (*e)->getStats();
					}
				}
			}
		}
		eTimer = SDL_GetTicks() + 0;
	}

	cleanup();
}

void Board::selectObject(Cursor &cursor)
{
	int x = cursor.getX() - BORDER_SIZE, y = cursor.getY() - BORDER_SIZE;

	if (map.getTerrain(x, y) == HAS_ENEMY)
	{
		objectSelected = true;
		enemySelected = true;
		towerSelected = false;

		for (std::vector<Enemy*>::iterator e = enemyHandler.enemies.begin(); e != enemyHandler.enemies.end(); ++e)
		{
			if ((*e)->getID() == map.getEnemy(x, y))
			{
				selectedEnemyStats = (*e)->getStats();
				break;
			}
		}		
	}
	else if (map.getTerrain(x, y) == HAS_TOWER)
	{
		objectSelected = true;
		enemySelected = false;
		towerSelected = true;

		for (std::vector<Tower*>::iterator t = towerHandler.towers.begin(); t != towerHandler.towers.end(); ++t)
		{
			if ((*t)->getID() == map.getTower(x, y))
			{
				selectedTowerStats = (*t)->getStats();
				break;
			}
		}
	}
}

void Board::deselectObject()
{
	objectSelected = false;
	towerSelected = false;
	enemySelected = false;
}

void Board::cleanup()
{
	towerHandler.destroyObjects();
	enemyHandler.destroyObjects();
}