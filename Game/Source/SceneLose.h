#pragma once
#include "Module.h"

struct SDL_Texture;

class LoseScene : public Module
{
public:

	LoseScene();

	// Destructor
	virtual ~LoseScene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void Init();

private:

	SDL_Texture* background;
	SString texturePath;

};
