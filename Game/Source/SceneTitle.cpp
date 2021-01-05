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

	button = app->gui->AddButton(0, 0, { 0,456,43,33 }, { 0,419,43,33 }, { 47,418,43,34 }, this);
	app->gui->AddText(0, 0, "Continue", nullptr, button);
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
	if (element == button) 
	{
		app->transitions->FadeToBlack(this, app->scene, 0.5f);
	}
}

