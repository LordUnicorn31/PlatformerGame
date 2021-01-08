#ifndef __CASTLESCENE_H__
#define __CASTLESCENE_H__

#include "Module.h"
#include "FlyEnemy.h"
#include "PatrolEnemy.h"
#include "WanderEnemy.h"

struct SDL_Texture;
class UiElement;

class CastleScene : public Module
{
public:

	CastleScene();

	// Destructor
	virtual ~CastleScene();

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

	void UiCallback(UiElement* element);

private:
	SString mapName;
	SString audioPath;
	SString mapPath;
	bool onDeath;

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
	UiElement* fullScreenCheck;
	UiElement* vsyncCheck;
	UiElement* fullScreenText;
	UiElement* vsyncText;

	bool exitGame;
};

#endif // __CASTLESCENE_H__
