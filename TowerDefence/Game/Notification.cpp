#include "Notification.h"

Notification::Notification()
{
	r = g = b = 0;
	messageLines.push_back("Default Message String");
}

Notification::~Notification(){}

void Notification::setup(Engine &e)
{
	engine = e;
}

void Notification::draw()
{
	if (visible)
	{
		engine.graphics.drawRectangleOL(NOTIFICATION_X, NOTIFICATION_Y, NOTIFICATION_WIDTH, NOTIFICATION_HEIGHT, r, g, b);

		int textY = NOTIFICATION_Y + 10;
		for (std::vector<std::string>::iterator m = messageLines.begin(); m != messageLines.end(); ++m)
		{
			engine.graphics.renderText(NOTIFICATION_X + 10, textY, (*m).c_str(), 20, r, g, b, "anonymous");
			textY += LINE_HEIGHT;
		}
	}
}

void Notification::update()
{
	visible = timer > SDL_GetTicks();
}

void Notification::set(std::string m, int t)
{
	messageLines.clear();
	timer = SDL_GetTicks() + NOTIFICATION_TIMER;

	switch (t)
	{
	case ERROR:
		r = 255; g = 0; b = 0;
		break;
	case WARNING: default:
		r = 255; g = 255; b = 0;
		break;
	case SUCCESS:
		r = 0; g = 255; b = 0;
		break;
	}

	if (m.length() > MAX_LINE_LENGTH)
	{
		std::string word;
		std::stringstream ss(m);
		std::vector<std::string> words;
		while (ss >> word)	words.push_back(word);
		std::reverse(words.begin(), words.end());
		
		std::stringstream line;
		while (!words.empty())
		{
			if (line.str().length() + words.back().length() < MAX_LINE_LENGTH)
			{
				line << words.back() << " ";
				words.pop_back();
			}
			else
			{
				messageLines.push_back(line.str());
				line.str(std::string());
			}
		}
		messageLines.push_back(line.str());
	}
	else
	{
		messageLines.push_back(m);
	}
}