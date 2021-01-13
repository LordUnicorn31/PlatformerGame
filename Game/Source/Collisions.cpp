#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Module.h"
#include "Log.h"
#include "Entity.h"
#include "Scene.h"
#include "Map.h"


Collisions::Collisions()
{
	name = "collisions";

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_ALLY][COLLIDER_ALLY] = false;
	matrix[COLLIDER_ALLY][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_ALLY][COLLIDER_COLLECTIBLE] = true;
	matrix[COLLIDER_ALLY][COLLIDER_ATTACK] = false;
	matrix[COLLIDER_ALLY][COLLIDER_CHEST] = true;

	matrix[COLLIDER_ENEMY][COLLIDER_ALLY] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_COLLECTIBLE] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ATTACK] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_CHEST] = false;

	matrix[COLLIDER_COLLECTIBLE][COLLIDER_ALLY] = true;
	matrix[COLLIDER_COLLECTIBLE][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_COLLECTIBLE][COLLIDER_COLLECTIBLE] = false;
	matrix[COLLIDER_COLLECTIBLE][COLLIDER_ATTACK] = true;
	matrix[COLLIDER_COLLECTIBLE][COLLIDER_CHEST] = false;

	matrix[COLLIDER_ATTACK][COLLIDER_ALLY] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_ATTACK][COLLIDER_COLLECTIBLE] = true;
	matrix[COLLIDER_ATTACK][COLLIDER_ATTACK] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_CHEST] = false;

	matrix[COLLIDER_CHEST][COLLIDER_ALLY] = true;
	matrix[COLLIDER_CHEST][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_CHEST][COLLIDER_COLLECTIBLE] = false;
	matrix[COLLIDER_CHEST][COLLIDER_ATTACK] = false;
	matrix[COLLIDER_CHEST][COLLIDER_CHEST] = false;
}

// Destructor
Collisions::~Collisions()
{}

void Collisions::Init() 
{
	enabled = true;
	active = true;
}

bool Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->toDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	// Calculate collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect))
			{
				if (matrix[c1->type][c2->type] && c1->callback && c1->active)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback && c2->active)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	return 1;
}

// Called before render is available
bool Collisions::Update(float dt)
{

	DebugDraw();

	return 1;
}

void Collisions::DebugDraw()
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false) 
		return;

	Uint8 alpha = 140;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;
		if (colliders[i]->active) 
		{
			switch (colliders[i]->type)
			{
			case COLLIDER_COLLECTIBLE: // blue
				app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha);
				break;
			
			case COLLIDER_ALLY: // green
				app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha);
				break;
			case COLLIDER_ENEMY: //red
				app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha);
				break;
			case COLLIDER_ATTACK: //purple
				app->render->DrawRectangle(colliders[i]->rect, 255, 0, 255, alpha);
				break;
			case COLLIDER_CHEST:
				app->render->DrawRectangle(colliders[i]->rect, 0, 255, 255, alpha);
				break;
			}
		}
	}
}

// Called before quitting
bool Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, ColliderType type, Module* callback, Entity* entity)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback, entity);
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return SDL_HasIntersection(&rect, &r);
}
