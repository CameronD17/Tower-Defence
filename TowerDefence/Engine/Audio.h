#pragma once
#include "ResourceManager.h"
#include <SDL_mixer.h>


/*-------------------------------------------------------- *
 * AUDIO
 * This class will deal with all audio and sound effect
 * within the game
 *-------------------------------------------------------- */

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
