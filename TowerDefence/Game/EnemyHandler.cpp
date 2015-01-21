#include "EnemyHandler.h"

EnemyHandler::EnemyHandler(void){}
	
void EnemyHandler::init(Engine& e, Map* m)								// Setting up the EnemyHandler
{
	engine = e;
	enemyCount = 1;

	Enemy * enemy = new Enemy(engine.resources, (*m).startX, (*m).startY, 1, (*m).targetX, (*m).targetY, 1, m, enemyCount);
	enemies.push_back(enemy);
}

void EnemyHandler::launchEnemy(Cursor &cursor, Map* m)
{
	enemyCount++;
	Enemy * e = new Enemy(engine.resources, cursor.getX(), cursor.getY(), 0, (*m).targetX, (*m).targetY, 1, m, enemyCount);
	enemies.push_back(e);
}

void EnemyHandler::updateEnemyPaths(int x, int y, Map* m)
{
	for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		for (unsigned i = (*e)->getPath().getPathSize(); i > 0; i--)
		{
			int pathX = ((*e)->getPath().getXAt(i)*BLOCK_SIZE) + BORDER_SIZE;
			int pathY = ((*e)->getPath().getYAt(i)*BLOCK_SIZE) + BORDER_SIZE;

			if (pathX == x && pathY == y)
			{
				(*e)->updatePath(m);
				break;
			}
		}
	}
}

void EnemyHandler::updateEnemyTargets(Map* m)
{
	for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		(*e)->updateTarget((*m).targetX, (*m).targetY, m);
	}
}

void EnemyHandler::destroyObjects()
{
	for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end();)
	{
		if ((*e)->isDeleted())
		{
			delete *e;
			e = enemies.erase(e);
		}
		else e++;
	}
}