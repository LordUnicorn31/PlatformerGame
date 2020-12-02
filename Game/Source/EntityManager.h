#pragma once
#include "Module.h"
#include "Point.h"
#include "Render.h"
#include "SString.h"
//#include "Animation.h"

#define MAXSELECTEDUNITS 33
#define MAXRESOURCES 9999

class Entity;
struct SDL_Texture;
enum class EntityType : unsigned char;
enum class Enemy : unsigned char;

class EntityManager :public Module {
public:

	EntityManager();
	~EntityManager();

	void Init();
	bool Start();
	bool Update(float dt);
	void UpdateAll(float dt, bool doLogic);
	bool PostUpdate();
	bool CleanUp();

	Entity* CreateEntity(EntityType type);
	void DestroyEntity(Entity* entity);

	const List<Entity*> GetEntities()const;


private:
	bool newgame;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	List<Entity*> entities;
	bool doLogic;
	float accumulatedTime;
	float updateMsCycle;
	SDL_Texture* player;
	SDL_Texture* enemyWalk;
	SDL_Texture* enemyFly;
	SDL_Texture* apple;
	SDL_Texture* Coin;
};
