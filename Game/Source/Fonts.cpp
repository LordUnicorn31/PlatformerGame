#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Textures.h"
#include "Fonts.h"
#include "SDL2_ttf-2.0.15/SDL_ttf.h"

Fonts::Fonts() : Module()
{
	name = "fonts";
}

// Destructor
Fonts::~Fonts()
{}

// Called before render is available
bool Fonts::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		defaultFont = Load(path, size);

		smallFont = Load(path, 16);

		dialogFont = Load(path, 27);

		resourcesPanelFont = Load(path, 20);

		tutorialFont = Load(path, 23);
	}

	return ret;
}

// Called before quitting
bool Fonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");
	for (int i = 0; i < fonts.count(); ++i) 
	{
		if (fonts[i] != nullptr) 
		{
			TTF_CloseFont(fonts[i]);
			fonts[i] = nullptr;
		}
	}
	fonts.clear();

	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const Fonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFont(path, size);

	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.add(font);
	}

	return font;
}

// Print text using font
SDL_Texture* Fonts::Print(const char* text, SDL_Color color, TTF_Font* font)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : defaultFont, text, color);

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = app->tex->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool Fonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if (TTF_SizeText((font) ? font : defaultFont, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}