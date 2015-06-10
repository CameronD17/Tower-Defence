#include "Tower.h"
#include <iostream>

Tower::Tower(Engine &e, int x, int y, int t, int id, Map &m)
{
	engine = e;
	setX(x);
	setY(y);
	setID(id);
	stats.id = id;

	m.setTower(x - BORDER_SIZE, y - BORDER_SIZE, id);

	stats.type = t;
	stats.maxCapacity = 1;
	stats.level = 0;
	stats.maxLevel = 3;
	stats.hits = 0;
	stats.kills = 0;

	switch (t)
	{
	case TOWER_1:		// Machine Gun			
		stats.cost = TOWER_1_COST;
		stats.damage = 4;
		stats.range = 4 * TILE_SIZE;
		stats.reload = 20;
		//texture = resource.getImage("Game/Images/Sprites/tower1.png"); 
		break;
	case TOWER_2:		// Light Cannon
		stats.cost = TOWER_2_COST;
		stats.damage = 6;
		stats.range = 6 * TILE_SIZE;
		stats.reload = 40;
		//texture = resource.getImage("Game/Images/Sprites/tower2.png"); 
		break;
	case TOWER_3:		// Sniper's Nest
		stats.cost = TOWER_3_COST;
		stats.damage = 15;
		stats.range = 10 * TILE_SIZE;
		stats.reload = 120;
		//texture = resource.getImage("Game/Images/Sprites/tower3.png"); 
		break;
	case TOWER_4:		// Sonic Blaster
		stats.cost = TOWER_4_COST;
		stats.damage = 6;
		stats.range = 6 * TILE_SIZE;
		stats.reload = 40;
		//texture = resource.getImage("Game/Images/Sprites/tower4.png");  
		break;
	case TOWER_5:		// Heavy Cannon
		stats.cost = TOWER_5_COST;
		stats.damage = 15;
		stats.range = 8 * TILE_SIZE;
		stats.reload = 80;
		//texture = resource.getImage("Game/Images/Sprites/tower5.png");  
		break;
	case TOWER_6:		// IED
		stats.cost = TOWER_6_COST;
		stats.damage = 20;
		stats.range = 1 * TILE_SIZE;
		stats.reload = 0;
		//texture = resource.getImage("Game/Images/Sprites/tower6.png");  
		break;
	case TOWER_7:		// Nuclear Artillery
		stats.cost = TOWER_7_COST;
		stats.damage = 30;
		stats.range = 4 * TILE_SIZE;
		stats.reload = 80;
		//texture = resource.getImage("Game/Images/Sprites/tower7.png");  
		break;
	case TOWER_8:		// Ghost Hunter
		stats.cost = TOWER_8_COST;
		stats.damage = 25;
		stats.range = 4 * TILE_SIZE;
		stats.reload = 40;
		//texture = resource.getImage("Game/Images/Sprites/tower8.png");  
		break;
	case TOWER_9:		// Spare Tower 1
		stats.cost = TOWER_9_COST;
		stats.damage = 0;
		stats.range = 0 * TILE_SIZE;
		stats.reload = 0;
		//texture = resource.getImage("Game/Images/Sprites/tower9.png");  
		break;
	case TOWER_10:		// Spare Tower 2
		stats.cost = TOWER_10_COST;
		stats.damage = 0;
		stats.range = 0 * TILE_SIZE;
		stats.reload = 0;
		//texture = resource.getImage("Game/Images/Sprites/tower10.png");  
		break;
	default:
		setTexture(NULL);
		//towerSelected = false;
		break;
	}
};

Tower::~Tower()
{

}

void Tower::upgrade()
{
	stats.level++;
	stats.damage *= 2;
}

void Tower::update(Map &m, Bank &bank, std::vector<boost::shared_ptr<Enemy>> &enemies)
{
	hasEnemy = false;
	for (int x = getX() - stats.range; x < getX() + stats.range; x += TILE_SIZE)
	{
		if (x > 0 && x < BOARD_WIDTH)
		{
			for (int y = getY() - stats.range; y < getY() + stats.range; y += TILE_SIZE)
			{
				if (y > 0 && y < BOARD_HEIGHT)
				{
					if (m.hasEnemy(x, y))
					{
						for (std::vector<boost::shared_ptr<Enemy>>::iterator e = enemies.begin(); e != enemies.end(); ++e)
						{
							if ((*e)->getID() == m.getEnemy(x, y))
							{
								enemy = (*e);
								hasEnemy = true;
							}
						}
					}
				}
			}
		}
	}

	if (!hasEnemy)
	{
		bullets.clear();
	}
	else
	{
		if ((signed)bullets.size() <= stats.maxCapacity)
		{
			Bullet * b = new Bullet((getX() + (TILE_SIZE / 2)), (getY() + (TILE_SIZE / 2)), stats.range, enemy->getX(), enemy->getY());
			bullets.push_back(b);
		}
		for (std::vector<Bullet*>::iterator b = bullets.begin(); b != bullets.end(); ++b)
		{
			(*b)->update(enemy);
			engine.physics.nonUniformMove((*b), (*b)->getDX(), (*b)->getDY());

			if ((*b)->hit())
			{
				if (enemy->reduceHealth(stats.damage, m, bank))
				{
					stats.kills++;
				}
			}
		}
	}
}

tStats Tower::getStats()const
{
	return stats;
}