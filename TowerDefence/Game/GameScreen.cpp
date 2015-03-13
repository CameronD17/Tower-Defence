#include "GameScreen.h"

GameScreen::GameScreen(){}

GameScreen::~GameScreen(){}

void GameScreen::init(Engine &e, Cursor &c)
{
	engine = e;
	cursor = c;
}

void GameScreen::loadGame(std::string game)
{
	notification.setup(engine);
	board.setup(engine, game);
	sidebar.setup(engine);
	pauseMenu.setup(engine, cursor);
	transition.init(engine);
	engine.admin.start();
}

void GameScreen::draw()
{
	engine.graphics.clear();

	board.draw();
	sidebar.draw();
	notification.draw();
	cursor.draw();
	pauseMenu.draw();

	engine.admin.countedFrames++;
	std::stringstream fps;
	fps << "FPS: " << (int)engine.admin.avgFPS;
	engine.graphics.renderText(24, 5, fps.str(), SMALL, 255, 255, 255);

	engine.graphics.update();
	engine.admin.updateFPS();
}

int GameScreen::getInput()
{
	input k = engine.interfaces.getInput();

	if (k.keyPress)
	{
		switch (k.key)
		{
		case SDLK_ESCAPE:
			return EXIT_CURRENT_STATE;

		case SDLK_q:
			return EXIT_APPLICATION;

		case SDLK_p:
			pauseMenu.pause();

		default:
			break;
		}
	}
	
	if (k.x > BORDER_SIZE && k.x < (BORDER_SIZE + BOARD_WIDTH) && k.y > BORDER_SIZE && k.y < (BORDER_SIZE + BOARD_HEIGHT))
	{
		board.getInput(k, cursor, notification);
	}
	else
	{
		int sidebarInput = sidebar.getInput(k, cursor, board, notification);
		if (sidebarInput == PAUSED_STATE) pauseMenu.pause();
		return sidebarInput;
	}

	return UNCHANGED_STATE;
}

int GameScreen::update()
{
	if (transition.isClosed) openTransition();

	int state = getInput();

	while (pauseMenu.paused)
	{
		state = pauseMenu.getInput();
		draw();
	}

	board.update();
	sidebar.update(board);
	notification.update();
	draw();

	if (state == EXIT_CURRENT_STATE || state == EXIT_APPLICATION) closeTransition();

	return state;
}

void GameScreen::closeTransition()
{
	transition.closing = true;
	while (transition.closing)
	{
		transition.close();

		engine.graphics.clear();
		board.draw();
		sidebar.draw();
		cursor.draw();
		pauseMenu.draw();		
		transition.draw();
		engine.graphics.update();
	}
}

void GameScreen::openTransition()
{
	transition.opening = true;
	while (transition.opening)
	{
		transition.open();

		engine.graphics.clear();
		board.draw();
		sidebar.draw();
		cursor.draw();
		transition.draw();
		engine.graphics.update();
	}
}