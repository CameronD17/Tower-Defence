#pragma once
#include "ResourceManager.h"
#include <SDL_mixer.h>

class Audio
{
private:
	ResourceManager *resource;
	bool music;

public:	
	Audio(void);
	~Audio(void);

	void init(ResourceManager *r = NULL);

	void playSound(std::string filepath);
	void playMusic();
	bool pauseMusic();

	void close();
};
