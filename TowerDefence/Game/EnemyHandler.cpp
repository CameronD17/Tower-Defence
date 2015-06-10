#include "EnemyHandler.h"

EnemyHandler::EnemyHandler(void){}

EnemyHandler::~EnemyHandler(void)
{
	waves.clear();
	enemies.clear();
	awaitinglaunch.clear();
}
	
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
				awaitinglaunch.push_back(boost::shared_ptr<Enemy>(new Enemy(engine, m.startX - TILE_SIZE, m.startY - TILE_SIZE, type, m.targetX, m.targetY, level, m, enemyCount)));
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
	for (std::vector<boost::shared_ptr<Enemy>>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		(*e)->draw();
	}

	std::stringstream wave;
	wave << (waveCount - waves.size() + 1) << " / " << waveCount;
	engine.graphics.renderText(SIDEBAR_X + TILE_SIZE, STATS_Y - TILE_SIZE, wave.str(), SMALL);

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

void EnemyHandler::update(Map &m)
{
	// Destroy any enemies that have been killed, and release new enemies into the field
	destroy();
	launch(m);

	// Update all enemies
	for (std::vector<boost::shared_ptr<Enemy>>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		(*e)->move(m);
	}
}

void EnemyHandler::launch(Map &m)
{
	if (SDL_GetTicks() > timer)
	{
		if (autolaunch && waveTimer <= SDL_GetTicks()) nextWave();

		if (waves.back().numberOfEnemies == launched && nextWaveWaiting)
		{
			waves.pop_back();
			nextWaveWaiting = false;
			launched = 0;
			timer = SDL_GetTicks() + ENEMY_TIMER;
		}

		if (waves.back().numberOfEnemies > launched)
		{
			enemies.push_back(awaitinglaunch.back());
			awaitinglaunch.pop_back(); 
			launched++;
			timer = SDL_GetTicks() + ENEMY_TIMER;
		}
	}
}

void EnemyHandler::launch(Cursor &cursor, Map &m)
{
	enemyCount++;
	//Enemy * e = new Enemy(engine, cursor.getX(), cursor.getY(), 0, m.targetX, m.targetY, 1, m, enemyCount);
	//enemies.push_back(e);
}

void EnemyHandler::nextWave()
{
	if (waves.back().numberOfEnemies == launched && waves.size() > 0)
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
	for (std::vector<boost::shared_ptr<Enemy>>::iterator e = awaitinglaunch.begin(); e != awaitinglaunch.end(); ++e)
	{
		(*e)->updatePath(m, x, y);
	}

	for (std::vector<boost::shared_ptr<Enemy>>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		(*e)->updatePath(m, x, y);
	}
}

void EnemyHandler::updateTargets(Map &m)
{
	for (std::vector<boost::shared_ptr<Enemy>>::iterator e = awaitinglaunch.begin(); e != awaitinglaunch.end(); ++e)
	{
		(*e)->updateTarget(m.targetX, m.targetY, m);
	}

	for (std::vector<boost::shared_ptr<Enemy>>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		(*e)->updateTarget(m.targetX, m.targetY, m);
	}
}

void EnemyHandler::destroy()
{
	for (std::vector<boost::shared_ptr<Enemy>>::iterator e = enemies.begin(); e != enemies.end();)
	{
		if ((*e)->isDeleted())
		{
			//delete *e;
			e = enemies.erase(e);
		} 
		else e++;
	}
}