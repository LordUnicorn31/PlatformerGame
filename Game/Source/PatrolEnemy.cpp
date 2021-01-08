#include "PatrolEnemy.h"
#include "App.h"
#include "Render.h"
#include "Map.h"
#include "Collisions.h"
#include "EntityManager.h"

enum class PatrolEnemyState
{

};

PatrolEnemy::PatrolEnemy(iPoint pos) : Dynamic(EntityType::PATROL_ENEMY, pos)
{
	maxSpeed = 1.0f;
	a = 0.5f;
	terminalSpeed = 0.0f;
	initialPosition = pos;
	idleAnimation.PushBack({ 0, 80, 16, 16 });
	moveAnimation.PushBack({ 16, 80, 16, 16 });
	moveAnimation.PushBack({ 32, 80, 16, 16 });
	moveAnimation.speed = 2.0f;
	deathAnimation.PushBack({ 0, 80, 16, 16 });
	deathAnimation.PushBack({ 80, 80, 16, 16 });
	timeSinceSwap = 0.0f;
	entityCollider = app->collisions->AddCollider({ pos.x + 2, pos.y + 2, width - 2, height }, COLLIDER_TYPE::COLLIDER_ENEMY, app->entity);
}

PatrolEnemy::~PatrolEnemy(){}

void PatrolEnemy::Update(float dt)
{
	//Idle animation by definition
	//timeSinceSwap += dt * 1000;
	//iPoint mapPos = Map::WorldToMap(pos.x, pos.y);
	/*//TODO: problema amb el time ()
	if ((timeSinceSwap > 8.3333f) && Map::GetTileProperty((mapPos.y * Map::GetMapWidth() + mapPos.x), "Swap")) 
	{
		maxSpeed = -maxSpeed;
		timeSinceSwap = 0.0f;
	}*/
	currentAnimation = &moveAnimation;
	//Draw(dt);
}

void PatrolEnemy::Draw(float dt)
{
	if(maxSpeed > 0)
		app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);
	else
		app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f,SDL_FLIP_HORIZONTAL);
}

void PatrolEnemy::UpdateLogic()
{
	iPoint mapPos = Map::WorldToMap(pos.x, pos.y);
	if (Map::GetTileProperty((mapPos.y * Map::GetMapWidth() + mapPos.x), "Swap"))
		maxSpeed = -maxSpeed;
	Move();
}

void PatrolEnemy::Move()
{
	pos.x += maxSpeed;
	SetEntityCollider();
}

void PatrolEnemy::Die()
{

}