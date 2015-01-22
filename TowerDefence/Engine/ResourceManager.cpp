#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	imageFilepaths = "Assets/Inputs/Filepaths/IMAGE_FILEPATHS.txt";
	soundFilepaths = "Assets/Inputs/Filepaths/SOUND_FILEPATHS.txt";
	fontFilepaths = "Assets/Inputs/Filepaths/FONT_FILEPATHS.txt";
	musicFilepaths = "Assets/Audio/background.mp3";
}

ResourceManager::~ResourceManager(void)
{
}


// Open and close resources

void ResourceManager::open(SDL_Renderer* r)
{
	renderer = r;

	string line;
	ifstream imageData(imageFilepaths);
	if (imageData.is_open())
	{
		while (getline(imageData, line))
		{
			images[line] = SDL_CreateTextureFromSurface(renderer, loadImage(line));
		}
		imageData.close();
	}

	ifstream soundData(soundFilepaths);
	if (soundData.is_open())
	{
		while (getline(soundData, line))
		{
			sounds[line] = loadSound(line);
		}
		soundData.close();
	}

	ifstream fontData(fontFilepaths);
	if (fontData.is_open())
	{
		while (getline(fontData, line))
		{
			stringstream linedataEXTRA_SMALL;
			linedataEXTRA_SMALL << line << EXTRA_SMALL;
			fonts[linedataEXTRA_SMALL.str()] = loadFont(line, EXTRA_SMALL);

			stringstream linedataSMALL;
			linedataSMALL << line << SMALL;
			fonts[linedataSMALL.str()] = loadFont(line, SMALL);

			stringstream linedataMEDIUM;
			linedataMEDIUM << line << MEDIUM;
			fonts[linedataMEDIUM.str()] = loadFont(line, MEDIUM);

			stringstream linedataLARGE;
			linedataLARGE << line << LARGE;
			fonts[linedataLARGE.str()] = loadFont(line, LARGE);

			stringstream linedataEXTRA_LARGE;
			linedataEXTRA_LARGE << line << EXTRA_LARGE;
			fonts[linedataEXTRA_LARGE.str()] = loadFont(line, EXTRA_LARGE);
		}
		fontData.close();
	}

	music = loadMusic(musicFilepaths);
}

void ResourceManager::close()
{
	// Clean up images
	for (auto itr = images.begin(); itr != images.end(); itr++)
	{
		if ((*itr).second != NULL)
		{
			SDL_DestroyTexture((*itr).second);
			(*itr).second = NULL;
		}
	}

	// Clean up sounds
	// Mix_FreeMusic(music);	
	for (auto itr = sounds.begin(); itr != sounds.end(); itr++)
	{
		if ((*itr).second != NULL)
		{
			Mix_FreeChunk((*itr).second);
			(*itr).second = NULL;
		}
	}

	// Clean up fonts	
	for (auto itr = fonts.begin(); itr != fonts.end(); itr++)
	{
		if ((*itr).second != NULL)
		{
			TTF_CloseFont((*itr).second);
			(*itr).second = NULL;
		}
	}
}


// Load resources

TTF_Font *ResourceManager::loadFont(string filepath, int size)
{
	TTF_Font *font = TTF_OpenFont(filepath.c_str(), size);

	if(font == NULL) printf("Failed to load font - SDL_TTF Error: %s\n", TTF_GetError());

	return font;
}

SDL_Surface *ResourceManager::loadImage(string filepath)
{	    
	SDL_Surface *image = IMG_Load(filepath.c_str());
    
	if(image == NULL) printf("Unable to load image %s! SDL Error: %s\n", filepath, SDL_GetError());
    
	return image;
}

Mix_Chunk *ResourceManager::loadSound(string filepath)
{
	Mix_Chunk *sound = Mix_LoadWAV(filepath.c_str());

	if(sound == NULL) printf("Failed to load sound - SDL_Mixer Error\n");

	return sound;
}

Mix_Music * ResourceManager::loadMusic(string filepath)
{
	Mix_Music *music = Mix_LoadMUS(filepath.c_str());

	if(music == NULL) printf("Failed to load music - SDL_Mixer Error");

	return music;
}


// Access resources

SDL_Texture * ResourceManager::getImage(string imgname)
{
	stringstream compiled;
	compiled << "Assets/Images/" << imgname;

	return images[compiled.str()];
}

Mix_Chunk * ResourceManager::getSound(string soundname)
{
	stringstream compiled;
	compiled << "Assets/Sounds/" << soundname;

	return sounds[compiled.str()];
}

TTF_Font * ResourceManager::getFont(string fontname, int i)
{
	stringstream compiled;
	compiled << "Assets/Fonts/" << fontname << ".ttf" << i;
	
	return fonts[compiled.str()];
}

Mix_Music * ResourceManager::getMusic()
{
	return music;
}