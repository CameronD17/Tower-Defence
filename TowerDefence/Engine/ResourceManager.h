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

using namespace std;

class ResourceManager
{
private:
	SDL_Renderer							*renderer;

	string									imageFilepaths, 
											soundFilepaths, 
											fontFilepaths, 
											musicFilepaths;

	Mix_Music								*music;
	unordered_map<string, SDL_Texture*>		images;
	unordered_map<string, Mix_Chunk*>		sounds;
	unordered_map<string, TTF_Font*>		fonts;			

public:		
	ResourceManager();
	~ResourceManager(void);

	// Load data
	TTF_Font	*loadFont(string filepath, int size);
	Mix_Chunk	*loadSound(string filepath);
	Mix_Music	*loadMusic(string filepath);
	SDL_Surface *loadImage(string filepath);
	

	// Access data		
	TTF_Font	*getFont(string fontname,int size);
	Mix_Chunk	*getSound(string filepath);		
	SDL_Texture *getImage(string filepath);
	Mix_Music	*getMusic();

	void open(SDL_Renderer* r);
	void close();
};
