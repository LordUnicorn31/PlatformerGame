#pragma once
#include "SDL_rect.h"
#include "Point.h"

struct SDL_Texture;

enum class EntityType : unsigned char 
{
	PLAYER,
	PATROL_ENEMY,
	WANDER_ENEMY,
	FLY_ENEMY,
	ITEM,
};

class Entity 
{
public:
	Entity(EntityType type, iPoint initialPos);
	virtual ~Entity();
	EntityType etype;
	iPoint pos;
	int width;
	int height;
	SDL_Texture* sprite;
	
public:
	virtual void Draw(float dt);
	virtual void Update(float dt);
	virtual void UpdateLogic();
};

class Dynamic : public Entity 
{
protected:
	Dynamic(EntityType type, iPoint initialPos);
	~Dynamic() {};
	virtual void Move() {};
	virtual void Die() {};
	float maxSpeed;
	fPoint targetSpeed;
	float terminalSpeed;
	float a;
};

class Static : public Entity
{
protected:
	Static(EntityType type, iPoint initialPos);
	~Static() {};
};

