#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Scene.h"
#include "SceneTitle.h"
#include "Input.h"
#include "Transitions.h"
#include "Audio.h"
#include "SceneLogo.h"
#include "Player.h"
#include "Window.h"
#include "Gui.h";

SceneTitle::SceneTitle() : Module()
{
	name = "sceneTitle";
}

// Destructor
SceneTitle::~SceneTitle()
{
}


// Called before render is available
bool SceneTitle::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	texturePath.create(config.child("texture").attribute("path").as_string());
	audioPath.create(config.child("audio").attribute("path").as_string());

	return true;
}

// Called before the first frame
bool SceneTitle::Start()
{
	exitGame = false;
	titleImage = app->tex->Load(texturePath.GetString());
    app->audio->PlayMusic(audioPath.GetString());

	newGameButton = app->gui->AddButton((int)525.5f, 340, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 },this);
	app->gui->AddText(52, 22, "NEW GAME", nullptr, newGameButton, { 255,255,255 }, 32, false, false, false);
	continueButton = app->gui->AddButton((int)525.5f, 420, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this);
	app->gui->AddText(50, 22, "CONTINUE", nullptr, continueButton, { 255,255,255 }, 32, false, false, false);
	optionsButton = app->gui->AddButton((int)525.5f, 500, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this);
	app->gui->AddText(60, 22, "OPTIONS", nullptr, optionsButton, { 255,255,255 }, 32, false, false, false);
	exitButton = app->gui->AddButton((int)525.5f, 580, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this);
	app->gui->AddText(80, 22, "EXIT", nullptr, exitButton, { 255,255,255 }, 32, false, false, false);
	return true;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneTitle::Update(float dt)
{
	bool ret = true;

	app->render->DrawTexture(titleImage, 0, 0, NULL);

	if (exitGame) {
		ret = false;
		exitGame = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		app->win->FullScreen();
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->transitions->FadeToBlack(this, app->scene,0.5f);
	}

	return ret;
}

// Called each loop iteration
bool SceneTitle::PostUpdate()
{
	app->render->RenderMouse();
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	return ret;
}


// Called before quitting
bool SceneTitle::CleanUp()
{
	LOG("Freeing scene");

	app->gui->DeleteAllUiElements();
	app->tex->UnLoad(titleImage);
	app->audio->UnloadMusic();
	app->audio->UnloadFx();
	titleImage = nullptr;


	return true;
}

void SceneTitle::Init()
{
	enabled = false;

	active = true;
}

void SceneTitle::UiCallback(UiElement* element)
{
	if (element == newGameButton)
	{
		app->transitions->FadeToBlack(this, app->scene, 0.5f);
	}

	if (element == exitButton)
	{
		exitGame = true;
	}

	if (element == continueButton)
	{

	}

	if (element == optionsButton)
	{
		optionsMenu = app->gui->AddImage(417, 200, { 20,540,446,465 }, true, false, false, nullptr, this);
		backButton = app->gui->AddButton(30, 40, { 806,368,35,24 }, { 815,246,35,24 }, { 806,368,35,24 }, this, optionsMenu);
		sliderBarFx = app->gui->AddSlider(115, 100, app->audio->GetFxVolume(), MAX_VOLUME, true, false, false, optionsMenu, this);
		sliderBarMus = app->gui->AddSlider(115, 175, app->audio->GetMusicVolume(), MAX_VOLUME, true, false, false, optionsMenu, this);
		optionsText = app->gui->AddText(130, 35, "OPTIONS MENU", nullptr, optionsMenu, { 255,255,255 }, 42, false, false, false);
		fxText = app->gui->AddText(70, 95, "FX", nullptr, optionsMenu, { 255,255,255 }, 42, false, false, false);
		musicText = app->gui->AddText(25, 170, "MUSIC", nullptr, optionsMenu, { 255,255,255 }, 42, false, false, false);
	}

	if (element == backButton) {
		/*app->audio->PlayFx(buttonFx);*/
		app->gui->RemoveUiElement(optionsMenu);
		app->gui->RemoveUiElement(sliderBarMus);
		app->gui->RemoveUiElement(sliderBarFx);
		app->gui->RemoveUiElement(optionsText);
		app->gui->RemoveUiElement(fxText);
		app->gui->RemoveUiElement(musicText);
		
	}
	if (element == fullScreen) {
		/*app->audio->PlayFx(buttonFx);*/
		app->win->FullScreen();
	}
	if (element == sliderBarFx) {
		app->audio->FxVolume(((UiSlider*)element)->value);
	}
	if (element == sliderBarMus) {
		app->audio->MusicVolume(((UiSlider*)element)->value);
	}
}

