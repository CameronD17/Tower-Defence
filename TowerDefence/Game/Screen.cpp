#include "Screen.h"

Screen::Screen()
{
	transition.init(engine);
}

Screen::Screen(Engine& e)
{
	engine = e;
	transition.init(engine);
}

Screen::~Screen()
{
}


// *** DRAW METHODS *** //

int Screen::update()
{
	int state = UNCHANGED_STATE;


	return state;
}