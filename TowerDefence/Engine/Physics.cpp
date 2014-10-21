#include "Physics.h"
#include <iostream>
#include <math.h>

Physics::Physics(void)
{
}

Physics::~Physics(void)
{
}

bool Physics::move(Object *o, int dir, int amt)
{
	switch(dir)
	{
		case UP: 			
			if(o->getY() >= BORDER) 
			{
				o->setY(o->getY() - amt);	
				return true;
			}
            break;

        case UPRIGHT: 
			if((o->getY() >= BORDER) && (o->getY() <= (BOARD_WIDTH * BLOCK_SIZE) + BORDER))
			{
				o->setY(o->getY() - amt);
				o->setX(o->getX() + amt);
				return true;
			}
            break;

        case RIGHT:
			if(o->getX() + amt < BOARD_WIDTH * BLOCK_SIZE)
			{
				o->setX(o->getX() + amt);
				return true;
			}
            break;

        case DOWNRIGHT:
			if((o->getY() < (BOARD_HEIGHT * BLOCK_SIZE) + BORDER) && (o->getX() <= (BOARD_WIDTH * BLOCK_SIZE) + BORDER))
			{
				o->setX(o->getX() + amt);
				o->setY(o->getY() + amt);
				return true;
			}
            break;

		case DOWN:	
			if(o->getY() + amt < BOARD_HEIGHT * BLOCK_SIZE)
			{
				o->setY(o->getY() + amt);	
				return true;
			}
            break;

        case DOWNLEFT:
			if((o->getY() < (BOARD_HEIGHT * BLOCK_SIZE) + BORDER) && (o->getX() >= BORDER))
			{
				o->setY(o->getY() + amt);
				o->setX(o->getX() - amt);
				return true;
			}
            break;

        case LEFT:
			if(o->getX() >= BORDER)
			{
				o->setX(o->getX() - amt);
				return true;
			}
            break;

        case UPLEFT:
			if((o->getX() >= BORDER) && (o->getY() >= BORDER))
			{
				o->setX(o->getX() - amt);
				o->setY(o->getY() - amt);
				return true;
			}
            break;

        default:
            break;
	}	
	return false;
}

bool Physics::checkPosition(button b)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (x < b.rect.x)
	{
		return false;
	}
	else if (x > b.rect.x + b.rect.w)
	{
		return false;
	}
	else if (y < b.rect.y)
	{
		return false;
	}
	else if (y > b.rect.y + b.rect.h)
	{
		return false;
	}

	return true;
}

bool Physics::impreciseCollision(Object *a, Object *b)
{
	float aX = (float)a->getX() + BLOCK_SIZE/2;		// Makes the enemy coordinates into the
	float aY = (float)a->getY() + BLOCK_SIZE/2;		// midpoint rather than the top left corner
	float bX = (float)b->getX();
	float bY = (float)b->getY();
	
	// Distance of a line formula is (root)(((x1-x2)^2) + ((y1-y2)^2). Not using square root, as it's not efficient.
	return (((aX - bX) * (aX - bX)) + ((aY - bY) * (aY - bY))) <= BLOCK_SIZE;
}

bool Physics::preciseCollision(Object *bat, Object *ball, int angle)
{
	// If the ball is in the right side of the screen
	if(ball->getX() > WINDOW_WIDTH/2)
	{
		// Left-Right collision
		if (!(ball->getX() + BALL_SIZE + angle >= bat->getX()))
			return false; 
		if (ball->getX() > bat->getX() + PADDLE_WIDTH)
			return false;
		if (!(ball->getY() + BALL_SIZE > bat->getY() && ball->getY() <= bat->getY() + PADDLE_HEIGHT))
			return false;
	}
	else
	{
		// Right-left collision
		if (!(ball->getX() + angle <= bat->getX() + PADDLE_WIDTH))
			return false;
		if (ball->getX() < bat->getX())
			return false;
		if (!(ball->getY() + BALL_SIZE >= bat->getY() && ball->getY() <= bat->getY() + PADDLE_HEIGHT))
			return false;
	}

	return true;
}

bool Physics::checkDistance(Object *tower, Object *enemy)
{
	return (	enemy->getX() > (tower->getX() - (BORDER + BLOCK_SIZE * tower->getType()))			// Within Left boundary
			&&	enemy->getX() < (tower->getX() + BLOCK_SIZE + (BLOCK_SIZE * tower->getType()))		// Within Right boundary
			&&	enemy->getY() > (tower->getY() - (BORDER + BLOCK_SIZE * tower->getType()))			// Within Top boundary
			&&	enemy->getY() < (tower->getY() + BLOCK_SIZE + (BLOCK_SIZE * tower->getType())));	// Within Bottom boundary
}

int Physics::getDirection(Object *tower, Object *enemy)
{
	if(enemy->getY() < tower->getY())			// Enemy is above the tower
	{
		if(enemy->getX() < tower->getX())		// Enemy is also left of the tower
			return UPLEFT;
		else if(enemy->getX() > tower->getX())	// Enemy is also right of the tower
			return UPRIGHT;
		else									// Enemy is directly above the tower
			return UP;
	}	
	else if(enemy->getY() > tower->getY())		// Enemy is below the tower
	{
		if(enemy->getX() < tower->getX())		// Enemy is also left of the tower
			return DOWNLEFT;
		else if(enemy->getX() > tower->getX())	// Enemy is also right of the tower
			return DOWNRIGHT;
		else									// Enemy is directly below the tower
			return DOWN; 
	}
	else if(enemy->getX() < tower->getX())		// Enemy is directly left of the tower
		return LEFT;
	else if(enemy->getX() > tower->getX())		// Enemy is directly right of the tower
		return RIGHT;

	return 0;
}

// Derived from http://research.ncl.ac.uk/game/mastersdegree/gametechnologies/aipathfinding/main.zip 
// Author unknown - presumed to be Dr William Blewitt?

std::vector<int> Physics::aStar(int xStart, int yStart, int xGoal, int yGoal, const bool (&passable)[BOARD_WIDTH * BOARD_HEIGHT])
{
	int xAxis[4] = {-1, 0, 1, 0}; // right, down, left, up
	int yAxis[4] = {0, 1, 0, -1};	

	#define QUEUESIZE (BOARD_HEIGHT * BOARD_WIDTH)
	#define MAPSIZE (BOARD_HEIGHT * BOARD_WIDTH)

	typedef struct Node
	{
		unsigned char x;
		unsigned char y;
		unsigned char f;
	} Node;

	typedef struct Queue
	{
		Node node[QUEUESIZE];
		unsigned char queueSize;
		unsigned char tempF;		
		unsigned char pushF;
		unsigned char tempID;
	} Queue;

	typedef struct Path
	{
		Queue queue[2];
		unsigned char openNodes[MAPSIZE];
		unsigned char closedNodes[MAPSIZE];
		unsigned char directionHistory[MAPSIZE];
	} Path;

	Path path;

	int pathFound	= 0;
	int index		= 0;
	
	int stopX		= 255;
	int stopY		= 255;	
	int nextX		= 255;
	int nextY		= 255;	

	int i, j, k, l, x, y, xChange, yChange;	

	for(i = 0; i < MAPSIZE; i++)		
	{
		path.openNodes[i] = 0;
		path.closedNodes[i] = 0;
	}

	path.queue[index].queueSize = 0;		
	path.queue[1 - index].queueSize = 0;	

	Node goalNode;
	goalNode.x = xGoal;
	goalNode.y = yGoal;

	Node startNode;
	startNode.x = xStart;
	startNode.y = yStart;
	startNode.f = abs(goalNode.x - startNode.x) + abs(goalNode.y - startNode.y);
	
	path.queue[index].node[0] = startNode;
	path.queue[index].queueSize++;	
	
	path.openNodes[(startNode.x * BOARD_HEIGHT) + startNode.y] = startNode.f;

	while(path.queue[index].queueSize != 0 && pathFound == 0)
	{
		Node currentNode = path.queue[index].node[0];

		x = currentNode.x;
		y = currentNode.y;

		for(k = 0; k < path.queue[index].queueSize - 1; k++)
		{
			path.queue[index].node[k] = path.queue[index].node[k + 1];
		}

		path.queue[index].queueSize--;
		path.queue[index].node[path.queue[index].queueSize].f = 255;
		
		path.openNodes[(x * BOARD_HEIGHT) + y] = 0;
		path.closedNodes[(x * BOARD_HEIGHT) + y] = 1;

		if(x == goalNode.x && y == goalNode.y)
		{
			pathFound = 1;
			stopX = x;
			stopY = y;
		}
		else
		{
			for(i = 0; i < 4; i++)
			{
				xChange = x + xAxis[i];
				yChange = y + yAxis[i];
				
				int current = (xChange * BOARD_HEIGHT) + yChange;

				if(		xChange >= 0 &&	xChange < BOARD_WIDTH 
					&&	yChange >= 0 &&	yChange < BOARD_HEIGHT  
					&&	path.closedNodes[current] != 1
					&&	passable[current] != false)
				{
					Node nextNode;
					nextNode.x = xChange;					
					nextNode.y = yChange;					
					nextNode.f = abs(goalNode.x - nextNode.x) + abs(goalNode.y - nextNode.y);
					
					if(path.openNodes[current] == 0)
					{
						path.openNodes[current] = nextNode.f;

						if(path.queue[index].queueSize == 0)
						{
							path.queue[index].node[0].x = nextNode.x;
							path.queue[index].node[0].y = nextNode.y;
							path.queue[index].node[0].f = nextNode.f;
							path.queue[index].queueSize++;
						}
						else
						{
							path.queue[index].pushF = nextNode.f;
							path.queue[index].tempID = 0;
							path.queue[index].tempF = path.queue[index].node[path.queue[index].tempID].f;
							
							while(path.queue[index].tempF < path.queue[index].pushF)
							{
								path.queue[index].tempID++;
								path.queue[index].tempF = path.queue[index].node[path.queue[index].tempID].f;
							}
							
							for(l = path.queue[index].queueSize; l > path.queue[index].tempID; l--)
							{
								path.queue[index].node[l].x = path.queue[index].node[l-1].x;
								path.queue[index].node[l].y = path.queue[index].node[l-1].y;
								path.queue[index].node[l].f = path.queue[index].node[l-1].f;
							}
							path.queue[index].node[path.queue[index].tempID].x = nextNode.x;
							path.queue[index].node[path.queue[index].tempID].y = nextNode.y;
							path.queue[index].node[path.queue[index].tempID].f = nextNode.f;
							
							path.queue[index].queueSize++;
						}

						path.directionHistory[current] = (i+2)%4;						
					}					
					else if(path.openNodes[current] > nextNode.f)	
					{
						path.openNodes[current] = nextNode.f;

						path.directionHistory[current] = (i+2)%4;
						
						while(!(xChange == path.queue[index].node[0].x && yChange == path.queue[index].node[0].y))
						{
							if(path.queue[1 - index].queueSize < 1)
							{
								path.queue[1 - index].node[0].x = path.queue[index].node[0].x;
								path.queue[1 - index].node[0].y = path.queue[index].node[0].y;
								path.queue[1 - index].node[0].f = path.queue[index].node[0].f;
								path.queue[1 - index].queueSize++;
							}
							else
							{
								path.queue[1 - index].pushF = path.queue[index].node[0].f;
								path.queue[1 - index].tempID = 0;
								path.queue[1 - index].tempF = path.queue[1 - index].node[path.queue[1 - index].tempID].f;
								
								while(path.queue[1 - index].tempF < path.queue[1 - index].pushF)
								{
									path.queue[1 - index].tempID++;
									path.queue[1 - index].tempF = path.queue[1 - index].node[path.queue[1 - index].tempID].f;
								}
								
								for(l = path.queue[1 - index].queueSize; l > path.queue[1 - index].tempID; l--)
								{
									path.queue[1 - index].node[l].x = path.queue[1 - index].node[l-1].x;
									path.queue[1 - index].node[l].y = path.queue[1 - index].node[l-1].y;
									path.queue[1 - index].node[l].f = path.queue[1 - index].node[l-1].f;								
								}
								
								path.queue[1 - index].node[path.queue[1 - index].tempID].x = path.queue[index].node[0].x;
								path.queue[1 - index].node[path.queue[1 - index].tempID].y = path.queue[index].node[0].y;
								path.queue[1 - index].node[path.queue[1 - index].tempID].f = path.queue[index].node[0].f;
								
								path.queue[1 - index].queueSize++;
							}
							
							for( k = 0; k < path.queue[index].queueSize - 1; k++)
							{
								path.queue[index].node[k] = path.queue[index].node[k + 1];
							}
							path.queue[index].queueSize--;
							path.queue[index].node[path.queue[index].queueSize].f = 255;
						}
						
						for( k = 0; k < path.queue[index].queueSize - 1; k++)
						{
							path.queue[index].node[k] = path.queue[index].node[k + 1];
						}
						path.queue[index].queueSize--;
						path.queue[index].node[path.queue[index].queueSize].f = 255;
					
						if(path.queue[index].queueSize > path.queue[1 - index].queueSize)
						{
							index = 1 - index;
												
							while(path.queue[index].queueSize != 0)
							{
								if(path.queue[1 - index].queueSize < 1)
								{
									path.queue[1 - index].node[0].x = path.queue[index].node[0].x;
									path.queue[1 - index].node[0].y = path.queue[index].node[0].y;
									path.queue[1 - index].node[0].f = path.queue[index].node[0].f;
									path.queue[1 - index].queueSize++;
								}
								else
								{
									path.queue[1 - index].pushF = path.queue[index].node[0].f;
									path.queue[1 - index].tempID = 0;
									path.queue[1 - index].tempF = path.queue[1 - index].node[path.queue[1 - index].tempID].f;
								
									while(path.queue[1 - index].tempF < path.queue[1 - index].pushF)
									{
										path.queue[1 - index].tempID++;
										path.queue[1 - index].tempF = path.queue[1 - index].node[path.queue[1 - index].tempID].f;
									}
								
									for(l = path.queue[1 - index].queueSize; l > path.queue[1 - index].tempID; l--)
									{
										path.queue[1 - index].node[l].x = path.queue[1 - index].node[l-1].x;
										path.queue[1 - index].node[l].y = path.queue[1 - index].node[l-1].y;
										path.queue[1 - index].node[l].f = path.queue[1 - index].node[l-1].f;								
									}
								
									path.queue[1 - index].node[path.queue[1 - index].tempID].x = path.queue[index].node[0].x;
									path.queue[1 - index].node[path.queue[1 - index].tempID].y = path.queue[index].node[0].y;
									path.queue[1 - index].node[path.queue[1 - index].tempID].f = path.queue[index].node[0].f;
								
									path.queue[1 - index].queueSize++;
								}
							
								for( k = 0; k < path.queue[index].queueSize - 1; k++)
								{
									path.queue[index].node[k] = path.queue[index].node[k + 1];
								}
								path.queue[index].queueSize--;
								path.queue[index].node[path.queue[index].queueSize].f = 255;
							}
						
						
							index = 1 - index;
						
							if(path.queue[index].queueSize == 0)
							{
								path.queue[index].node[0].x = nextNode.x;
								path.queue[index].node[0].y = nextNode.y;
								path.queue[index].node[0].f = nextNode.f;
								path.queue[index].queueSize++;
							}
							else
							{
								path.queue[index].pushF = nextNode.f;
								path.queue[index].tempID = 0;
								path.queue[index].tempF = path.queue[index].node[path.queue[index].tempID].f;
							
								while(path.queue[index].tempF < path.queue[index].pushF)
								{
									path.queue[index].tempID++;
									path.queue[index].tempF = path.queue[index].node[path.queue[index].tempID].f;
								}
							
								for(l = path.queue[index].queueSize; l > path.queue[index].tempID; l--)
								{
									path.queue[index].node[l].x = path.queue[index].node[l-1].x;
									path.queue[index].node[l].y = path.queue[index].node[l-1].y;
									path.queue[index].node[l].f = path.queue[index].node[l-1].f;
								}
								path.queue[index].node[path.queue[index].tempID].x = nextNode.x;
								path.queue[index].node[path.queue[index].tempID].y = nextNode.y;
								path.queue[index].node[path.queue[index].tempID].f = nextNode.f;
							
								path.queue[index].queueSize++;
							}
						}											
					}
				}
			}
		}
	}

	std::vector<int> finalPath;
	if(pathFound == 1)
	{
		int thisX = stopX;
		int thisY = stopY;
		while(!(thisX == startNode.x && thisY == startNode.y))
		{
			j = path.directionHistory[(thisX * BOARD_HEIGHT) + thisY];
			nextX = thisX;
			nextY = thisY;
			thisX = thisX + xAxis[j];
			thisY = thisY + yAxis[j];

			if(thisX > nextX)
				finalPath.push_back(LEFT);
			else if(thisX < nextX)
				finalPath.push_back(RIGHT);
			else if(thisY < nextY)
				finalPath.push_back(DOWN);			
			else if(thisY > nextY)
				finalPath.push_back(UP);
		}
	}

	return finalPath;
};
