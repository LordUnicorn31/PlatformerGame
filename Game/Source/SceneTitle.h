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

	UiElement* newGameButton;
	UiElement* continueButton;
	UiElement* optionsButton;
	UiElement* exitButton;
	UiElement* creditButton;

	UiElement* optionsMenu;
	UiElement* backButton;
	UiElement* sliderBarFx;
	UiElement* sliderBarMus;
	UiElement* musicText;
	UiElement* fxText;
	UiElement* optionsText;
	UiElement* fullScreenCheck;
	UiElement* vsyncCheck;
	UiElement* fullScreenText;
	UiElement* vsyncText;

	UiElement* creditPanel;
	UiElement* backCredit;
	UiElement* creditTitle;
	UiElement* creditsText;

};