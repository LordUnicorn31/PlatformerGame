#pragma once
#include "Module.h"

struct SDL_Texture;

class SceneWin : public Module
{
public:

	SceneWin();

	// Destructor
	virtual ~SceneWin();

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

	void UiCallback(UiElement* element);

private:

	SDL_Texture* background;
	SString texturePath;
	SString audioPath;
	SString buttonPath;

	uint buttonFx;

	UiElement* buttonTitle;
	UiElement* textTitle;

};