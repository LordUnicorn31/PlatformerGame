#include "SDL2_ttf-2.0.15/SDL_ttf.h"
#include "Text.h"
#include "Render.h"
#include "App.h"


void Text::Start()
{
	TTF_Init();
}

SDL_Texture* Text::AddText(const char* file, int size, const char* text, SDL_Color color)
{
	SDL_Texture* texture = NULL;
	TTF_Font* font = TTF_OpenFont(file, 10);
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
	texture = SDL_CreateTextureFromSurface(app->render->renderer, textSurface);

	return texture;
}

void Text::Clean()
{
	TTF_Quit();
}