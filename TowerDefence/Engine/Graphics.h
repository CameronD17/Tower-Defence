#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "ResourceManager.h"

 

class Graphics
{

private:
	int width, height;	
	SDL_Surface *makeSurfaceFromText(int x, int y, std::string text, int size, int r, int g, int b);

public:	
	Graphics(void);
	Graphics(int x, int y);
	~Graphics(void);

	ResourceManager *resource = NULL;
	SDL_Window		*window = NULL;	
	SDL_Renderer	*renderer = NULL;

	bool init(int w, int h, ResourceManager *r);
		
	void clear();		
	void update();
	void screenshot();

	void drawLine(int x1, int y1, int x2, int y2, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);

	void drawRectangle(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b);

	void drawRectangleOL(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b);

	void renderText(int x, int y, std::string text, int size, int r = 255, int g = 255, int b = 255, std::string fontName = "imagine", SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void renderImage(int x, int y, std::string text, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	void renderTexture(int x, int y, SDL_Texture *t);

	void close();
};
