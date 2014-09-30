#include "Graphics.h"

Graphics::Graphics(void)
{	
	window = NULL;
	renderer = NULL;
}

Graphics::~Graphics(void)
{

}

bool Graphics::init(int w, int h, ResourceManager *r)
{
	bool success = true;

	resource = r;

	width = w;	height = h;

	//Create window
	window = SDL_CreateWindow("Tower Defence!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if(window == NULL) 
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());			success = false;
	}
	else 
	{
		//Create renderer for window
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if(renderer == NULL) 
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());	success = false;
		}
		else 
		{
			SDL_Surface *surface = resource->loadImage("Engine/Images/icon.png");
			SDL_SetWindowIcon(window, surface); 
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		}
	}

	return success;
}

void Graphics::clear()
{
	SDL_SetRenderDrawColor(renderer,0,0,0,0);	// Draw a black rectangle over the whole screen
	SDL_RenderClear(renderer);						// Clear the renderer
}

void Graphics::update()
{
    SDL_RenderPresent(renderer);					// Update the renderer
}

void Graphics::drawRectangle(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect rect;
    rect.x = x; rect.y = y;
    rect.w = w; rect.h = h;
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
}

void Graphics::drawRectangleOL(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect rect;
    rect.x = x; rect.y = y;
    rect.w = w; rect.h = h;
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &rect);
}

void Graphics::renderText(int x, int y, string text, int size, int r, int g, int b, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{	
	// Placeholders for the size of the text being rendered
	int width;
	int height;
	
	// The colour to render the text. Default is white (255, 255, 255)
	SDL_Color textColor = {r, g, b};

	SDL_Surface *textSurface;

	switch(size)
	{
	case 10:
		textSurface = TTF_RenderText_Solid(resource->getFont(0), text.c_str(), textColor);
		break;
	case 15:
		textSurface = TTF_RenderText_Solid(resource->getFont(1), text.c_str(), textColor);
		break;
	case 20:
		textSurface = TTF_RenderText_Solid(resource->getFont(2), text.c_str(), textColor);
		break;
	case 30:
		textSurface = TTF_RenderText_Solid(resource->getFont(3), text.c_str(), textColor);
		break;
	case 40:
		textSurface = TTF_RenderText_Solid(resource->getFont(4), text.c_str(), textColor);
		break;
	case 50:
		textSurface = TTF_RenderText_Solid(resource->getFont(5), text.c_str(), textColor);
		break;
	default:
		textSurface = TTF_RenderText_Solid(resource->loadFont(size), text.c_str(), textColor);
	}	
		
	SDL_Texture *fontText = SDL_CreateTextureFromSurface(renderer, textSurface);

	if(fontText == NULL)
	{
		printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		//Get image dimensions
		width = textSurface->w;
		height = textSurface->h;
	
		//Set rendering space and render to screen
		SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};
	
		//Set clip rendering dimensions
		if(clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		//Render to screen
		SDL_RenderCopyEx(renderer, fontText, clip, &renderQuad, angle, center, flip);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(fontText);
	}
}

void Graphics::renderImage(int x, int y, string filepath, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Texture *imgTexture = resource->getImage(filepath);
	SDL_Rect pos;
    pos.x = x;  pos.y = y;
    SDL_QueryTexture(imgTexture, 0, 0, &pos.w, &pos.h);
    SDL_RenderCopy(renderer, imgTexture, 0, &pos);
}

void Graphics::renderTexture(int x, int y, SDL_Texture *t)
{
	SDL_Rect pos;
    pos.x = x;  pos.y = y;
    SDL_QueryTexture(t, 0, 0, &pos.w, &pos.h);
    SDL_RenderCopy(renderer, t, 0, &pos);
}

void Graphics::close()
{	
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;
}