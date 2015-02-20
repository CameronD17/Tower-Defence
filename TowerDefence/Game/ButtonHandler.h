#pragma once
#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Button.h"

class ButtonHandler 
{
private:
	void loadButtonsFromFile(std::string filepath);
	bool buttonSelected;
	Button* selectedButton;

public:	
	ButtonHandler();	
	Engine engine;

	void init(Engine &e, std::string filepath);

	std::vector<Button*> buttons;
	void selectButton(int id);
	void selectButton(std::string name);
	void deselectButton(int id);
	void deselectButton(std::string name);
	void deselectAllButtons();
	
	Button* getSelectedButton();
	int getSelectedButtonId();
	bool getButtonSelected();

	int getHoveredButtonId();
	std::string getHoveredButtonName();

	void setHoveredButton(Cursor &c);

	Button* findButtonByName(std::string name);
	Button* findButtonById(int i);	
};
