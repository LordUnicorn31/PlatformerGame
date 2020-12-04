#include "Textures.h"
#include "EntityManager.h"
#include "Entity.h"
#include "App.h"
//#include "Animation.h"
#include "Input.h"
#include "PathFinding.h"
#include "Map.h"
#include "Log.h"
#include <time.h>
#include "String.h"
#include "Scene.h"
#include "Audio.h"

EntityManager::EntityManager()  
{
	name = "EntityManager";
	doLogic = false;
	accumulatedTime = 0.0f;
	updateMsCycle = 16.66666666f; //A 60 fps
}

EntityManager::~EntityManager() 
{

}

void EntityManager::Init() 
{
	active = true;
	enabled = false;
}

bool EntityManager::Awake(pugi::xml_node& entityNode)
{
	texturePath = entityNode.child_value("texture");
	return true;
}


bool EntityManager::Start() 
{
	/*//Load the initial entities
	if (newgame) 
	{

	}*/
	entityTexture = app->tex->Load(texturePath.GetString());
	//initialize player position

	return true;
}


bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;

	if (accumulatedTime >= updateMsCycle) 
		doLogic = true;

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}


void EntityManager::UpdateAll(float dt, bool DoLogic) 
{
	ListItem<Entity*>* it = entities.start;
	for (it; it != nullptr; it = it->next) {
		it->data->Update(dt);
		if (DoLogic)
			it->data->UpdateLogic();
		it->data->Draw(dt);
	}
	ListItem<Entity*>* i = entities.start;
	/*for (i; i != entities.end; it = it->next) {
		if (i->data->todie) {
			Entity* todestroy = i->data;
			++i;
			DestroyEntity(todestroy);
		}
		else
			++i;
	}*/
}

bool EntityManager::CleanUp() 
{
	app->tex->UnLoad(entityTexture);
	return true;
}

Entity* EntityManager::CreateEntity(EntityType type) 
{

	return nullptr;
}

void EntityManager::DestroyEntity(Entity* entity) 
{
	
}

bool EntityManager::Load(pugi::xml_node& entitynode) 
{
	return true;
}

bool EntityManager::Save(pugi::xml_node& managernode) 
{
	return true;
}

const List<Entity*> EntityManager::GetEntities()const 
{
	return entities;
}
