#include "TowerHandler.h"

TowerHandler::TowerHandler(void){}
	
void TowerHandler::init(Engine &e, Map &m)
{
	engine = e;
	towers.clear();
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			terrain[x][y] = m.getTerrain(x*BLOCK_SIZE, y*BLOCK_SIZE);
		}
	}
}

void TowerHandler::draw()
{
	for (std::vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
	{
		if (selected && (*t)->getID() == selectedStats.id)
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

		if ((*t)->getStats().level > 0)
		{
			switch ((*t)->getStats().level)
			{
			case 1:
				engine.graphics.drawRectangle((*t)->getX() + 20, (*t)->getY() + 2, 2, 5, 205, 127, 50);
				break;
			case 2:
				engine.graphics.drawRectangle((*t)->getX() + 17, (*t)->getY() + 2, 2, 5, 192, 192, 192);
				engine.graphics.drawRectangle((*t)->getX() + 20, (*t)->getY() + 2, 2, 5, 192, 192, 192);
				break;
			case 3:
				engine.graphics.drawRectangle((*t)->getX() + 17, (*t)->getY() + 2, 5, 5, 255, 205, 50);
				break;
			default:
				break;
			}
		}

		for (std::vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end(); ++b)
		{
			engine.graphics.drawRectangle((*b)->getX(), (*b)->getY(), 2, 2, 255, 255, 255);
		}
	}

	if (selected)
	{
		std::stringstream IDText, damageText, rangeText, killsText;
		int statsStartX = SIDEBAR_X + BLOCK_SIZE;
		int statsStartY = STATS_Y + BLOCK_SIZE;

		IDText << "Tower ID: " << selectedStats.id;
		engine.graphics.renderText(statsStartX, statsStartY, IDText.str(), SMALL, 255, 255, 255, "anonymous");
		damageText << "Damage: " << selectedStats.damage;
		engine.graphics.renderText(statsStartX, statsStartY + BLOCK_SIZE, damageText.str(), SMALL, 255, 255, 255, "anonymous");
		rangeText << "Range: " << (selectedStats.range / BLOCK_SIZE) << " tiles.";
		engine.graphics.renderText(statsStartX, statsStartY + (BLOCK_SIZE * 2), rangeText.str(), SMALL, 255, 255, 255, "anonymous");
		killsText << "Total Kills: " << selectedStats.kills;
		engine.graphics.renderText(statsStartX, statsStartY + (BLOCK_SIZE * 3), killsText.str(), SMALL, 255, 255, 255, "anonymous");
	}
}

bool TowerHandler::build(Map &map, Cursor &cursor, Bank &bank, Notification &notification)
{
	int x = cursor.getX() - BORDER_SIZE, y = cursor.getY() - BORDER_SIZE;
		
	if (map.buildable(x, y))															// a) Can the terran be built upon (not blocked)?
	{		
		if (!(x == map.startX && y == map.startY))										// b) Is the player trying to build on the enemy spawn point?
		{			
			if (bank.getCredit() >= cursor.getStats().cost)								// c) Does the player have enough credit to purchase the tower?
			{
				char terrainReset = map.getTerrain(x, y);
				Pathfinder p;
				map.setTerrain(x, y, BLOCKED_TERRAIN);

				if (p.findPath(map.startX, map.startY, map.targetX, map.targetY, map))	// d) Does the new tower completely block a route from enemy spawn to player base?
				{
					counter++;
					Tower * t = new Tower(cursor.getX(), cursor.getY(), cursor.getStats(), counter, map);
					map.setTower(x, y, counter);
					towers.push_back(t);
					bank.decreaseCredit(t->getStats().cost);
					notification.set("New Tower Built!", SUCCESS);
					return true;
				}
				else // d)
				{
					map.setTerrain(x, y, terrainReset);
					notification.set("Invalid placement - you cannot block the route to your base.", ERROR);
				}
			}
			else // c)
			{
				notification.set("Insufficient funds available to build.", ERROR);
			}
		}
		else // b)
		{
			notification.set("Invalid placement - you cannot build on the enemy spawn point.", ERROR);
		}
	}
	else // a)
	{
		notification.set("Invalid placement - this terrain is obstructed.", ERROR);
	}

	return false;
}

void TowerHandler::sell(Map &map, int id, Bank &bank, Notification &notification)
{	
	for (std::vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
	{
		if ((*t)->getID() == id)
		{
			map.setTower((*t)->getX() - BORDER_SIZE, (*t)->getY() - BORDER_SIZE, 0);
			map.setTerrain((*t)->getX() - BORDER_SIZE, (*t)->getY() - BORDER_SIZE, terrain[((*t)->getX() - BORDER_SIZE) / BLOCK_SIZE][((*t)->getY() - BORDER_SIZE) / BLOCK_SIZE]);
			bank.increaseCredit((*t)->getStats().cost / 2);
			(*t)->setDeleted(true);
			notification.set("Tower sold!", SUCCESS);
			selected = false;
		}
	}
}

void TowerHandler::upgrade(int id, Bank &bank, Notification &notification)
{
	for (std::vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
	{
		if ((*t)->getID() == id)
		{
			if ((*t)->getStats().level < (*t)->getStats().maxLevel)
			{
				(*t)->upgrade();
				bank.decreaseCredit((*t)->getStats().cost);
				std::stringstream notif;
				notif << "Tower " << (*t)->getStats().id << " upgraded to level " << (*t)->getStats().level;
				notification.set(notif.str(), SUCCESS);
			}
		}
	}
}

void TowerHandler::destroy()
{
	for (std::vector<Tower*>::iterator t = towers.begin(); t != towers.end();)
	{
		for (std::vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end();)
		{
			if ((*b)->isDeleted())
			{
				delete *b;
				b = (*t)->bullets.erase(b);
			}
			else b++;
		}

		if ((*t)->isDeleted())
		{
			for (std::vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end();)
			{
				delete *b;
				b = (*t)->bullets.erase(b);
			}
			delete *t;
			t = towers.erase(t);
		}
		else t++;
	}
}