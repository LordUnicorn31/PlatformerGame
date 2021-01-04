#pragma once
#include "SDL2_ttf-2.0.15/SDL_ttf.h"

class Text
{
public:

	void Start();
	SDL_Texture* AddText(const char* file, int size, const char* text, SDL_Color color);
	void Clean();

};

