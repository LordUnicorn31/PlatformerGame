#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Input.h"
#include "Map.h"
#include "Transitions.h"
#include "Audio.h"
#include "SceneWin.h"
#include "Player.h"
#include "Gui.h"
#include "SceneTitle.h"

SceneWin::SceneWin() : Module()
{
	name = "sceneWin";
}

// Destructor
SceneWin::~SceneWin()
{
}

// Called before render is available
bool SceneWin::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	texturePath.create(config.child("texture").attribute("path").as_string());
	audioPath.create(config.child("audio").attribute("path").as_string());
	bool ret = true;
	return ret;
}

// Called before the first frame
bool SceneWin::Start()
{
	
	app->player->Disable();
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	background = app->tex->Load(texturePath.GetString());
	app->audio->PlayMusic(audioPath.GetString());
	buttonTitle = app->gui->AddButton((int)525.5f, 470, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this);
	textTitle = app->gui->AddText(50, 22, "MAIN MENU", nullptr, buttonTitle, { 255,255,255 }, 32, false, false, false);

	return true;
}

// Called each loop iteration
bool SceneWin::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneWin::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		app->win->FullScreen();
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->transitions->FadeToBlack(this, app->scene, 0.5f);
	}
	app->render->DrawTexture(background, 0, 0, NULL, 1.0f, SDL_FLIP_NONE, false);

	return ret;
}

// Called each loop iteration
bool SceneWin::PostUpdate()
{
	app->render->RenderMouse();
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	return ret;
}


// Called before quitting
bool SceneWin::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(background);
	app->audio->UnloadMusic();
	app->gui->DeleteAllUiElements();
	buttonTitle = nullptr;
	textTitle = nullptr;
	return true;
}

void SceneWin::Init()
{
	enabled = false;

	active = true;
}

void SceneWin::UiCallback(UiElement* element)
{
	if (element == buttonTitle)
	{
		app->transitions->FadeToBlack(this, app->sceneTitle);
	}
}