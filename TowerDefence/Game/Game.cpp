#include "Game.h"

// *** CONSTRUCTORS AND INITIALISATION METHODS *** //

Game::Game(void){}

Game::Game(Engine &sc)
{
	engine = sc;												// The game relies on an "engine" in the form of the Engine
	cursor.init(engine.resources);								// This initialises the cursor with the engine

}
	
void Game::newGame(int m, int d)								// Setting up the game
{	
	srand(time(NULL));											// Initialise randomisatio

	newLevel();													// Start a new level
}

void Game::loadMap()
{
	
}



// *** UPDATE METHODS *** //

int Game::run()
{
	int gameState = getInput();	// Get any input from the player. If the player pauses or quits, this is recorded
		
	update();					// Update the players cursor, the game board, and the bots		
	draw();						// Clear the screen and redraw the updates

	return gameState;
}

void Game::update()
{		
	cleanup();			// Remove any destroyed enemies or spent bullets, refresh any messages
}

void Game::newLevel()
{
	
}

void Game::cleanup()
{
	
}

void Game::setMessage(string m)
{

}



// *** DRAW METHODS *** //

void Game::draw()
{
	engine.graphics.clear();			// Clear the graphics
	
	drawBoardBackground();				// Draw the background to the game and non-gameplay elements
	
	drawBoardForeground();				// Draw the non-moving foreground elements

	drawGamePieces();					// Draw the entities of the game
		
	drawCursor();						// Draw the player's cursor					
		
	if(debugMode) drawDebugFeatures();	// Draw debug features (if required)
			
	engine.graphics.update();			// Render the graphics
	engine.admin.countedFrames++;		// Capture the framerate
}

void Game::drawBoardBackground()
{
	
}

void Game::drawGamePieces()
{
	

}

void Game::drawBoardForeground()
{
	Uint8 red, green, blue;
	string text = "";



	red = 154; green = 0; blue = 0; text = "Expert";
	
	// *** Draw the board sides *** // 
	// Left
	engine.graphics.drawRectangle(0, 0, BORDER, ((BORDER * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue);
	
	// Right
	engine.graphics.drawRectangle(BORDER + (BOARD_WIDTH * BLOCK_SIZE), 0, BORDER, ((BORDER * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue);

	// Top
    engine.graphics.drawRectangle(0, 0, BORDER + (BOARD_WIDTH * BLOCK_SIZE), BORDER, red, green, blue);

	// Bottom
    engine.graphics.drawRectangle(0, BORDER + (BOARD_HEIGHT*BLOCK_SIZE), BORDER + (BOARD_WIDTH * BLOCK_SIZE), BORDER, red, green, blue);

	// Outline
	engine.graphics.drawRectangleOL(BORDER,BORDER,BOARD_WIDTH*BLOCK_SIZE,BOARD_HEIGHT*BLOCK_SIZE,0,0,0);
}

void Game::drawCursor()
{	
	// *** Draw the cursor *** //
	if(cursor.getType() == 0)	// If there is no tower selected by the cursor, draw a plain white cursor. This is the default starting cursor.
	{	
		engine.graphics.drawRectangleOL((BORDER+cursor.getX()), (BORDER+cursor.getY()), BLOCK_SIZE * 2, BLOCK_SIZE * 2, cursor.r, cursor.g, cursor.b);
	}
	else
	{
		engine.graphics.drawRectangle((BORDER + cursor.getX()), (BORDER + cursor.getY()), BLOCK_SIZE * 2, BLOCK_SIZE * 2, cursor.r, cursor.g, cursor.b);
	}

	stringstream cursorText;
	int x, y;
	SDL_GetMouseState(&x, &y);
	cursorText << "Cursor: (" << cursor.getX() << "," << cursor.getY() <<")  Actual: (" << x << "," << y << ")";

	engine.graphics.renderText(10, 10, cursorText.str(), 20, 255, 255, 255);
}

void Game::drawDebugFeatures()
{
}



// *** GAMEPLAY METHODS *** //

int Game::getInput()
{
	input k = engine.interfaces.getInput();	// Get input from the user

	if (k.keyPress)
	{
		if (k.key == SDLK_ESCAPE)	// Quit the game
		{
			return -1;
		}
	}
	else if (k.mouseDown)
	{

	}
	else
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE) - BLOCK_SIZE) && y > BORDER && y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE) - BLOCK_SIZE))
		{
			cursor.setType(1);
			cursor.setX((x - (x%BLOCK_SIZE)) - BORDER);
			cursor.setY((y - (y%BLOCK_SIZE)) - BORDER);
		}
		else 
		{
			cursor.setType(0);
				(x > BORDER) ? cursor.setX((BOARD_WIDTH*BLOCK_SIZE) - BLOCK_SIZE * 2) : cursor.setX(BORDER - BLOCK_SIZE);
				(y > BORDER) ? cursor.setY((BOARD_HEIGHT*BLOCK_SIZE) - BLOCK_SIZE * 2) : cursor.setY(BORDER - BLOCK_SIZE);
		}
			
	}

    return 0;
}