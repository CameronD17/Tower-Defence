#pragma once
#include "../Engine/SystemController.h"
#include "Constants.h"
#include "Game.h"

using namespace std;

class Menu 
{
private:	
	SystemController engine;	// Engine access	
	Game game;					// Game access

	void makeButtons();
	vector<button> buttons;

	// Draw methods
	void drawGenerics();
	void drawScores();
	void drawMainMenu();
	void drawMapSubmenu();
	void drawDiffSubmenu();

	// Track current cursor positions
	int cursorSelection;
	int mapSelection;
	int diffSelection;

	// Input handlers
	int	mainMenuInput(SDL_Keycode k);
	int subMenuInput(SDL_Keycode k);
	
public:
	// Constructors
    Menu(Game &g);
    ~Menu();

	// Set which page you're on
	bool scoresPage;
	bool minigame;
	bool existingGame;
	int menuDepth;

	// Get the user-selected choices
	int getMap();
	int getDiff();

	// Initialise the menu
	void init();

	// Get any input from the player
    int getInput();
	
	// Redraw
	void draw();

	int run();
};
