#include "EnemyHandler.h"

EnemyHandler::EnemyHandler(void){}
	
void EnemyHandler::init(Engine &e, Map &m)								// Setting up the EnemyHandler
{
	engine = e;
	enemyCount = 1;
	enemies.clear();
	Enemy * enemy = new Enemy(engine.resources, m.startX, m.startY, 1, m.targetX, m.targetY, 1, m, enemyCount);
	enemies.push_back(enemy);
}

void EnemyHandler::draw()
{
	for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		if (selected && (*e)->getID() == selectedStats.id)
		{
			engine.graphics.drawRectangleOL((*e)->getX() - 1, (*e)->getY() - 1, BLOCK_SIZE + 2, BLOCK_SIZE + 2, 255, 255, 255);
		}

		engine.graphics.drawRectangle((*e)->getX(), (*e)->getY(), BLOCK_SIZE, BLOCK_SIZE, 255, 0, 0);
		engine.graphics.drawRectangle((*e)->getX(), (*e)->getY(), (int)(((*e)->getStats().currentHealth / (*e)->getStats().maxHealth) * BLOCK_SIZE), 2, 0, 255, 0);
	}

	if (selected)
	{
		std::stringstream IDText, healthText, mHealthText, valueText, bountyText;
		int statsStartX = SIDEBAR_X + BLOCK_SIZE;
		int statsStartY = STATS_Y + BLOCK_SIZE;

		IDText << "Enemy ID: " << selectedStats.id;
		engine.graphics.renderText(statsStartX, statsStartY, IDText.str(), SMALL, 255, 0, 0, "anonymous");
		healthText << "Current Health: " << selectedStats.currentHealth;
		engine.graphics.renderText(statsStartX, statsStartY + BLOCK_SIZE, healthText.str(), SMALL, 255, 0, 0, "anonymous");
		mHealthText << "Max Health: " << selectedStats.maxHealth;
		engine.graphics.renderText(statsStartX, statsStartY + (BLOCK_SIZE * 2), mHealthText.str(), SMALL, 255, 0, 0, "anonymous");
		valueText << "Value: " << selectedStats.value << " points.";
		engine.graphics.renderText(statsStartX, statsStartY + (BLOCK_SIZE * 3), valueText.str(), SMALL, 255, 0, 0, "anonymous");
		bountyText << "Bounty: " << selectedStats.bounty << " credits.";
		engine.graphics.renderText(statsStartX, statsStartY + (BLOCK_SIZE * 4), bountyText.str(), SMALL, 255, 0, 0, "anonymous");
	}
}

void EnemyHandler::launch(Cursor &cursor, Map &m)
{
	enemyCount++;
	Enemy * e = new Enemy(engine.resources, cursor.getX(), cursor.getY(), 0, m.targetX, m.targetY, 1, m, enemyCount);
	enemies.push_back(e);
}

void EnemyHandler::updatePaths(int x, int y, Map &m)
{
	for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
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

void EnemyHandler::updateTargets(Map &m)
{
	for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		(*e)->updateTarget(m.targetX, m.targetY, m);
	}
}

void EnemyHandler::destroy()
{
	for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end();)
	{
		if ((*e)->isDeleted())
		{
			delete *e;
			e = enemies.erase(e);
		}
		else e++;
	}
}