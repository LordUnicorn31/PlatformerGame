#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "FlyEnemy.h"
#include "PatrolEnemy.h"
#include "WanderEnemy.h"
#include "Coin.h"
#include "Chest.h"
#include "Gui.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	void Init();

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

	// Camera movement

	void UiCallback(UiElement* element);
	

private:
	SString mapName;
	SString mapPath;
	SString audioPath;
	iPoint mapBat;
	iPoint mapZombie;
	iPoint mapSkeleton;
	iPoint mapCoin;
	iPoint mapChest;
	PatrolEnemy* skeleton = nullptr;
	FlyEnemy* bat = nullptr;
	WanderEnemy* zombie = nullptr;
	Coin* coin = nullptr;
	Chest* chest = nullptr;
	UiElement* pauseButton;
	UiElement* pauseWindow;
	UiElement* continueButton;
	UiElement* saveButton;
	UiElement* titleButton;
	UiElement* optionsButton;
	UiElement* exitButton;

	UiElement* optionsMenu;
	UiElement* backButton;
	UiElement* musSlider;
	UiElement* fxSlider;

	UiElement* optionsText;
	UiElement* fxText;
	UiElement* musicText;

	bool exitGame;
};

#endif // __SCENE_H__