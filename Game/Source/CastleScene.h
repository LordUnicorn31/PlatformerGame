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
	void ChangeCoinCounter();
	void ChangeLivesCounter();
	void ChangeScoreCounter();

private:
	SString mapName;
	SString audioPath;
	SString mapPath;
	SString buttonPath;
	
	uint buttonFx;

	UiElement* pauseButton = nullptr;
	UiElement* pauseWindow = nullptr;
	UiElement* continueButton = nullptr;
	UiElement* saveButton = nullptr;
	UiElement* titleButton = nullptr;
	UiElement* optionsButton = nullptr;
	UiElement* exitButton = nullptr;

	UiElement* optionsMenu = nullptr;
	UiElement* backButton = nullptr;
	UiElement* musSlider = nullptr;
	UiElement* fxSlider = nullptr;

	UiElement* optionsText = nullptr;
	UiElement* fxText = nullptr;
	UiElement* musicText = nullptr;
	UiElement* fullScreenCheck = nullptr;
	UiElement* vsyncCheck = nullptr;
	UiElement* fullScreenText = nullptr;
	UiElement* vsyncText = nullptr;

	UiElement* livesHeart = nullptr;
	UiElement* playerCoins = nullptr;
	UiElement* livesText = nullptr;
	UiElement* coinsText = nullptr;
	UiElement* scoreText = nullptr;
	UiElement* scoreT = nullptr;

	UiElement* timeText = nullptr;
	UiElement* clockImage = nullptr;

	float currentTime;
	int totalLevelTime;

	bool exitGame;

	int score;
};

#endif // __CASTLESCENE_H__
