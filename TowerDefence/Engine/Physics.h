#pragma once
#include "Object.h"
#include "../Game/Constants.h"

#include <vector>

/*-------------------------------------------------------- *
 * PHYSICS
 * This class will handle all physics related to the game.
 * In this specific engine, that will be mainly movement 
 * of the cursor and enemies, collision detection, A* path-
 * finding and distance-checking
 *-------------------------------------------------------- */


class Physics
{

public:	
	Physics(void);
	~Physics(void);

	// Move an object in a chosen direction (dir) by a chosen amount (amt). This allows multiple uses of the method.
	bool move(Object *o, int dir, int amt);

	bool nonUniformMove(Object* o, int xDir, int yDir);
	
	// Check for imprecise collision detection (bullets to enemies - this can just check for cheap vicinity collisions as that is all that is required here)
	bool impreciseCollision(Object *enemy, Object *bullet);

	// Check for precise collisions (surface to surface). Used in the Pong mini-game (could also be extended for use as a general collision method?)
	bool preciseCollision(Object *bat, Object *ball, int angle);

	// Check for distance between two objects (towers and objects)
	bool checkDistance(Object *tower, Object *enemy);

	// Get the direction for a tower to fire (oops, hardcoding into the engine...)
	int getDirection(Object *tower, Object *enemy);

	bool checkPosition(button b);
};
