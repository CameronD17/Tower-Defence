#pragma once
#include "../Engine/Engine.h"
#include "../Engine/Constants.h"

class Notification
{
private:
	Engine engine;
	int r, g, b;
	std::vector<std::string> messageLines;
	bool visible = false;
	unsigned int timer;

public:
	Notification();
	~Notification();

	void setup(Engine &e);
	void update();
	void draw();
	void set(std::string message, int notificationType);
};
