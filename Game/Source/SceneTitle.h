#pragma once
#include "Module.h"

struct SDL_Texture;
class UiElement;

class SceneTitle : public Module
{
public:

	SceneTitle();

	// Destructor
	virtual ~SceneTitle();

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
	
	SDL_Texture* titleImage;
	bool exitGame;
	SString texturePath;
	SString audioPath;

	UiElement* button;
	UiElement* newGameButton;
	UiElement* continueButton;
	UiElement* optionsButton;
	UiElement* exitButton;

};