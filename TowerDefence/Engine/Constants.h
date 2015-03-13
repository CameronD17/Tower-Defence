#pragma once

// Window information
#define WINDOW_WIDTH 1366					//1366						// 800 --> 1440
#define WINDOW_HEIGHT 768					//768						// 640 --> 900
#define SCREEN_FPS 30
#define SCREEN_TICK (1000/SCREEN_FPS)

// Application state constants
#define EXIT_APPLICATION -2
#define EXIT_CURRENT_STATE -1
#define UNCHANGED_STATE 0

// Font sizes
#define EXTRA_SMALL 10
#define SMALL 20
#define MEDIUM 30
#define LARGE 40
#define EXTRA_LARGE 50

// Board constants
#define BOARD_TILE_W 40
#define BOARD_TILE_H 30
#define TILE_SIZE 24
#define BOARD_WIDTH (BOARD_TILE_W * TILE_SIZE)
#define BOARD_HEIGHT (BOARD_TILE_H * TILE_SIZE)
#define BORDER_SIZE 24
#define	BLOCK_COUNT (BOARD_TILE_W * BOARD_TILE_H)					// 1200

// Sidebar constants
#define SIDEBAR_X (BORDER_SIZE * 2) + BOARD_WIDTH	// 1008
#define SIDEBAR_WIDTH (WINDOW_WIDTH - SIDEBAR_X)					// 358
#define STATS_Y ((BOARD_HEIGHT / 4) * 3)				// 540
#define STATS_HEIGHT (WINDOW_HEIGHT - STATS_Y)						// 288

// Notifications
#define NOTIFICATION_X 1032		
#define NOTIFICATION_Y 424		
#define NOTIFICATION_WIDTH 290		
#define NOTIFICATION_HEIGHT 96	
#define NOTIFICATION_TIMER 2000
#define ERROR -1
#define WARNING 0
#define SUCCESS 1
#define LINE_HEIGHT 20
#define MAX_LINE_LENGTH 25

// Map values
#define CLEAR_TERRAIN 'C'
#define WATER_TERRAIN 'W'
#define ROUGH_TERRAIN 'R'
#define BLOCKED_TERRAIN 'B'
#define NO_ENEMY 0
#define NO_TOWER 0

// Enemy values
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
#define ENEMY_TIMER 1000
#define ENEMY_WAVE_TIMER 5000

#define BASIC_SOLDIER 1
#define MOTORBIKE 2
#define JEEP 3
#define BASIC_TANK 4
#define SUPER_SOLDIER 5
#define SUPER_TANK 6
#define GHOST_SOLDIER 7

// Cursor values
#define TOWER_1 1
#define TOWER_1_COST 50
#define TOWER_2 2
#define TOWER_2_COST 75
#define TOWER_3 3
#define TOWER_3_COST 90
#define TOWER_4 4
#define TOWER_4_COST 60
#define TOWER_5 5
#define TOWER_5_COST 110
#define TOWER_6 6
#define TOWER_6_COST 25
#define TOWER_7 7
#define TOWER_7_COST 140
#define TOWER_8 8
#define TOWER_8_COST 175
#define TOWER_9 9
#define TOWER_9_COST 0
#define TOWER_10 10
#define TOWER_10_COST 0
#define CLEAR 0
#define LAUNCH_ENEMY 11
#define CHANGE_TARGET 12

// Pause Menu
#define PAUSE_MENU_X 483
#define PAUSE_MENU_Y 134
#define PAUSE_MENU_WIDTH 400
#define PAUSE_MENU_HEIGHT 600
#define PAUSED_STATE 1

// Main Menu submenus
#define LOAD_GAME_SUBMENU_X 204
#define LOAD_GAME_SUBMENU_Y 134
#define LOAD_GAME_SUBMENU_WIDTH 968
#define LOAD_GAME_SUBMENU_HEIGHT 500
#define LOAD_GAME_SUBMENU_INTERNAL_X 635
#define LOAD_GAME_SUBMENU_INTERNAL_Y 225
#define LOAD_GAME_SUBMENU_INTERNAL_WIDTH 520 
#define LOAD_GAME_SUBMENU_INTERNAL_HEIGHT 390
#define SAVE_GAME_PANEL_BUTTONS 6
#define EXIT_SUBMENU 1
#define MAP_1 2
#define MAP_2 8
#define MAP_3 14
#define MAP_4 20
#define MAP_5 26
#define MAP_1_NEW_GAME 3
#define MAP_2_NEW_GAME 9
#define MAP_3_NEW_GAME 15
#define MAP_4_NEW_GAME 21
#define MAP_5_NEW_GAME 27
#define MAP_1_SAVE_1 4
#define MAP_1_SAVE_2 5
#define MAP_1_SAVE_3 6
#define MAP_1_SAVE_4 7
#define MAP_2_SAVE_1 10
#define MAP_2_SAVE_2 11
#define MAP_2_SAVE_3 12
#define MAP_2_SAVE_4 13
#define MAP_3_SAVE_1 16
#define MAP_3_SAVE_2 17
#define MAP_3_SAVE_3 18
#define MAP_3_SAVE_4 19
#define MAP_4_SAVE_1 22
#define MAP_4_SAVE_2 23
#define MAP_4_SAVE_3 24
#define MAP_4_SAVE_4 25
#define MAP_5_SAVE_1 28
#define MAP_5_SAVE_2 29
#define MAP_5_SAVE_3 30
#define MAP_5_SAVE_4 31

// Transition Panel
#define TRANSITION_SCREEN_MIDPOINT (WINDOW_WIDTH / 2)					// 683
#define TRANSITION_PANEL_WIDTH  (WINDOW_WIDTH / 2)						// 683
#define TRANSITION_PANEL_HEIGHT WINDOW_HEIGHT
#define TRANSITION_PANEL_SPEED	20