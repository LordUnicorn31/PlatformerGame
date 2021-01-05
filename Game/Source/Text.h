#pragma once

class Text
{
public:

	void Start();
	SDL_Texture* AddText(const char* file, int size, const char* text, SDL_Color color);
	void Clean();

};

