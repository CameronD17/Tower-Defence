#include "Audio.h"

Audio::Audio(void)
{
}

Audio::~Audio(void)
{
}

void Audio::init(ResourceManager *r)
{
	resource = r;
	music = true;
}

void Audio::playSound(std::string filepath)
{
    Mix_PlayChannel(-1, resource->getSound(filepath), 0);
}

void Audio::playMusic()
{
    if(!Mix_PlayingMusic())
	{
        Mix_PlayMusic(resource->getMusic(), -1);
	}
}

bool Audio::pauseMusic()
{
	if(Mix_PausedMusic())
	{
		Mix_ResumeMusic();
		music = true;
		return true;
	}
	else
	{
		Mix_PauseMusic();
		music = false;
		return false;
	}
}
