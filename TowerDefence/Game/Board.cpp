#include "Board.h"

Board::Board(void){}
	
void Board::setup(Engine &e)								// Setting up the Board
{
	engine = e;			
	map.init(0);
	towerHandler.init(&map);
	enemyHandler.init(engine, &map);
	eTimer = SDL_GetTicks() + 300;
}

void Board::update()
{
	if (SDL_GetTicks() > eTimer)
	{
		for (std::vector<Enemy*>::iterator e = enemyHandler.enemies.begin(); e != enemyHandler.enemies.end(); ++e)
		{
			if ((*e)->canWalk(&map))
			{
				engine.physics.move((*e), (*e)->nextMove(), (*e)->getSpeed());
			}
		}

		for (std::vector<Tower*>::iterator t = towerHandler.towers.begin(); t != towerHandler.towers.end(); ++t)
		{
			(*t)->update(&map, &enemyHandler.enemies);

			for (std::vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end(); ++b)
			{
				engine.physics.nonUniformMove((*b), (*b)->dX, (*b)->dY);
			}
		}

		eTimer = SDL_GetTicks() + 0;
	}

	cleanup();
}

void Board::checkForObject(Cursor &cursor)
{
	int x = cursor.getX() - BORDER, y = cursor.getY() - BORDER;

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
			}
		}
	}
}

void Board::cleanup()
{
	enemyHandler.destroyObjects();
	towerHandler.destroyObjects();
}