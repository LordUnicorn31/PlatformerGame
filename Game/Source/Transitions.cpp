#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <math.h>
#include "App.h"
#include "Transitions.h"
#include "Render.h"
#include "Window.h"
#include "SDL_render.h"
#include "SDL_timer.h"

Transitions::Transitions() : Module()
{
	name = "transitions";
}

Transitions::~Transitions()
{}

// Load assets
bool Transitions::Start()
{
	screen = { 0, 0, app->win->width * app->win->scale, app->win->height * app->win->scale };
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool Transitions::Update(float dt)
{
	if (current_step == FadeStep::NONE)
		return true;

	Uint32 now = SDL_GetTicks() - startTime;
	float normalized = MIN(1.0f, (float)now / (float)totalTime);

	switch (current_step)
	{
	case FadeStep::FADE_TO_BLACK:
	{
		if (now >= totalTime)
		{
			moduleOff->Disable();
			moduleOn->Enable();
			totalTime += totalTime;
			startTime = SDL_GetTicks();
			current_step = FadeStep::FADE_FROM_BLACK;
		}
	} break;

	case FadeStep::FADE_FROM_BLACK:
	{

		normalized = 1.0f - normalized;

		if (now >= totalTime)
			current_step = FadeStep::NONE;

	} break;

	case FadeStep::SLIDE_IN:

		normalized = 1.0f;
		if (screen.x >= 0)
			screen.x = 0;
		else
			screen.x += (int)(((float)(screen.w) / (float)totalTime) * dt * (float)now * 2);

		if (now >= totalTime)
		{
			current_step = FadeStep::SLIDE_CHANGE;
		}

		break;

	case FadeStep::SLIDE_CHANGE:

		normalized = 1.0f;
		screen.x;
		moduleOff->Disable();
		moduleOn->Enable();
		totalTime += totalTime;
		startTime = SDL_GetTicks();
		current_step = FadeStep::SLIDE_OUT;

		break;

	case FadeStep::SLIDE_OUT:

		normalized = 1.0f;
		screen.x += (int)(((float)(screen.w) / (float)totalTime) * dt * (float)now * 2);
		if (now >= totalTime) 
		{
			current_step = FadeStep::NONE;
		}

		break;
	}

	SDL_SetRenderDrawColor(app->render->renderer, (Uint8)0, (Uint8)0, (Uint8)0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one Module, then activate the other
bool Transitions::FadeToBlack(Module* Module_off, Module* Module_on, float time)
{
	bool ret = false;
	if (current_step == FadeStep::NONE)
	{
		screen.x = 0;
		current_step = FadeStep::FADE_TO_BLACK;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5f * 1000.0f);
		moduleOff = Module_off;
		moduleOn = Module_on;
		ret = true;
	}

	return ret;
}

bool Transitions::Slide(Module* Module_off, Module* Module_on, float time)
{
	bool ret = false;

	if (current_step == FadeStep::NONE)
	{
		screen.x -= app->win->width;
		current_step = FadeStep::SLIDE_IN;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5 * 1000.0f);
		moduleOff = Module_off;
		moduleOn = Module_on;
		ret = true;
	}
	return ret;
}



bool Transitions::IsFading() const
{
	return current_step != FadeStep::NONE;
}

bool Transitions::CleanUp() {
	return true;
}
