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
}

EntityManager::~EntityManager() {

}

void EntityManager::Init() {
	active = true;
	enabled = false;
}


bool EntityManager::Start() {
	//Load the initial entities
	if (newgame) 
	{

	}
	
	return true;
}


bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;

	if (accumulatedTime >= updateMsCycle) doLogic = true;

	

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}


void EntityManager::UpdateAll(float dt, bool DoLogic) 
{
		
}


bool EntityManager::PostUpdate() {
	
	return true;
}

bool EntityManager::CleanUp() {
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


const ListItem<Entity*>* EntityManager::GetEntities()const 
{
	return entities;
}
