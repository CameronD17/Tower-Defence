#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager(void)
{
}


void ResourceManager::init(SDL_Renderer	*r)
{
	renderer = r;
	preloadImages("Assets/Inputs/IMAGE_FILEPATHS.txt");
	preloadSounds("Assets/Inputs/SOUND_FILEPATHS.txt");
	preloadFonts();

	music = loadMusic("Assets/Audio/background.mp3");
}



// Initially load resources

void ResourceManager::preloadImages(string filepath)
{
	string line;
	ifstream imageData(filepath);

	if (imageData.is_open())
	{
		while(getline(imageData,line))
		{
			images.push_back(SDL_CreateTextureFromSurface(renderer, loadImage(line)));
			imgFilePath.push_back(line);
		}
		imageData.close();
	}
}

void ResourceManager::preloadSounds(string filepath)
{
	string line;
	ifstream soundData(filepath);

	if (soundData.is_open())
	{
		while(getline(soundData,line))
		{
			Mix_Chunk *c = loadSound(line);
			sounds.push_back(c);
			soundFilePath.push_back(line);
		}
		soundData.close();
	}
}

void ResourceManager::preloadFonts()
{	
	fonts.push_back(loadFont(10));
	fonts.push_back(loadFont(15));
	fonts.push_back(loadFont(20));
	fonts.push_back(loadFont(30));
	fonts.push_back(loadFont(40));
	fonts.push_back(loadFont(50));		
}



// Load resources

TTF_Font *ResourceManager::loadFont(int size)
{
	TTF_Font *font = TTF_OpenFont("Assets/Fonts/imagine.ttf", size);

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

SDL_Texture * ResourceManager::getImage(string filepath)
{
	SDL_Texture *imgTexture = SDL_CreateTextureFromSurface(renderer, loadImage("Assets/Images/broken.gif"));
	int i = 0;
	
	for(vector<string>::iterator s = imgFilePath.begin(); s != imgFilePath.end(); ++s)
	{	
		if((*s) == filepath)
		{
			imgTexture = images.at(i);
			break;
		}	
		i++;
	}

	return imgTexture;
}

Mix_Chunk * ResourceManager::getSound(string filepath)
{
	Mix_Chunk *sound = Mix_LoadWAV("sound is null");
	int i = 0;
	
	for(vector<string>::iterator s = soundFilePath.begin(); s != soundFilePath.end(); ++s)
	{	
		if((*s) == filepath)
		{
			sound = sounds.at(i);
			break;
		}	
		i++;
	}

	return sound;
}

TTF_Font * ResourceManager::getFont(int i)
{
	return fonts.at(i);
}

Mix_Music * ResourceManager::getMusic()
{
	return music;
}



// Free resources

void ResourceManager::close()
{
	// Clean up images
	for(unsigned int i = 0; i < images.size(); i++)
	{
		if (images.at(i) != NULL) 
		{ 
			SDL_DestroyTexture(images.at(i));
			images.at(i) = NULL;
		}
	}

	// Clean up sounds
	Mix_FreeMusic(music);	
	for (unsigned int i = 0; i < sounds.size(); i++)
	{
		if (sounds.at(i) != NULL) 
		{ 
			Mix_FreeChunk(sounds.at(i));
			sounds.at(i) = NULL;
		}
	}

	// Clean up fonts	
	for (unsigned int i = 0; i < fonts.size(); i++)
	{
		if (fonts.at(i) != NULL) 
		{ 
			TTF_CloseFont(fonts.at(i));
			fonts.at(i) = NULL;
		}
	}
}