#pragma once
#include "SDL_rect.h"
#include "Point.h"

struct SDL_Texture;

enum class EntityType : unsigned char {
	PLAYER,
	PATROL_ENEMY,
	WANDER_ENEMY,
	FLY_ENEMY,
	ITEM,
};

class Entity {
public:
	Entity(EntityType type);
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

class Dynamic : public Entity {
protected:
	Dynamic(EntityType type);
	~Dynamic();
	virtual void Move();
	virtual void Die();
	//Collider* myCollider;
	//Collider* combatCollider;
	float maxSpeed;
	fPoint targetSpeed;
	float terminalSpeed;
	float a;
};

/*class Static : public Entity {

};*/