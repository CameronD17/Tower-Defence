#include "EnemyHandler.h"

EnemyHandler::EnemyHandler(void){}
	
void EnemyHandler::init(Engine &e, Map &m, std::string game)
{
	engine = e;
	enemyCount = 0;
	waveCount = 0;
	launched = 0;
	timer = SDL_GetTicks() + ENEMY_TIMER;
	waveTimer = SDL_GetTicks();
	autolaunch = false;
	nextWaveWaiting = false;
	awaitinglaunch.clear();
	enemies.clear();
	waves.clear();

	std::stringstream filename;
	filename << game << "enemies.txt";

	loadEnemiesFromFile(m, filename.str());
}

bool EnemyHandler::loadEnemiesFromFile(Map &m, std::string filename)
{
	std::ifstream data;
	data.open(filename);

	int size, type, level; bool isBoss;

	if (data.good())
	{
		while (data >> size >> type >> level >> isBoss)
		{
			for (int i = 0; i < size; i++)
			{
				enemyCount++;
				Enemy * enemy = new Enemy(engine, m.startX - BLOCK_SIZE, m.startY - BLOCK_SIZE, type, m.targetX, m.targetY, level, m, enemyCount);
				awaitinglaunch.push_back(enemy);
			}
			waves.push_back({ size, type, level, isBoss });
			waveCount++;
		}

		data.close();
		return true;
	}	

	return false;
}

void EnemyHandler::draw()
{
	for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		/*if (selected && (*e)->getID() == selectedStats.id)
		{
			engine.graphics.drawRectangleOL((*e)->getX() - 1, (*e)->getY() - 1, BLOCK_SIZE + 2, BLOCK_SIZE + 2, 255, 255, 255);
		}*/

		(*e)->draw();
	}

	/*if (selected)
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
	}*/

	std::stringstream wave;
	wave << (waveCount - waves.size() + 1) << " / " << waveCount;
	engine.graphics.renderText(SIDEBAR_X + BLOCK_SIZE, STATS_Y - BLOCK_SIZE, wave.str(), SMALL);

	if (autolaunch)
	{
		if (waveTimer < SDL_GetTicks())
		{
			engine.graphics.renderText(1212, 350, "QUEUED", MEDIUM);
		}
		else
		{
			std::stringstream time;
			time << std::fixed << std::setprecision(1) << (double)(waveTimer - SDL_GetTicks()) / 1000;
			engine.graphics.renderText(1212, 350, time.str(), MEDIUM);
		}
	}
}

void EnemyHandler::launch(Map &m)
{
	if (autolaunch && waveTimer <= SDL_GetTicks()) nextWave();

	if (waves.back().numberOfEnemies == launched && nextWaveWaiting)
	{
		waves.pop_back();
		nextWaveWaiting = false;
		launched = 0;
	}

	if (waves.back().numberOfEnemies > launched)
	{
		enemies.push_back(awaitinglaunch.back());
		awaitinglaunch.pop_back(); 
		launched++;
		timer = SDL_GetTicks() + ENEMY_TIMER;
	}
}

void EnemyHandler::launch(Cursor &cursor, Map &m)
{
	enemyCount++;
	Enemy * e = new Enemy(engine, cursor.getX(), cursor.getY(), 0, m.targetX, m.targetY, 1, m, enemyCount);
	enemies.push_back(e);
}

void EnemyHandler::nextWave()
{
	if (waves.back().numberOfEnemies == launched)
	{
		if (autolaunch)
		{
			waveTimer = SDL_GetTicks() + ENEMY_WAVE_TIMER;
		}
		waves.pop_back();
		nextWaveWaiting = false;
		launched = 0;
	}
	else
	{
		nextWaveWaiting = true;
	}
}

void EnemyHandler::autoLaunch(Map &m)
{
	if (autolaunch)
	{
		autolaunch = false;
		nextWaveWaiting = false;
	}
	else
	{
		autolaunch = true;
		waveTimer = SDL_GetTicks() + ENEMY_WAVE_TIMER;
		nextWave();
	}
}

void EnemyHandler::updatePaths(int x, int y, Map &m)
{
	for (std::vector<Enemy*>::iterator e = awaitinglaunch.begin(); e != awaitinglaunch.end(); ++e)
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
	for (std::vector<Enemy*>::iterator e = awaitinglaunch.begin(); e != awaitinglaunch.end(); ++e)
	{
		(*e)->updateTarget(m.targetX, m.targetY, m);
	}

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