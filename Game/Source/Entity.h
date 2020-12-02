#pragma once
#include "SDL_rect.h"

struct SDL_Texture;



enum class EntityType : unsigned char {
	PLAYER,
	ENEMY_WALK,
	ENEMY_FLY,
	ITEM,
};

class Entity {
public:
	Entity(EntityType type);
	virtual ~Entity();
	EntityType etype;
	float health;
	int maxHealth;
	SDL_Texture* sprite;
	
public:
	virtual void Draw(float dt);
	virtual void Update(float dt);
	virtual void UpdateLogic();
};