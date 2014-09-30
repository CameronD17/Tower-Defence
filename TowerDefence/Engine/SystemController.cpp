#include "SystemController.h"

SystemController::SystemController()
{
}

SystemController::~SystemController(void)
{
}

bool SystemController::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		//Enable VSync
		if(!SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1"))
		{
			printf("Warning: VSync not enabled!");
		}

		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		
		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		if(!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			return false;
		}

		//Initialize SDL_ttf
		if(TTF_Init() == -1)
		{
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			return false;
		}	

		//Initialize SDL_mixer
		if(Mix_OpenAudio(22050, AUDIO_S16SYS, 1, 4096) == -1)
		{
			return false;    
		}
	}	
	
	success = graphics.init(SCREEN_WIDTH, SCREEN_HEIGHT, &resources);
	resources.init(graphics.renderer);
	audio.init(&resources);

	return success;
}

void SystemController::refresh()
{

}

void SystemController::close(void)
{
	graphics.close();
	resources.close();

    Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}