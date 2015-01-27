#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Button.h"

using namespace std;

class ButtonHandler 
{
	
private:
	void loadButtonsFromFile(string filepath);

	bool buttonSelected;
	Button* selectedButton;


public:	
	ButtonHandler();	
	Engine engine;

	void init(Engine &e, string filepath);

	vector<Button*> buttons;
	void selectButton(int id);

	//vector<Button*> getButtons();
	Button* getSelectedButton();
	bool getButtonSelected();
	int getHoveredButtonId();

	void setHoveredButton(Cursor &c);

	void deselectButton(int id);
	void deselectAllButtons();

	Button* findButtonByName(string name);
	
};
