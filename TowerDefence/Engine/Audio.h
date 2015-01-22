#pragma once
#include "ResourceManager.h"
#include <SDL_mixer.h>

class Audio
{
private:
	ResourceManager *resource;

public:	
	Audio(void);
	~Audio(void);

	bool music;

	void init(ResourceManager *r);

	void playSound(string filepath);
	void playMusic();
	bool pauseMusic();

	void close();
};
