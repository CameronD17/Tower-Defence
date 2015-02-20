#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include "Constants.h"

class ResourceManager
{
private:
	SDL_Renderer *renderer = NULL;

	std::string imageFilepaths, soundFilepaths, fontFilepaths, musicFilepaths;

	Mix_Music *music = NULL;
	std::unordered_map<std::string, SDL_Texture*>	images;
	std::unordered_map<std::string, Mix_Chunk*>		sounds;
	std::unordered_map<std::string, TTF_Font*>		fonts;

public:		
	ResourceManager();
	~ResourceManager(void);

	// Load data
	TTF_Font	*loadFont(std::string filepath, int size);
	Mix_Chunk	*loadSound(std::string filepath);
	Mix_Music	*loadMusic(std::string filepath);
	SDL_Surface *loadImage(std::string filepath);
	

	// Access data		
	TTF_Font	*getFont(std::string fontname, int size);
	Mix_Chunk	*getSound(std::string filepath);
	SDL_Texture *getImage(std::string filepath);
	Mix_Music	*getMusic();

	void open(SDL_Renderer *r = NULL);
	void close();
};
