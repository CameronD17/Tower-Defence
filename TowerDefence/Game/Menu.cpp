//#include "Menu.h"
#include <iostream>
#include <fstream>
#include <sstream>
//
//Menu::Menu(Game &g)
//{
//	game = g;
//	engine = g.engine;
//	
//	menuDepth = 0;
//	mapSelection = 0;
//	cursorSelection = 0;
//	diffSelection = 0;
//
//	init();
//}
//
//Menu::~Menu()
//{
//
//}
//	
//void Menu::init()	// Setting up the Menu
//{
//	scoresPage = false;
//	existingGame = false;
//	engine.audio.playMusic();
//}
//
//void Menu::makeButtons()
//{
//	button settings;
//	settings.name = "Settings";
//	settings.rect.x = 10;										settings.rect.y = 10;
//	settings.rect.w = (int)(SCREEN_WIDTH * 0.3);				settings.rect.h = (int)(SCREEN_HEIGHT * 0.2);
//	buttons.push_back(settings);
//
//	button highscores;
//	highscores.name = "Highscores";
//	highscores.rect.x = (int)(SCREEN_WIDTH * 0.7) - 10;			highscores.rect.y = 10;
//	highscores.rect.w = (int)(SCREEN_WIDTH * 0.3);				highscores.rect.h = (int)(SCREEN_HEIGHT * 0.2);
//	buttons.push_back(highscores);
//
//	button towerDefenceButton;
//	towerDefenceButton.name = "TowerDefence";
//	towerDefenceButton.rect.x = 10;								towerDefenceButton.rect.y = (int)(SCREEN_HEIGHT * 0.2) + 20;
//	towerDefenceButton.rect.w = (int)(SCREEN_WIDTH * 0.5) - 15; towerDefenceButton.rect.h = (int)(SCREEN_HEIGHT * 0.4) - 20;
//	buttons.push_back(towerDefenceButton);
//
//	button tetrisButton;
//	tetrisButton.name = "Tetris";
//	tetrisButton.rect.x = (int)(SCREEN_WIDTH / 2) + 5;			tetrisButton.rect.y = (int)(SCREEN_HEIGHT * 0.2) + 20;
//	tetrisButton.rect.w = (int)(SCREEN_WIDTH * 0.5) - 15;		tetrisButton.rect.h = (int)(SCREEN_HEIGHT * 0.4) - 20;
//	buttons.push_back(tetrisButton);
//
//	button pongOutButton;
//	pongOutButton.name = "PongOut";
//	pongOutButton.rect.x = 10;									pongOutButton.rect.y = (int)(SCREEN_HEIGHT * 0.6) + 10;
//	pongOutButton.rect.w = (int)(SCREEN_WIDTH * 0.5) - 15;		pongOutButton.rect.h = (int)(SCREEN_HEIGHT * 0.4) - 20;
//	buttons.push_back(pongOutButton);
//
//	button defenderButton;
//	defenderButton.name = "Defender";
//	defenderButton.rect.x = (int)(SCREEN_WIDTH / 2) + 5;		defenderButton.rect.y = (int)(SCREEN_HEIGHT * 0.6) + 10;
//	defenderButton.rect.w = (int)(SCREEN_WIDTH * 0.5) - 15;		defenderButton.rect.h = (int)(SCREEN_HEIGHT * 0.4) - 20;
//	buttons.push_back(defenderButton);
//}
//
//int Menu::run()
//{
//	int state = getInput();
//	draw();
//	return state;
//}
//
//int Menu::getInput()
//{
//	input k = engine.interfaces.getInput();
//
//	// Input is a keypress
//	if (k.keyPress)
//	{
//		if (k.key == SDLK_m)			// Pause the music
//		{
//			engine.audio.pauseMusic();
//		}
//		else if (k.key == SDLK_ESCAPE)	// Quit the game / move up a menu level
//		{
//			if (menuDepth == 0)	return -1;
//			else menuDepth--;
//		}
//		else
//		{
//			if (menuDepth == 0)
//			{
//				return mainMenuInput(k.key);
//			}
//			else
//			{
//				return subMenuInput(k.key);
//			}
//		}
//	}
//
//	// Input is a mouse click
//	else if (k.mouseDown)
//	{
//		int selection = 0;
//		for (vector<button>::iterator i = buttons.begin(); i != buttons.end(); ++i)
//		{
//			selection++;
//			if (engine.physics.checkPosition(*i))			return selection;
//		}
//	}
//
//	// Input is mouse movement
//	else
//	{
//		for (vector<button>::iterator i = buttons.begin(); i != buttons.end(); ++i)
//		{
//			if (engine.physics.checkPosition(*i))	i->selected = true;
//			else									i->selected = false;
//		}
//	}
//
//	return 0;
//}
//
//int Menu::mainMenuInput(SDL_Keycode k)
//{
//	switch(k)
//	{ 
//	case SDLK_ESCAPE:		// Exit Game
//		return -1;
//	case SDLK_RETURN:		// Do whatever the cursor points to
//		switch(cursorSelection)
//		{
//		case 0:				// New Game
//			menuDepth++;
//			return 1;
//		case 1:				// Resume Game
//			return 2;
//		case 2:				// Highscores
//			return 3;
//		case 3:				// Instructions
//			return 4;
//		case 4:				// Exit Game
//			return -1;
//		default:			// Do nothing (shouldn't really ever be triggered)
//			return 0;
//		}
//		break;
//
//	case SDLK_DOWN:			// Move cursor down
//		if(cursorSelection < 4) {
//			cursorSelection++;							// Move down to the next item on the menu
//			if(cursorSelection == 1 && !existingGame)	// If there isn't a current game, skip Resume	
//				cursorSelection++;
//		} else cursorSelection = 0;						// If we reach the bottom, go back to the top
//		break;
//
//	case SDLK_UP:			// Move cursor up
//		if(cursorSelection > 0) {
//			cursorSelection--;							// Move up to the previous item on the menu
//			if(cursorSelection == 1 && !existingGame)	// If there isn't a current game, skip Resume
//				cursorSelection--;
//		} else cursorSelection = 4;						// If we reach the top, go back to the bottom
//		break;
//
//	case SDLK_RIGHT:		// Move cursor right
//		if(cursorSelection == 3)	// If the cursor is on "Minigame"					
//			cursorSelection++;			// Move across to "Quit"
//		break;
//
//	case SDLK_LEFT:			// Move cursor left
//		if(cursorSelection == 4)	// If the cursor is on "Quit"					
//			cursorSelection--;			// Move across to "Minigame"
//		break;
//		
//	default:
//		if(cursorSelection == 1 && !existingGame) cursorSelection = 0;
//		break;
//	}
//}
//
//int Menu::subMenuInput(SDL_Keycode k)
//{
//	if(menuDepth == 1)
//	{
//		switch(k)
//		{
//		case SDLK_DOWN:			// Move cursor down
//			if(mapSelection < 3) 
//				mapSelection++;
//			break;
//
//		case SDLK_UP:			// Move cursor up
//			if(mapSelection > 0) 
//				mapSelection--;	
//			break;
//
//		case SDLK_RETURN:		
//			menuDepth++;
//			break;
//
//		default:
//			break;	
//		}
//	}
//	else if(menuDepth == 2)
//	{
//		switch(k)
//		{
//		case SDLK_DOWN:			// Move cursor down
//			if(diffSelection < 3) 
//				diffSelection++;
//			break;
//
//		case SDLK_UP:			// Move cursor up
//			if(diffSelection > 0) 
//				diffSelection--;	
//			break;
//
//		case SDLK_RETURN:
//			return 2;
//
//		default:
//			break;	
//		}
//	}
//}
//
//int Menu::getMap() 
//{
//	return mapSelection;
//}
//
//int Menu::getDiff() 
//{
//	return diffSelection;
//}
//
//void Menu::draw()
//{
//	engine.graphics.clear();		// Clear the graphics
//	
//	drawGenerics();
//
//	if(scoresPage)				drawScores();			// Draw the scores 		
//	else												// Draw the current menu
//	{
//		switch(menuDepth)
//		{
//		case 1:
//			drawMapSubmenu();
//			break;
//		case 2:
//			drawDiffSubmenu();
//			break;
//		default:
//			drawMainMenu();
//			break;
//		}
//	}
//
//	engine.graphics.update();		// Render the graphics
//}
//
//void Menu::drawGenerics()
//{
//	// Draw the logo
//	engine.graphics.renderImage(40, 30, "Game/Images/logo.png");
//	// Draw the speakers
//	if(engine.audio.music)	engine.graphics.renderImage(750, 590, "Game/Images/speakerOn.png");
//	else					engine.graphics.renderImage(750, 590, "Game/Images/speakerOff.png");
//}
//
//void Menu::drawMainMenu()
//{
//	// Draw the menu options. The current selection is coloured green. If no game has been started, then Resume Game is greyed out
//	if(cursorSelection == 0)		engine.graphics.renderText(300, 200, "New Game", 40, 34, 139, 34);
//	else							engine.graphics.renderText(300, 200, "New Game", 40);
//	
//	if(cursorSelection == 1)		engine.graphics.renderText(250, 300, "Resume Game", 40, 34, 139, 34);
//	else if(existingGame)			engine.graphics.renderText(250, 300, "Resume Game", 40);
//	else							engine.graphics.renderText(250, 300, "Resume Game", 40, 169, 169, 169);
//
//	if(cursorSelection == 2)		engine.graphics.renderText(275, 400, "Highscores", 40, 34, 139, 34);
//	else							engine.graphics.renderText(275, 400, "Highscores", 40);
//
//	if(cursorSelection == 3)		engine.graphics.renderText(150, 500, "Play Minigame", 30, 34, 139, 34);	
//	else							engine.graphics.renderText(150, 500, "Play Minigame", 30);
//
//	if(cursorSelection == 4)		engine.graphics.renderText(550, 500, "Quit", 30, 34, 139, 34);	
//	else							engine.graphics.renderText(550, 500, "Quit", 30);
//
//	// Draw the cursor at the corresponding position
//	switch(cursorSelection)
//	{
//	case 0:
//		engine.graphics.drawRectangle(200, 200, 30, 30, 255, 255, 255);
//		break;
//	case 1:
//		engine.graphics.drawRectangle(200, 300, 30, 30, 255, 255, 255);
//		break;
//	case 2:
//		engine.graphics.drawRectangle(200, 400, 30, 30, 255, 255, 255);
//		break;
//	case 3:
//		engine.graphics.drawRectangle(50, 500, 30, 30, 255, 255, 255);
//		break;
//	case 4:
//		engine.graphics.drawRectangle(450, 500, 30, 30, 255, 255, 255);
//		break;
//	}
//
//}
//
//void Menu::drawScores()
//{
//	string initText;
//    int scoreInt;
//	int xR = 100, xI = 300, xS = 550, y = 150, i = 0;
//
//	ifstream scores ("Game/highscores.txt", ios::in);
//
//	engine.graphics.renderText(xR, y, "Rank", 40);
//	engine.graphics.renderText(xI, y, "Name", 40);
//	engine.graphics.renderText(xS, y, "Score", 40);
//	engine.graphics.drawRectangle(xR, y+40, 600, 2, 255, 255, 255);
//
//	if (scores.is_open())
//	{
//		while(scores >> initText >> scoreInt)
//		{
//			// Move to the next row
//			y+=60; i++;					
//
//			// Convert integers to strings
//			stringstream sInt;
//			sInt << scoreInt;
//			stringstream rank;
//			rank << i;
//
//			// Draw scores. 1st = Gold, 2nd = Silver, 3rd = Bronze, 4th and below are white
//			switch(i)
//			{
//			case 1:
//				engine.graphics.renderText(xR, y, rank.str(), 20, 255, 215, 0);
//				engine.graphics.renderText(xI, y, initText, 20, 255, 215, 0);
//				engine.graphics.renderText(xS, y, sInt.str(), 20, 255, 215, 0);
//				break;
//			case 2:
//				engine.graphics.renderText(xR, y, rank.str(), 20, 192, 192, 192);
//				engine.graphics.renderText(xI, y, initText, 20, 192, 192, 192);
//				engine.graphics.renderText(xS, y, sInt.str(), 20, 192, 192, 192);
//				break;
//			case 3:
//				engine.graphics.renderText(xR, y, rank.str(), 20, 160, 82, 45);
//				engine.graphics.renderText(xI, y, initText, 20, 160, 82, 45);
//				engine.graphics.renderText(xS, y, sInt.str(), 20, 160, 82, 45);
//				break;
//			default:
//				engine.graphics.renderText(xR, y, rank.str(), 20);
//				engine.graphics.renderText(xI, y, initText, 20);
//				engine.graphics.renderText(xS, y, sInt.str(), 20);
//				break;
//			}
//		}
//		scores.close();
//	}
//}
//
//void Menu::drawMapSubmenu()
//{	
//	// Draw the menu title
//	engine.graphics.renderText(225, 120, "Choose Map", 40);
//	
//	// Draw the list of menu options. The current selection is coloured green. 
//	if(mapSelection == 0)	engine.graphics.renderText(50, 200, "Custom", 30, 34, 139, 34);
//	else					engine.graphics.renderText(50, 200, "Custom", 30);
//
//	if(mapSelection == 1)	engine.graphics.renderText(50, 300, "Grassland", 30, 34, 139, 34);
//	else					engine.graphics.renderText(50, 300, "Grassland", 30);
//
//	if(mapSelection == 2)	engine.graphics.renderText(50, 400, "Desert", 30, 34, 139, 34);
//	else					engine.graphics.renderText(50, 400, "Desert", 30);
//
//	if(mapSelection == 3)	engine.graphics.renderText(50, 500, "Tundra", 30, 34, 139, 34);
//	else					engine.graphics.renderText(50, 500, "Tundra", 30);
//
//	// Draw the cursor
//	switch(mapSelection)
//	{
//	case 0:		// Custom
//		engine.graphics.drawRectangle(10, 200, 20, 20, 255, 255, 255);
//		break;
//	case 1:		// Grassland
//		engine.graphics.drawRectangle(10, 300, 20, 20, 255, 255, 255);
//		break;
//	case 2:		// Desert
//		engine.graphics.drawRectangle(10, 400, 20, 20, 255, 255, 255);
//		break;
//	case 3:		// Tundra
//		engine.graphics.drawRectangle(10, 500, 20, 20, 255, 255, 255);
//		break;
//	}
//
//	// Draw the map sample
//	engine.graphics.drawRectangleOL(349, 174, 362, 402, 255, 255, 255);
//	switch(mapSelection)
//	{
//	case 0:		// Custom
//		engine.graphics.drawRectangle(350, 175, 360, 400, 105, 105, 105);
//		engine.graphics.renderText(450, 300, "?", 250);
//		break;
//	case 1:		// Grassland
//		engine.graphics.renderImage(350, 175, "Game/Images/Maps/grassmapsample.png");
//		break;
//	case 2:		// Desert
//		engine.graphics.renderImage(350, 175, "Game/Images/Maps/desertmapsample.png");
//		break;
//	case 3:		// Tundra
//		engine.graphics.renderImage(350, 175, "Game/Images/Maps/tundramapsample.png");
//		break;
//	}
//}
//
//void Menu::drawDiffSubmenu()
//{
//	// Draw the menu title
//	engine.graphics.renderText(175, 120, "Choose Difficulty", 40);
//	
//	// Draw the list of menu options. The current selection is coloured green. 
//	if(diffSelection == 0)	engine.graphics.renderText(350, 200, "Easy", 30, 34, 139, 34);
//	else					engine.graphics.renderText(350, 200, "Easy", 30);
//
//	if(diffSelection == 1)	engine.graphics.renderText(335, 300, "Medium", 30, 34, 139, 34);
//	else					engine.graphics.renderText(335, 300, "Medium", 30);
//
//	if(diffSelection == 2)	engine.graphics.renderText(350, 400, "Hard", 30, 34, 139, 34);
//	else					engine.graphics.renderText(350, 400, "Hard", 30);
//
//	if(diffSelection == 3)	engine.graphics.renderText(330, 500, "Expert", 30, 34, 139, 34);
//	else					engine.graphics.renderText(330, 500, "Expert", 30);
//
//	// Draw the cursor
//	switch(diffSelection)
//	{
//	case 0:		// Easy
//		engine.graphics.drawRectangle(300, 200, 20, 20, 255, 255, 255);
//		break;
//	case 1:		// Medium
//		engine.graphics.drawRectangle(300, 300, 20, 20, 255, 255, 255);
//		break;
//	case 2:		// Hard
//		engine.graphics.drawRectangle(300, 400, 20, 20, 255, 255, 255);
//		break;
//	case 3:		// Expert
//		engine.graphics.drawRectangle(300, 500, 20, 20, 255, 255, 255);
//		break;
//	}
//}

#include "Menu.h"

// *** SETUP *** // 

Menu::Menu(Game &e)
{
	engine = e.engine;
	init();
}

Menu::~Menu(){}

void Menu::init()
{
	makeButtons();
	menuDepth = 0;
}

void Menu::makeButtons()
{
	button settings;
	settings.name = "Settings";
	settings.rect.x = 10;										settings.rect.y = 10;
	settings.rect.w = (int)(WINDOW_WIDTH * 0.3);				settings.rect.h = (int)(WINDOW_HEIGHT * 0.2);
	buttons.push_back(settings);

	button highscores;
	highscores.name = "Highscores";
	highscores.rect.x = (int)(WINDOW_WIDTH * 0.7) - 10;			highscores.rect.y = 10;
	highscores.rect.w = (int)(WINDOW_WIDTH * 0.3);				highscores.rect.h = (int)(WINDOW_HEIGHT * 0.2);
	buttons.push_back(highscores);

	button towerDefenceButton;
	towerDefenceButton.name = "TowerDefence";
	towerDefenceButton.rect.x = 10;								towerDefenceButton.rect.y = (int)(WINDOW_HEIGHT * 0.2) + 20;
	towerDefenceButton.rect.w = (int)(WINDOW_WIDTH * 0.5) - 15; towerDefenceButton.rect.h = (int)(WINDOW_HEIGHT * 0.4) - 20;
	buttons.push_back(towerDefenceButton);

	button tetrisButton;
	tetrisButton.name = "Tetris";
	tetrisButton.rect.x = (int)(WINDOW_WIDTH / 2) + 5;			tetrisButton.rect.y = (int)(WINDOW_HEIGHT * 0.2) + 20;
	tetrisButton.rect.w = (int)(WINDOW_WIDTH * 0.5) - 15;		tetrisButton.rect.h = (int)(WINDOW_HEIGHT * 0.4) - 20;
	buttons.push_back(tetrisButton);

	button pongOutButton;
	pongOutButton.name = "PongOut";
	pongOutButton.rect.x = 10;									pongOutButton.rect.y = (int)(WINDOW_HEIGHT * 0.6) + 10;
	pongOutButton.rect.w = (int)(WINDOW_WIDTH * 0.5) - 15;		pongOutButton.rect.h = (int)(WINDOW_HEIGHT * 0.4) - 20;
	buttons.push_back(pongOutButton);

	button defenderButton;
	defenderButton.name = "Defender";
	defenderButton.rect.x = (int)(WINDOW_WIDTH / 2) + 5;		defenderButton.rect.y = (int)(WINDOW_HEIGHT * 0.6) + 10;
	defenderButton.rect.w = (int)(WINDOW_WIDTH * 0.5) - 15;		defenderButton.rect.h = (int)(WINDOW_HEIGHT * 0.4) - 20;
	buttons.push_back(defenderButton);
}


// *** RUN *** //

int Menu::run()
{
	int state = getInput();
	int gameState = 0;

	switch (state)
	{

		// Settings
	case 1:
		break;

		// Highscores
	case 2:
		break;

		// Tower Defence
	case 3:
		//game = new TowerDefence(engine);
		break;

		// Tetris
	case 4:
		break;

		// PongOut
	case 5:
		//game = new PongOut(engine);
		break;

		// Defender
	case 6:
		//game = new Defender(engine);
		break;
	default:
		break;

	}

	draw();

	return state;
}

int Menu::getInput()
{
	// Get input
	input k = engine.interfaces.getInput();

	// Input is a keypress
	if (k.keyPress)
	{
		if (k.key == SDLK_F11)
		{
			if (SDL_GetWindowFlags(engine.graphics.window) & SDL_WINDOW_FULLSCREEN)
				SDL_SetWindowFullscreen(engine.graphics.window, 0);
			else
				SDL_SetWindowFullscreen(engine.graphics.window, SDL_WINDOW_FULLSCREEN);
		}
		else if (k.key == SDLK_ESCAPE)
		{
			return -1;
		}
		//else	return -1;
	}

	// Input is a mouse click
	else if (k.mouseDown)
	{
		int selection = 0;
		for (vector<button>::iterator i = buttons.begin(); i != buttons.end(); ++i)
		{
			selection++;
			if (engine.physics.checkPosition(*i))			return selection;
		}
	}

	// Input is mouse movement
	else
	{
		for (vector<button>::iterator i = buttons.begin(); i != buttons.end(); ++i)
		{
			if (engine.physics.checkPosition(*i))	i->selected = true;
			else									i->selected = false;
		}
	}

	return 0;
}


// *** DRAW *** //

void Menu::draw()
{
	engine.graphics.clear();		// Clear the graphics

	drawMenu();						// Draw the menu

	engine.graphics.update();		// Render the graphics
}

void Menu::drawMenu()
{
	// Draw background image
	engine.graphics.renderImage(0, 0, "Menu/Assets/Images/menuBackground.png");

	// Draw logo
	engine.graphics.renderImage((int)(WINDOW_WIDTH * 0.3) + 10, (int)(WINDOW_HEIGHT * 0.06), "Menu/Assets/Images/logo.png");

	// Draw buttons
	for (vector<button>::iterator i = buttons.begin(); i != buttons.end(); ++i)
	{
		stringstream s;
		if (i->selected)
		{
			s << "Menu/Assets/Images/" << i->name << "ButtonColour.png";
			engine.graphics.renderImage(i->rect.x, i->rect.y, s.str());
		}
		else
		{
			s << "Menu/Assets/Images/" << i->name << "ButtonGrey.png";
			engine.graphics.renderImage(i->rect.x, i->rect.y, s.str(), 130);
		}
	}
}