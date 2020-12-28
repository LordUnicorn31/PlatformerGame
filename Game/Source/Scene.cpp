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

	mapBat.x = config.child("batposx").attribute("x").as_int();
	mapBat.y = config.child("batposy").attribute("y").as_int();
	mapZombie.x = config.child("zombieposx").attribute("x").as_int();
	mapZombie.y = config.child("zombieposy").attribute("y").as_int();
	mapSkeleton.x = config.child("skeletonposx").attribute("x").as_int();
	mapSkeleton.y = config.child("skeletonposy").attribute("y").as_int();
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
 	app->entity->Enable();
	app->audio->PlayMusic(audioPath.GetString());

	Map::Load(mapPath.GetString(),mapName.GetString());
	app->player->Enable();
	


	iPoint batPos = Map::MapToWorld(mapBat.x, mapBat.y);
	iPoint skeletonPos = Map::MapToWorld(mapSkeleton.x, mapSkeleton.y);
	iPoint zombiePos = Map::MapToWorld(mapZombie.x, mapZombie.y);
	bat = (FlyEnemy*)app->entity->CreateEntity(EntityType::FLY_ENEMY, batPos);
	skeleton = (PatrolEnemy*)app->entity->CreateEntity(EntityType::PATROL_ENEMY, skeletonPos);
	zombie = (WanderEnemy*)app->entity->CreateEntity(EntityType::WANDER_ENEMY, zombiePos);


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

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	
	app->audio->UnloadMusic();
	app->player->Disable();
	app->entity->Disable();
	Map::UnLoad();
	return true;
}

