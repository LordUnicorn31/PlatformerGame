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
	SString buttonPath;
	uint buttonFx;

	UiElement* newGameButton = nullptr;
	UiElement* continueButton = nullptr;
	UiElement* optionsButton = nullptr;
	UiElement* exitButton = nullptr;
	UiElement* creditButton = nullptr;

	UiElement* optionsMenu = nullptr;
	UiElement* backButton = nullptr;
	UiElement* sliderBarFx = nullptr;
	UiElement* sliderBarMus = nullptr;
	UiElement* musicText = nullptr;
	UiElement* fxText = nullptr;
	UiElement* optionsText = nullptr;
	UiElement* fullScreenCheck = nullptr;
	UiElement* vsyncCheck = nullptr;
	UiElement* fullScreenText = nullptr;
	UiElement* vsyncText = nullptr;

	UiElement* creditPanel = nullptr;
	UiElement* backCredit = nullptr;
	UiElement* creditTitle = nullptr;
	UiElement* creditsText = nullptr;

};