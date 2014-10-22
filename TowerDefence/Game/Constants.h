#pragma once

#define WINDOW_WIDTH 1366		// 800 --> 1440
#define WINDOW_HEIGHT 768		// 640 --> 900

#define BOARD_WIDTH 40
#define BOARD_HEIGHT 30
#define	BLOCK_COUNT (BOARD_WIDTH * BOARD_HEIGHT)

#define BLOCK_SIZE 24
#define BULLET_SIZE 2
#define BORDER 24

#define SCREEN_FPS 30
#define SCREEN_TICK (1000/SCREEN_FPS)

#define UP 1
#define UPRIGHT 2
#define RIGHT 3
#define DOWNRIGHT 4
#define DOWN 5
#define DOWNLEFT 6
#define LEFT 7
#define UPLEFT 8

#define CLEARTERRAIN 1
#define WATERTERRAIN 2
#define ROUGHTERRAIN 3
#define BLOCKEDTERRAIN 4
#define DIAGONALCOST 14
#define ORTHOGONALCOST 10

//#define UP 1
//#define UPRIGHT 2
//#define RIGHT 3
//#define DOWNRIGHT 4
//#define DOWN 5
//#define DOWNLEFT 6
//#define LEFT 7
//#define UPLEFT 8
//#define SCREEN_FPS 30
//#define SCREEN_TICK (1000/SCREEN_FPS)