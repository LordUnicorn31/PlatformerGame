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
#include "FlyEnemy.h"
#include "PatrolEnemy.h"
#include "Coin.h"
#include "Chest.h"

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
	//Load the initial entities
	entityTexture = app->tex->Load(texturePath.GetString());

	return true;
}


bool EntityManager::Update(float dt)
{
	accumulatedTime += dt * 1000.0f;

	if (accumulatedTime >= updateMsCycle) 
		doLogic = true;

	UpdateAll(dt, doLogic);

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
	/*ListItem<Entity*>* i = entities.start;
	for (i; i != entities.end; it = it->next) {
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
	//Dlete all entities
	entities.clear();

	//Unload the entity texture
	app->tex->UnLoad(entityTexture);
	entityTexture = nullptr;
	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, iPoint pos) 
{
	Entity* ret = nullptr;
	
	switch (type)
	{
		
		case EntityType::FLY_ENEMY: 
			ret = new FlyEnemy(pos);
			ret->sprite = entityTexture;
			break;
		case EntityType::PATROL_ENEMY: 
			ret = new PatrolEnemy(pos);
			ret->sprite = entityTexture;
			break;
		case EntityType::WANDER_ENEMY:
			ret = new WanderEnemy(pos);
			ret->sprite = entityTexture;
			break;
		case EntityType::COIN:
			ret = new Coin(pos);
			ret->sprite = entityTexture;
			break;
		case EntityType::CHEST:
			ret = new Chest(pos);
			ret->sprite = entityTexture;
			break;
	}

	if (ret != nullptr) entities.add(ret);

	return ret;
}

void EntityManager::DestroyEntity(Entity* entity) 
{
	int listIdx = entities.find(entity);
	if (listIdx != -1)
		entities.del(entities.At(listIdx));
	else
		LOG("Entity to delete not found");
}

bool EntityManager::Load(pugi::xml_node& entitynode) 
{
	//TODO: entities load
	return true;
}

bool EntityManager::Save(pugi::xml_node& managernode) 
{
	//TODO: entities save
	return true;
}

const List<Entity*> EntityManager::GetEntities()const 
{
	return entities;
}
