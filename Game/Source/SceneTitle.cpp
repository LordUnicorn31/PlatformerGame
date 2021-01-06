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
	app->tex->UnLoad(app->render->mouseText);
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
		/*optionsMenu = app->gui->AddImage(400, 250, { 20,540,446,465 }, true, false, false, nullptr, this);
		backButton = app->gui->AddButton(30, 40, { 806,368,35,24 }, { 815,246,35,24 }, { 806,368,35,24 }, this, optionsMenu);
		fullScreen = app->gui->AddButton(100, 250, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this, optionsMenu);
		sliderbarFx = app->gui->AddSlider(115, 100, app->audio->GetFxVolume(), MIX_MAX_VOLUME, true, false, false, optionsMenu, this);
		sliderbarMus = app->gui->AddSlider(115, 175, app->audio->GetMusicVolume(), MIX_MAX_VOLUME, true, false, false, optionsMenu, this);
		app->gui->AddText(55, 25, "FULLSCREEN", nullptr, { 255,255,255 }, 42, false, false, false, fullScreen);
		app->gui->AddText(150, 35, "OPTIONS MENU", nullptr, { 236,178,0 }, 42, false, false, false, optionsMenu);
		app->gui->AddText(70, 100, "FX", nullptr, { 236,178,0 }, 42, false, false, false, optionsMenu);
		app->gui->AddText(50, 175, "MUSIC", nullptr, { 236,178,0 }, 42, false, false, false, optionsMenu);*/
	}
}

