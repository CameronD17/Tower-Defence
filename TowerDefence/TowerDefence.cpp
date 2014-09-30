#include "Engine\SystemController.h"
#include "Game\Game.h"
#include "Game\Menu.h"
#include "Game\Pong\Pong.h"
#include <stdlib.h>
#include <crtdbg.h>

#define CRTDBG_MAP_ALLOC

/*	
 *	There are three main components of this tower defence game - the engine, the menu, and the game itself
 *	The menu gives the player five options - start a new game, resume an existing game, view the highscores, play a minigame (Pong) and quit
 */

int main(int argc, char* args[])
{
	SystemController sc;	// Instantiate a new engine
	sc.init();				// Initialise everything engine-related (graphics, resource-management, etc.)
	
	Game g(sc);				// Initialise a new game using the new engine
	Pong p(sc);				// Initialise a new Pong mini-game using the new engine
	Menu m(g);				// Initialise a new menu using the new game

	int menuState = 0;		// To check if the player selected a menu action
	int subMenuState = 0;	// To check if the player is in a submenu or highscores page
	int gameState = 0;		// To check if the player has quit or paused the game
	
	// ** MAIN MENU LOOP ** //
	while(menuState != -1)
	{
		menuState = m.getInput();	// Get any input from the player. If a menu item is selected, then this is actioned.

		switch(menuState) {

		// ** NEW GAME ** //
		case 1:			
			while(subMenuState != 2 && m.menuDepth > 0)	subMenuState = m.run();
			g.newGame(m.getMap(), m.getDiff());	
			m.menuDepth = subMenuState = gameState = 0;

		// ** RESUME GAME ** //
		case 2:	
			m.existingGame = true;			// Allow the player to resume a game
			while(gameState != -1)			// Main game loop, which runs until the player quits or loses			
			{	
				gameState = g.run();

				if(g.gameOver)	{			// Check if the player has lost :(				
					while(g.over() == 1) g.draw();	// Clear the screen and redraw until the player continues
					m.existingGame = false;			// Remove the ability to resume the game
				}
			}

			if(!g.gameOver) break;

		// ** HIGHSCORES ** //
		case 3:				
			m.scoresPage = true;	// Change the display to the scores page instead of the menu page		
			while(subMenuState != -1)		subMenuState = m.run();	
			m.scoresPage = false;	// Return the display to the menu page
			break;

		// ** PONG ** //
		case 4:	
			p.init();			
			while(gameState != -1)		gameState = p.run(); 
			break;

		default:
			break;
		}

		m.menuDepth = subMenuState = gameState = 0;	
		m.draw();						// Clear the screen and redraw the updates
	}

	sc.close();					// Finish the game and clean up objects	
	return 0;
	_CrtDumpMemoryLeaks();
}