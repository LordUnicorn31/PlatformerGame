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
#include "SceneLose.h"
#include "Player.h"

LoseScene::LoseScene() : Module()
{
	name = "sceneLose";
}

// Destructor
LoseScene::~LoseScene()
{
}

// Called before render is available
bool LoseScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	texturePath.create(config.child("texture").attribute("path").as_string());
	bool ret = true;
	return ret;
}

// Called before the first frame
bool LoseScene::Start()
{
	app->player->Disable();
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	background = app->tex->Load(texturePath.GetString());

	return true;
}

// Called each loop iteration
bool LoseScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool LoseScene::Update(float dt)
{
	bool ret = true;

	
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->transitions->FadeToBlack(this, app->scene,0.5f);
	}
	app->render->DrawTexture(background, 0, 0, NULL, true);

	return ret;
}

// Called each loop iteration
bool LoseScene::PostUpdate()
{
	
	bool ret = true;

	return ret;
}


// Called before quitting
bool LoseScene::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(background);
	return true;
}

void LoseScene::Init()
{
	enabled = false;

	active = true;
}

