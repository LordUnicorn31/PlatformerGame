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
	map_name.create(config.child("map_name").attribute("name").as_string());
	SDL_ShowCursor(SDL_DISABLE);
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	img = app->tex->Load("Assets/textures/test.png");
	app->audio->PlayMusic("Assets/audio/music/Forest.ogg");
	app->map->Load(map_name.GetString());
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
	/*if(app->render->camera.y >= -3200 && app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if(app->render->camera.x <= 0 && app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1;*/

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		app->win->FullScreen();


	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGame();
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGame();

	app->map->Draw();
	CameraMovement();
	//app->render->DrawTexture(img, 380, 100);

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

	return true;
}

void Scene::CameraMovement()
{
	app->render->camera.x = -(app->player->position.x - app->render->camera.w/2);
	
	if (app->render->camera.x > 0)
	{
		app->render->camera.x = 0;
	}

	app->render->camera.y = -(app->player->position.y - app->render->camera.h/2);
	
	if (app->render->camera.y > 0)
	{
		app->render->camera.y = 0;
	}
}
