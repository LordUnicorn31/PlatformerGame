#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Transitions.h"
#include "CastleScene.h"
#include "EntityManager.h"
#include "FlyEnemy.h"
#include "PatrolEnemy.h"
#include "WanderEnemy.h"
#include "Coin.h"
#include "Chest.h"
#include "Gui.h"
#include "SceneTitle.h"
#include "Collisions.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	mapName.create(config.child("mapname").attribute("name").as_string());
	mapPath.create(config.child("mapfolder").attribute("name").as_string());
	audioPath.create(config.child("audio").attribute("path").as_string());

	SDL_ShowCursor(SDL_DISABLE);
	return ret;
}

void Scene::Init()
{
	enabled = false;
	active = true;
}

// Called before the first frame
bool Scene::Start()
{   
	exitGame = false;
	pauseButton = app->gui->AddButton(1200, 10, { 755, 527, 39,39 }, { 871, 736, 39,39 }, { 755, 527, 39,39 }, this, nullptr, false, true, false);
	app->audio->PlayMusic(audioPath.GetString());
	app->entity->Enable();
	Map::Load(mapPath.GetString(),mapName.GetString());
	app->player->Enable();
	app->collisions->Enable();

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		app->win->FullScreen();

	//PROBLEM: Don't forget save and load
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGame();
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGame();
	

	Map::Draw();
	app->player->Lives(this);

	// DEBUG KEYS
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->player->Die();
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->transitions->FadeToBlack(this, app->castleScene);
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) 
	{
		if (optionsMenu == nullptr)
			pauseButton->module->UiCallback(pauseButton);
		else
			backButton->module->UiCallback(backButton);
	}
		
	app->render->RenderMouse();
	if (exitGame) {
		ret = false;
		exitGame = false;
	}
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->gui->DeleteAllUiElements();
	pauseWindow = nullptr;
	optionsMenu = nullptr;
	app->audio->UnloadMusic();
	app->player->Disable();
	app->entity->Disable();
	app->collisions->Disable();
	Map::UnLoad();
	return true;
}

void Scene::UiCallback(UiElement* element)
{
	if (element == pauseButton) {
		/*app->audio->PlayMusic("Resources/audio/music/music_options.ogg", 0.0f);*/
		if (pauseButton != nullptr) {
			if (pauseWindow == nullptr) 
			{

				pauseWindow = app->gui->AddImage(417, 150, { 0, 512, 483, 512 },this);
				app->gui->AddText(200, 50, "PAUSE", NULL, pauseWindow, { 255, 255, 255, 255 }, 32, false, false, false);
				continueButton = app->gui->AddButton(120, 90, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this, pauseWindow, false, true, false);
				app->gui->AddText(50, 25, "CONTINUE", NULL, continueButton, { 255, 255, 255, 255 }, 32, false, false, false);
				saveButton = app->gui->AddButton(120, 165, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this, pauseWindow, false, true, false);
				app->gui->AddText(80, 25, "SAVE", NULL, saveButton, { 255, 255, 255, 255 }, 32, false, false, false);
				optionsButton = app->gui->AddButton(120, 240, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this, pauseWindow, false, true, false);
				app->gui->AddText(60, 25, "OPTIONS", NULL, optionsButton, { 255, 255, 255, 255 }, 32, false, false, false);
				titleButton = app->gui->AddButton(120, 315, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this, pauseWindow, false, true, false);
				app->gui->AddText(50, 25, "MAIN MENU", NULL, titleButton);
				exitButton = app->gui->AddButton(120, 390, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this, pauseWindow, false, true, false);
				app->gui->AddText(45, 25, "EXIT GAME", NULL, exitButton, { 255, 255, 255, 255 }, 32, false, false, false);
				app->freeze = true;

			}
			else 
			{
				app->freeze = false;
				app->gui->RemoveUiElement(pauseWindow);
				app->gui->RemoveUiElement(continueButton);
				app->gui->RemoveUiElement(saveButton);
				app->gui->RemoveUiElement(optionsButton);
				app->gui->RemoveUiElement(exitButton);
				app->gui->RemoveUiElement(titleButton);
				pauseWindow = nullptr;
				/*app->audio->PlayMusic("Resources/audio/music/game.ogg", 0.0f);*/

			}
		}
	}
	if (element == exitButton) 
	{
		//Create The Funtionality
		/*app->audio->PlayFx(buttonFx);*/
		exitGame = true;

	}
	
	if (element == titleButton)
	{
		app->freeze = false;
		pauseWindow = nullptr;
		app->transitions->FadeToBlack(this, app->sceneTitle);
	}

	if (element == continueButton) 
	{
		/*app->audio->PlayMusic("Resources/audio/music/game.ogg", 0.0f);*/
		/*app->audio->PlayFx(buttonFx);*/
		app->freeze = false;
		if (pauseWindow != nullptr) 
		{
			app->gui->RemoveUiElement(pauseWindow);
			app->gui->RemoveUiElement(continueButton);
			app->gui->RemoveUiElement(saveButton);
			app->gui->RemoveUiElement(optionsButton);
			app->gui->RemoveUiElement(titleButton);
			app->gui->RemoveUiElement(exitButton);
			pauseWindow = nullptr;
		}
	}
	/*if (element == fullScreen) {
		app->audio->PlayFx(buttonFx);
		app->win->Fullscreen();
	}*/
	if (element == saveButton) 
	{
		/*app->audio->PlayFx(buttonFx);*/
		app->SaveGame();
	}
	if (element == fullScreenCheck) {
		/*app->audio->PlayFx(buttonFx);*/
		app->win->FullScreen();
	}
	if (element == vsyncCheck)
	{
		app->render->vSync = !app->render->vSync;
	}
	if (element == optionsButton) 
	{
		/*app->audio->PlayFx(buttonFx);*/
		optionsMenu = app->gui->AddImage(437, 177, { 20,540,446,465 }, this);
		backButton = app->gui->AddButton(30, 20, { 806,368,35,24 }, { 815,246,35,24 }, { 806,368,35,24 }, this, optionsMenu, false, true, false);
		musSlider = app->gui->AddSlider(115, 200, app->audio->GetMusicVolume(), MAX_VOLUME, true, false, false, optionsMenu, this);
		fxSlider = app->gui->AddSlider(115, 100, app->audio->GetFxVolume(), MAX_VOLUME, true, false, false, optionsMenu, this);
		/*app->gui->AddText(55, 25, "FULLSCREEN", app->font->smallFont, { 255,255,255 }, 42, false, false, false, fullScreen);*/
		optionsText = app->gui->AddText(135, 20, "OPTIONS MENU", nullptr, optionsMenu, {255, 255, 255}, 42, false, false, false);
		fxText = app->gui->AddText(70, 95, "FX", nullptr, optionsMenu, { 255, 255, 255 }, 42, false, false, false);
		musicText = app->gui->AddText(25, 200, "MUSIC", nullptr, optionsMenu, { 255, 255, 255 }, 42, false, false, false);
		fullScreenCheck = app->gui->AddCheckBox(300, 250, { 987, 808, 30, 30 }, { 1028, 808, 30, 30 }, this, optionsMenu);
		vsyncCheck = app->gui->AddCheckBox(300, 300, { 987, 808, 30, 30 }, { 1028, 808, 30, 30 }, this, optionsMenu);
		fullScreenText = app->gui->AddText(100, 255, "FULLSCREEN", nullptr, optionsMenu);
		vsyncText = app->gui->AddText(110, 305, "VSYNC ON", nullptr, optionsMenu);
	}
	if (element == backButton) 
	{
		app->gui->RemoveUiElement(optionsMenu);
		app->gui->RemoveUiElement(backButton);
		app->gui->RemoveUiElement(optionsText);
		app->gui->RemoveUiElement(fxText);
		app->gui->RemoveUiElement(musicText);
		app->gui->RemoveUiElement(musSlider);
		app->gui->RemoveUiElement(fxSlider);
		app->gui->RemoveUiElement(fullScreenText);
		app->gui->RemoveUiElement(vsyncText);
		app->gui->RemoveUiElement(fullScreenCheck);
		app->gui->RemoveUiElement(vsyncCheck);
		/*app->audio->PlayFx(buttonFx);*/
		optionsMenu = nullptr;
	}
	if (element == musSlider) 
	{
		app->audio->MusicVolume(((UiSlider*)element)->value);
	}
	if (element == fxSlider) 
	{
		app->audio->FxVolume(((UiSlider*)element)->value);
	}
}

