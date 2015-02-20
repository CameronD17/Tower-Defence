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