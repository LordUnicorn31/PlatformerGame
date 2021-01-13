#include "App.h"
#include "Chest.h"
#include "Render.h"
#include "Map.h"
#include "Player.h"
#include "Collisions.h"
#include "Input.h"
#include "EntityManager.h"
#include "Entity.h"


Chest::Chest(iPoint pos) : Static(EntityType::CHEST, pos)
{
	initialPosition = pos;

	idleChest.PushBack({ 0, 691, 16, 12 });
	openChest.PushBack({ 0, 704, 16, 12 });
	entityCollider = app->collisions->AddCollider({ pos.x, pos.y, 16, 16 }, ColliderType::COLLIDER_CHEST, app->entity, this);
	currentAnimation = &idleChest;
}

Chest::~Chest() {};

void Chest::Update(float dt)
{
	Draw(dt);
}

void Chest::UpdateLogic()
{

}

void Chest::Draw(float dt)
{

	app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);
	
}

void Chest::Die()
{
	currentAnimation = &openChest;
	if (entityCollider != nullptr)
	{
		entityCollider->toDelete = true;
		entityCollider = nullptr;
	}

}

