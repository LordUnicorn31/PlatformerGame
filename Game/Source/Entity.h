#pragma once
#include "SDL_rect.h"
#include "Point.h"

struct SDL_Texture;
struct Collider;

enum class EntityType : unsigned char 
{
	PLAYER,
	PATROL_ENEMY,
	WANDER_ENEMY,
	FLY_ENEMY,
	COIN,
	CHEST
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
	Collider* entityCollider;
	bool toDie;
	
public:
	virtual void Draw(float dt);
	virtual void Update(float dt);
	virtual void Die() {};
	virtual void UpdateLogic();
protected:
	void SetEntityCollider();
};

class Dynamic : public Entity 
{
protected:
	Dynamic(EntityType type, iPoint initialPos);
	~Dynamic() {};
	virtual void Move() {};
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

