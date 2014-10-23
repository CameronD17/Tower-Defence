#include "AStar.h"

AStar::AStar(void)
{
}

AStar::~AStar(void)
{
}

vector<int> AStar::findPath(int sX, int sY, int tX, int tY, Map map, bool swim)
{
	setMapValues(map, swim);

	onOpenList = 0, onClosedList = 10, numberOfOpenListItems = 0, 
	parentXval = 0, parentYval = 0, pathLocation = 0,
	targetX = tX / BLOCK_SIZE, targetY = tY / BLOCK_SIZE, startX = sX / BLOCK_SIZE, startY = sY / BLOCK_SIZE;
	
	int m = 0, u = 0, v = 0, temp = 0,
	addedGCost = 0, tempGcost = 0, path = 0,
	newOpenListItemID = 0;

	// Step 1: check trivial cases

	if (startX == targetX && startY == targetY)
		return vector<int>(0);

	// Step 2: Clear shit up 
	//TODO: check if this can be removed
	if (onClosedList > 1000000) //reset whichList occasionally
	{
		for (int x = 0; x < BOARD_WIDTH; x++) {
			for (int y = 0; y < BOARD_HEIGHT; y++)
				whichList[x][y] = 0;
		}
		onClosedList = 10;
	}
	onClosedList = onClosedList + 2; //changing the values of onOpenList and onClosed list is faster than redimming whichList() array
	onOpenList = onClosedList - 1;
	Gcost[startX][startY] = 0; //reset starting square's G value to 0

	// Step 3: Add the starting location to the open list of squares to be checked.
	numberOfOpenListItems = 1;
	openList[1] = 1;//assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
	openX[1] = startX; openY[1] = startY;

	// Step 4: do this until a path is found
	do
	{
		// Step 5: If the open list is not empty, take the first cell off of the list.
		//	This is the lowest F cost cell on the open list.
		if (numberOfOpenListItems != 0)
		{
			// Step 6: Pop the first item off the open list.
			parentXval = openX[openList[1]];
			parentYval = openY[openList[1]]; //record cell coordinates of the item
			whichList[parentXval][parentYval] = onClosedList;//add the item to the closed list

			binaryHeap();

			// Step 7.Check the adjacent squares. (Its "children" -- these path children
			//	are similar, conceptually, to the binary heap children mentioned
			//	above, but don't confuse them. They are different. Path children
			//	are portrayed in Demo 1 with grey pointers pointing toward
			//	their parents.) Add these adjacent child squares to the open list
			//	for later consideration if appropriate (see various if statements
			//	below).
			for (int b = parentYval - 1; b <= parentYval + 1; b++)
			{
				for (int a = parentXval - 1; a <= parentXval + 1; a++)
				{
					//	If not off the map (do this first to avoid array out-of-bounds errors)
					if (a != -1 && b != -1 && a != BOARD_WIDTH+1 && b != BOARD_HEIGHT+1)
					{
						//	If not already on the closed list (items on the closed list have
						//	already been considered and can now be ignored).			
						if (whichList[a][b] != onClosedList) 
						{
							//	If not a wall/obstacle square.
							if (terrain[a][b] != BLOCKEDTERRAIN)
							{
								// If you aren't cutting a corner
								if (cutCorner(a, b))
								{
									//	If not already on the open list, add it to the open list.			
									if (whichList[a][b] != onOpenList)
									{
										//Create a new open list item in the binary heap.
										newOpenListItemID = newOpenListItemID + 1; //each new item has a unique ID #
										m = numberOfOpenListItems + 1;
										openList[m] = newOpenListItemID;//place the new open list item (actually, its ID#) at the bottom of the heap
										openX[newOpenListItemID] = a;
										openY[newOpenListItemID] = b;//record the x and y coordinates of the new item

										//Figure out its G cost
										if (abs(a - parentXval) == 1 && abs(b - parentYval) == 1)
											addedGCost = DIAGONALCOST * (terrain[a][b] + 1);//cost of going to diagonal squares	
										else
											addedGCost = ORTHOGONALCOST * (terrain[a][b] + 1);//cost of going to non-diagonal squares			

										Gcost[a][b] = Gcost[parentXval][parentYval] + addedGCost;

										//Figure out its H and F costs and parent
										Hcost[openList[m]] = ORTHOGONALCOST * (abs(a - targetX) + abs(b - targetY));
										Fcost[openList[m]] = Gcost[a][b] + Hcost[openList[m]];
										parentX[a][b] = parentXval; parentY[a][b] = parentYval;

										//Move the new open list item to the proper place in the binary heap.
										//Starting at the bottom, successively compare to parent items,
										//swapping as needed until the item finds its place in the heap
										//or bubbles all the way to the top (if it has the lowest F cost).
										while (m != 1) //While item hasn't bubbled to the top (m=1)	
										{
											//Check if child's F cost is < parent's F cost. If so, swap them.	
											if (Fcost[openList[m]] <= Fcost[openList[m / 2]])
											{
												temp = openList[m / 2];
												openList[m / 2] = openList[m];
												openList[m] = temp;
												m = m / 2;
											}
											else
												break;
										}
										numberOfOpenListItems = numberOfOpenListItems + 1;//add one to the number of items in the heap

										//Change whichList to show that the new item is on the open list.
										whichList[a][b] = onOpenList;
									}
									//	8.If adjacent cell is already on the open list, check to see if this
										//	path to that cell from the starting location is a better one. 
										//	If so, change the parent of the cell and its G and F costs.	
									else //If whichList(a,b) = onOpenList
									{
										//Figure out its G cost
										if (abs(a - parentXval) == 1 && abs(b - parentYval) == 1)
											addedGCost = DIAGONALCOST * (terrain[a][b] + 1);//cost of going to diagonal squares	
										else
											addedGCost = ORTHOGONALCOST * (terrain[a][b] + 1);//cost of going to non-diagonal squares				
										tempGcost = Gcost[parentXval][parentYval] + addedGCost;

										//If this path is shorter (G cost is lower) then change
										//the parent cell, G cost and F cost. 		
										if (tempGcost < Gcost[a][b]) //if G cost is less,
										{
											parentX[a][b] = parentXval; //change the square's parent
											parentY[a][b] = parentYval;
											Gcost[a][b] = tempGcost;//change the G cost			

											//Because changing the G cost also changes the F cost, if
											//the item is on the open list we need to change the item's
											//recorded F cost and its position on the open list to make
											//sure that we maintain a properly ordered open list.
											for (int x = 1; x <= numberOfOpenListItems; x++) //look for the item in the heap
											{
												if (openX[openList[x]] == a && openY[openList[x]] == b) //item found
												{
													Fcost[openList[x]] = Gcost[a][b] + Hcost[openList[x]];//change the F cost

													//See if changing the F score bubbles the item up from it's current location in the heap
													m = x;
													while (m != 1) //While item hasn't bubbled to the top (m=1)	
													{
														//Check if child is < parent. If so, swap them.	
														if (Fcost[openList[m]] < Fcost[openList[m / 2]])
														{
															temp = openList[m / 2];
															openList[m / 2] = openList[m];
															openList[m] = temp;
															m = m / 2;
														}
														else
															break;
													}
													break; //exit for x = loop
												} //If openX(openList(x)) = a
											} //For x = 1 To numberOfOpenListItems
										}//If tempGcost < Gcost(a,b)

									}//else If whichList(a,b) = onOpenList									
								}
							}
						}
					}
				}
			}
		}
		else
		{
			path = NOPATH; 
			break;
		}

		//If target is added to open list then path has been found.
		if (whichList[targetX][targetY] == onOpenList)
		{
			path = PATHFOUND; 
			break;
		}
	} while (1);

	//10.Save the path if it exists.
	if (path == PATHFOUND)
	{		
		calcPath();
		return directions;
	}
	else
	{
		return vector<int>(0);
	}
}

void AStar::calcPath()
{
	int tempx = 0;
	pathX = targetX; pathY = targetY;

	do
	{
		coordinates.push_back(pathX);
		coordinates.push_back(pathY);

		calcDirections(pathX, pathY, parentX[pathX][pathY], parentY[pathX][pathY]);
		
		tempx = parentX[pathX][pathY];
		pathY = parentY[pathX][pathY];
		pathX = tempx;

	} while (pathX != startX || pathY != startY);
}

void AStar::calcDirections(int x, int y, int pX, int pY)
{
	if (pY < y)				// Previous step was north...
	{
		if (pX < x)			// ...west...
			directions.push_back(DOWNRIGHT);	// ...so we go south-east
		else if (pX > x)	// ...east
			directions.push_back(DOWNLEFT);		// ...so we go south-west
		else				// ...exactly
			directions.push_back(DOWN);			// ...so we go south
	}
	else if (pY > y)		// Previous step was south...
	{
		if (pX < x)			// ...west
			directions.push_back(UPRIGHT);		// ...so we go north-east
		else if (pX > x)	// ...east
			directions.push_back(UPLEFT);		// ...so we go north-west
		else				// ...exactly
			directions.push_back(UP);			// ...so we go north
	}
	else if (pX < x)		// Previous step was west
		directions.push_back(RIGHT);			// ...so we go east
	else if (pX > x)		// Previous step was east
		directions.push_back(LEFT);				// ...so we go west
}

void AStar::binaryHeap()
{
	//Open List = Binary Heap : Delete this item from the open list, which
	//  is maintained as a binary heap. For more information on binary heaps, see:
	//	http://www.policyalmanac.org/games/binaryHeaps.htm

	numberOfOpenListItems = numberOfOpenListItems - 1;//reduce number of open list items by 1	

	//	Delete the top item in binary heap and reorder the heap, with the lowest F cost item rising to the top.
	openList[1] = openList[numberOfOpenListItems + 1];//move the last item in the heap up to slot #1
	int v = 1;

	//	Repeat the following until the new item in slot #1 sinks to its proper spot in the heap.
	do
	{
		int u = v;
		if (2 * u + 1 <= numberOfOpenListItems) //if both children exist
		{
			//Check if the F cost of the parent is greater than each child.
			//Select the lowest of the two children.
			if (Fcost[openList[u]] >= Fcost[openList[2 * u]])
				v = 2 * u;
			if (Fcost[openList[v]] >= Fcost[openList[2 * u + 1]])
				v = 2 * u + 1;
		}
		else
		{
			if (2 * u <= numberOfOpenListItems) //if only child #1 exists
			{
				//Check if the F cost of the parent is greater than child #1	
				if (Fcost[openList[u]] >= Fcost[openList[2 * u]])
					v = 2 * u;
			}
		}

		if (u != v) //if parent's F is > one of its children, swap them
		{
			int temp = openList[u];
			openList[u] = openList[v];
			openList[v] = temp;
		}
		else
			break; //otherwise, exit loop

	} while (1);
}

bool AStar::cutCorner(int a, int b)
{
	//	Don't cut across corners
	bool corner = true;
	if (a == parentXval - 1)
	{
		if (b == parentYval - 1)
		{
			if (	terrain[parentXval - 1][parentYval] == BLOCKEDTERRAIN
				||	terrain[parentXval][parentYval - 1] == BLOCKEDTERRAIN)
				corner = false;
		}
		else if (b == parentYval + 1) 
		{
			if (	terrain[parentXval][parentYval + 1] == BLOCKEDTERRAIN
				||	terrain[parentXval - 1][parentYval] == BLOCKEDTERRAIN)
				corner = false;
		}
	}
	else if (a == parentXval + 1)
	{
		if (b == parentYval - 1)
		{
			if (	terrain[parentXval][parentYval - 1] == BLOCKEDTERRAIN
				||	terrain[parentXval + 1][parentYval] == BLOCKEDTERRAIN)
				corner = false;
		}
		else if (b == parentYval + 1)
		{
			if (	terrain[parentXval + 1][parentYval] == BLOCKEDTERRAIN
				||	terrain[parentXval][parentYval + 1] == BLOCKEDTERRAIN)
				corner = false;
		}
	}

	return corner;
}

void AStar::setMapValues(Map map, bool swim)
{
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			terrain[x][y] = CLEARTERRAIN;
			/*if (swim)
			{
				terrain[x][y] = map.getTerrain(x, y);
			}
			else
			{
				if (map.getTerrain(x, y) == WATERTERRAIN)
				{
					terrain[x][y] == BLOCKEDTERRAIN;
				}
				else
				{
					terrain[x][y] = map.getTerrain(x, y);
				}
			}*/
		}
	}
}