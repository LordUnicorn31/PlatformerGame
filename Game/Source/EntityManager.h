#pragma once
#include "Module.h"
#include "Point.h"
#include "Render.h"
#include "SString.h"
//#include "Animation.h"

class Entity;
struct SDL_Texture;
enum class EntityType : unsigned char;
enum class Enemy : unsigned char;

class EntityManager :public Module 
{
public:

	EntityManager();
	~EntityManager();

	void Init();
	bool Awake(pugi::xml_node& entityNode);
	bool Start();
	bool Update(float dt);
	void UpdateAll(float dt, bool doLogic);
	bool CleanUp();

	Entity* CreateEntity(EntityType type, iPoint pos);
	void DestroyEntity(Entity* entity);

	const List<Entity*> GetEntities()const;
	SString texturePath;

private:
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	List<Entity*> entities;
	bool doLogic;
	float accumulatedTime;
	float updateMsCycle;
	SDL_Texture* entityTexture;
	
};
