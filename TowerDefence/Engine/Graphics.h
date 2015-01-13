#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "ResourceManager.h"

using namespace std;


/*-------------------------------------------------------- *
 * GRAPHICS
 * This class will deal with all graphics and visual 
 * representations within the game, including the game 
 * window and renderer
 *-------------------------------------------------------- */


class Graphics
{

private:
	int width, height;	
	
public:	
	Graphics(void);
	Graphics(int x, int y);
	~Graphics(void);

	ResourceManager *resource;

	SDL_Window		*window;	// The SDL window to display
	
	SDL_Renderer	*renderer;	// The SDL renderer

	bool init(int w, int h, ResourceManager *r);
		
	void clear();	// Clear the screen to be redrawn on
		
	void update();	// Update the window with redrawn data

	void drawLine(int x1, int y1, int x2, int y2);

	void drawRectangle(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b);

	void drawRectangleOL(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b);

	void renderText(int x, int y, string text, int size, int r = 255, int g = 255, int b = 255, string fontName = "imagine", SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void renderImage(int x, int y, string text, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	void renderTexture(int x, int y, SDL_Texture *t);

	SDL_Surface * makeSurfaceFromText(int x, int y, string text, int size, int r, int g, int b);

	void close();
};
