#ifndef __ANIMATION_H__
#define __ANIMATION_H__
#include "Defs.h"
#include "SDL.h"
#define MAX_FRAMES 2


class Animations
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float currentFrame = 0.0f;
	int lastFrame = 0;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& collider)
	{
		frames[lastFrame++] = collider;
	}

	SDL_Rect& GetCurrentFrame(float dt)
	{
		currentFrame += speed * dt;
		if (currentFrame >= lastFrame)
		{
			currentFrame = (loop) ? 0.0f : lastFrame - 1;
			loops++;
		}

		return frames[(int)currentFrame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		currentFrame = 0;
		loops = 0;
	}
};

#endif