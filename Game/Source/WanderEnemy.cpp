#include "WanderEnemy.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Map.h"
#include "EntityManager.h"
#include "Collisions.h"

WanderEnemy::WanderEnemy(iPoint pos) : Dynamic(EntityType::WANDER_ENEMY, pos)
{
	maxSpeed = 1.0f;
	a = 0.8f;
	terminalSpeed = 0.0f;
	initialPosition = pos;
	radius = 160;
	idleAnimation.PushBack({ 0, 96, 16, 16 });
	moveAnimation.PushBack({ 16, 96, 16, 16 });
	moveAnimation.PushBack({ 32, 96, 16, 16 });
	moveAnimation.speed = 6.0f;
	deathAnimation.PushBack({ 0, 96, 16, 16 });
	deathAnimation.PushBack({ 80, 96, 16, 16 });
	entityCollider = app->collisions->AddCollider({ pos.x +2, pos.y +2, width -2, height }, ColliderType::COLLIDER_ENEMY, app->entity, this);
}

WanderEnemy::~WanderEnemy(){}

void WanderEnemy::Update(float dt)
{
	currentAnimation = &moveAnimation;
}

void WanderEnemy::Draw(float dt)
{
	if (maxSpeed > 0)
		app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);
	else
		app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f, SDL_FLIP_HORIZONTAL);

	
}

void WanderEnemy::UpdateLogic()
{
	iPoint mapPos = Map::WorldToMap(pos.x, pos.y);
	if (Map::GetTileProperty((mapPos.y * Map::GetMapWidth() + mapPos.x), "Swap"))
		maxSpeed = -maxSpeed;
	Move();
}

void WanderEnemy::Move()
{
	/*while (app->player->GetPosition().x < pos.x) -maxSpeed;

	while (app->player->GetPosition().x > pos.x) maxSpeed;*/

	pos.x += maxSpeed;
	SetEntityCollider();
}

void WanderEnemy::Die()
{
	if (entityCollider != nullptr)
	{
		entityCollider->toDelete = true;
		entityCollider = nullptr;
		toDie = true;
	}
}

bool WanderEnemy::Limits()
{
	// Funcio per fer check dels limits on l'enemy pot caminar.
	return true;
}

bool WanderEnemy::CheckRadius(iPoint pos)
{
	return radius >= initialPosition.DistanceTo(pos);
}