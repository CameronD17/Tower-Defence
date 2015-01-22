#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Button.h"

using namespace std;

class SidebarButtonHandler 
{
	
private:
	void loadButtonsFromFile(string filepath);

	bool buttonSelected;
	Button* selectedButton;


public:	
	SidebarButtonHandler();	
	Engine engine;

	void init(Engine &e, string filepath);

	vector<Button*> buttons;
	void selectButton(int id);

	//vector<Button*> getButtons();
	Button* getSelectedButton();
	bool getButtonSelected();

	void deselectButton(int id);
	void deselectAllButtons();

	Button* findButtonByName(string name);
	
};
