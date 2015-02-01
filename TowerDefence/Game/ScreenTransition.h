#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"

using namespace std;

struct ScreenTransitionPanel
{
	coordinates coords;
	int width = TRANSITION_PANEL_WIDTH;
	int height = TRANSITION_PANEL_HEIGHT;
};

class ScreenTransition 
{
private:	

	Engine engine;

	ScreenTransitionPanel leftPanel;
	ScreenTransitionPanel rightPanel;

public:	
	ScreenTransition();	
	~ScreenTransition();

	bool opening;
	bool closing;
	bool isClosed;

	void init(Engine& e);
	void draw();
	int update();

	void open();
	void close();
};
