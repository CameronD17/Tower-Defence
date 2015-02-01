#include "ScreenTransition.h"

ScreenTransition::ScreenTransition()
{
	
}

ScreenTransition::~ScreenTransition()
{
}

void ScreenTransition::init(Engine& e)
{
	engine = e;

	leftPanel.coords = { 0, 0 };
	rightPanel.coords = { TRANSITION_SCREEN_MIDPOINT, 0 };

	isClosed = true;
	opening = true;
	closing = false;
}

// *** DRAW METHODS *** //

void ScreenTransition::draw()
{
	engine.graphics.drawRectangle(leftPanel.coords.xPos, leftPanel.coords.yPos, leftPanel.width, leftPanel.height, 255, 255, 0);
	engine.graphics.drawRectangle(rightPanel.coords.xPos, rightPanel.coords.yPos, rightPanel.width, rightPanel.height, 255, 255, 0);
}

int ScreenTransition::update()
{
	int state = UNCHANGED_STATE;

	return state;
}

void ScreenTransition::open()
{
	if (isClosed)
	{
		if (leftPanel.coords.xPos > (-TRANSITION_PANEL_WIDTH) && rightPanel.coords.xPos < WINDOW_WIDTH)
		{
			leftPanel.coords.xPos -= TRANSITION_PANEL_SPEED;
			rightPanel.coords.xPos += TRANSITION_PANEL_SPEED;
		}
		else
		{
			isClosed = false;
			opening = false;
		}
	}
}

void ScreenTransition::close()
{
	if (!isClosed)
	{
		if (leftPanel.coords.xPos < 0 && rightPanel.coords.xPos > TRANSITION_SCREEN_MIDPOINT)
		{
			leftPanel.coords.xPos += TRANSITION_PANEL_SPEED;
			rightPanel.coords.xPos -= TRANSITION_PANEL_SPEED;
		}
		else
		{
			isClosed = true;
			closing = false;
		}
	}
}