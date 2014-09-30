#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/*-------------------------------------------------------- *
 * RESOURCE MANAGER
 * This class will load all items to be used by the game,
 * store them for use, and destroy when no longer required
 *-------------------------------------------------------- */

class ResourceManager
{
private:
	SDL_Renderer			*renderer;			// SDL Renderer

	Mix_Music				*music;				// Background music for the game

	vector<SDL_Texture*>	images;				// Vector of all textures used by the game
	vector<string>			imgFilePath;		// Vector of the filepaths for the images

	vector<Mix_Chunk*>		sounds;				// Vector of all sound effects used by the game
	vector<string>			soundFilePath;		// Vector of the filepaths for the sounds

	vector<TTF_Font*>		fonts;				// Vector of common fonts used by the game
			
	void preloadImages(string filepath);		// Load all images used by the game
	void preloadSounds(string filepath);		// Load all sounds and music used by the game
	void preloadFonts();						// Load in the fonts used by the game


public:		

	// Generic constructors
	ResourceManager();
	~ResourceManager(void);

	void init(SDL_Renderer* r);	

	// Load data
	TTF_Font	*loadFont(int size);
	Mix_Chunk	*loadSound(string filepath);
	Mix_Music	*loadMusic(string filepath);
	SDL_Surface *loadImage(string filepath);

	// Access data		
	TTF_Font	*getFont(int size);
	Mix_Chunk	*getSound(string filepath);		
	SDL_Texture *getImage(string filepath);
	Mix_Music	*getMusic();

	void close();
};
