#pragma once
#ifndef __Collisions_H__
#define __Collisions_H__

#define MAX_COLLIDERS 200

#include "Module.h"

class Entity;


enum ColliderType
{
	COLLIDER_NONE = -1,
	COLLIDER_ALLY,
	COLLIDER_ENEMY,
	COLLIDER_ATTACK,
	COLLIDER_COLLECTIBLE,
	COLLIDER_CHEST,

	NUM_COLLIDER_TYPES
};

struct Collider
{
	SDL_Rect rect;
	bool toDelete = false;
	bool active = true;
	ColliderType type;
	Module* callback = nullptr;
	Entity* entity = nullptr;
	//Collider();

	Collider(SDL_Rect rectangle, ColliderType type, Module* callback = nullptr, Entity* iEntity = nullptr) :rect(rectangle), type(type), callback(callback), entity(iEntity)
	{

	}
	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class Collisions : public Module
{
public:

	Collisions();
	~Collisions();

	void Init();
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	Collider* AddCollider(SDL_Rect rect, ColliderType type, Module* callback = nullptr, Entity* entity = nullptr);
	void DebugDraw();

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[NUM_COLLIDER_TYPES][NUM_COLLIDER_TYPES];
	bool debug = false;

};

#endif // __Collision_H__
