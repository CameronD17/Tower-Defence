#pragma once

// Window information
#define WINDOW_WIDTH 1366											// 800 --> 1440
#define WINDOW_HEIGHT 768											// 640 --> 900
#define SCREEN_FPS 30
#define SCREEN_TICK (1000/SCREEN_FPS)

// Board constants
#define BOARD_WIDTH 40
#define BOARD_HEIGHT 30
#define BLOCK_SIZE 24
#define BORDER_SIZE 24
#define	BLOCK_COUNT (BOARD_WIDTH * BOARD_HEIGHT)					// 1200

// Sidebar constants
#define SIDEBAR_X (BORDER_SIZE * 2) + (BOARD_WIDTH * BLOCK_SIZE)	// 1008
#define SIDEBAR_WIDTH (WINDOW_WIDTH - SIDEBAR_X)					// 358
#define STATS_Y (((BOARD_HEIGHT*BLOCK_SIZE) / 3) * 2)				// 480
#define STATS_HEIGHT (WINDOW_HEIGHT - STATS_Y)						// 288

// Map values
#define CLEAR_TERRAIN 'C'
#define WATER_TERRAIN 'W'
#define ROUGH_TERRAIN 'R'
#define BLOCKED_TERRAIN 'B'
#define HAS_TOWER 'T'
#define HAS_ENEMY 'E'

// Enemy constants
#define UP 1
#define UP_RIGHT 2
#define RIGHT 3
#define DOWN_RIGHT 4
#define DOWN 5
#define DOWN_LEFT 6
#define LEFT 7
#define UP_LEFT 8
#define DIAGONAL_COST 14
#define ORTHOGONAL_COST 10
#define BOUNTY_MULTIPLIER 10
#define VALUE_MULTIPLIER 10
#define HEALTH_MULTIPLIER 10

// Sidebar buttons
#define EXIT_BUTTON 16

#define VAL(str) #str
#define TOSTRING(str) VAL(str)