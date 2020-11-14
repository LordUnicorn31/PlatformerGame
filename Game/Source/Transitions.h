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
	bool FadeToBlack(Module* Module_off, Module* Module_on, float time = 2.0f);
	bool Slide(Module* Module_off, Module* Module_on, float time = 2.0f);
	bool IsFading() const;


private:

	Module* moduleOff;
	Module* moduleOn;


	enum class fade_step
	{
		none,
		fadetoblack,
		fadefromblack,
		slidein,
		slidechange,
		slideout
	}
	current_step = fade_step::none;

	Uint32 startTime = 0;
	Uint32 totalTime = 0;
	SDL_Rect screen;

};

#endif //__TRANSITIONS_H__