#include "App.h"
#include "Coin.h"
#include "Render.h"
#include "Map.h"
#include "Collisions.h"
#include "Entity.h"
#include "EntityManager.h"

Coin::Coin(iPoint pos) : Static(EntityType::COIN, pos)
{
	initialPosition = pos;
	spinCoin.PushBack({ 50, 755, 10, 10 });
	entityCollider = app->collisions->AddCollider({ pos.x, pos.y, 16, 16 }, ColliderType::COLLIDER_COLLECTIBLE, app->entity, this);
}

Coin::~Coin() 
{
	if (entityCollider != nullptr)
		entityCollider->toDelete = true;
}

void Coin::Update(float dt)
{
	currentAnimation = &spinCoin;
	Draw(dt);
}

void Coin::UpdateLogic()
{

}

void Coin::Die()
{
	if (entityCollider != nullptr) 
	{
		entityCollider->toDelete = true;
		entityCollider = nullptr;
		toDie = true;
	}
}

void Coin::Draw(float dt)
{
	app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);
}