#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"


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
	app->audio->PlayMusic(audioPath.GetString());
	Map::Load(mapPath.GetString(),mapName.GetString());
	app->player->Enable();
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
	/*Camera For DEBUG purposes
	if(app->render->camera.y >= -3200 && app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 10*2;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 10*2;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && -app->render->camera.x + app->win->width <= 6400)
		app->render->camera.x -= 10*2;

	if(app->render->camera.x <= 0 && app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 10*2;
	*/
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		app->win->FullScreen();

	//PROBLEM: Don't forget save and load
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGame();
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGame();

	Map::Draw();
	
	//app->render->DrawTexture(img, 380, 100);


	// DEBUG KEYS
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->player->Die();
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->player->Die();
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	Map::UnLoad();
	app->audio->UnloadMusic();
	app->player->Disable();
	return true;
}

