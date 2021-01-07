#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "CastleScene.h"
#include "Map.h"
#include "Player.h"
#include "Transitions.h"
#include "SceneLose.h"
#include "EntityManager.h"

#include "Defs.h"
#include "Log.h"

CastleScene::CastleScene() : Module()
{
	name.create("castleScene");
}

// Destructor
CastleScene::~CastleScene()
{}

// Called before render is available
bool CastleScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	mapName.create(config.child("mapname").attribute("name").as_string());
	audioPath.create(config.child("audio").attribute("path").as_string());
	mapPath.create(config.child("mapfolder").attribute("name").as_string());

	SDL_ShowCursor(SDL_DISABLE);
	return ret;
}

void CastleScene::Init()
{
	enabled = false;
	active = true;
}

// Called before the first frame
bool CastleScene::Start()
{
	app->audio->PlayMusic(audioPath.GetString());
	app->entity->Enable();
	Map::Load(mapPath.GetString(),mapName.GetString());
	app->player->Enable();

	pauseButton = nullptr;
	return true;
}

// Called each loop iteration
bool CastleScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool CastleScene::Update(float dt)
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
	onDeath = app->player->OnDeath();
	if (onDeath)
	{
		app->LoadGame();
	}
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->transitions->FadeToBlack(this, app->scene);
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->player->Die();
	}
	return true;
}

// Called each loop iteration
bool CastleScene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		pauseButton->module->UiCallback(pauseButton);

	return ret;
}

// Called before quitting
bool CastleScene::CleanUp()
{
	LOG("Freeing scene");
	Map::UnLoad();
	app->audio->UnloadMusic();
	app->player->Disable();
	app->entity->Disable();

	return true;
}

