#pragma once
#ifndef __TRANSITIONS_H__
#define __TRANSITIONS_H__

#include "Module.h"
#include "SDL_rect.h"


class Transitions : public Module
{
public:


	Transitions();

	virtual ~Transitions();

	bool Start();
	bool Update(float dt);
	bool CleanUp();
	bool FadeToBlack(Module* ModuleOff, Module* ModuleOn, float time = 2.0f);
	bool Slide(Module* ModuleOff, Module* ModuleOn, float time = 2.0f);
	bool IsFading() const;


private:

	Module* moduleOff;
	Module* moduleOn;


	enum class FadeStep
	{
		NONE,
		FADE_TO_BLACK,
		FADE_FROM_BLACK,
		SLIDE_IN,
		SLIDE_CHANGE,
		SLIDE_OUT
	}
	current_step = FadeStep::NONE;

	Uint32 startTime = 0;
	Uint32 totalTime = 0;
	SDL_Rect screen;

};

#endif //__TRANSITIONS_H__