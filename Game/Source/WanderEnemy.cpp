#include "WanderEnemy.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Map.h"

enum class WanderEnemyStates : unsigned char
{
	NONE,
	MOVE,
	WANDER,
	DIE,
};

WanderEnemy::WanderEnemy(iPoint pos) : Dynamic(EntityType::WANDER_ENEMY, pos)
{
	maxSpeed = 1.9f;
	a = 0.8f;
	terminalSpeed = 0.0f;
	initialPosition = pos;
	currentState = WanderEnemyStates::NONE;
	radius = 160;
	idleAnimation.PushBack({ 0, 96, 16, 16 });
	moveAnimation.PushBack({ 16, 96, 16, 16 });
	moveAnimation.PushBack({ 32, 96, 16, 16 });
	moveAnimation.speed = 6.0f;
	deathAnimation.PushBack({ 0, 96, 16, 16 });
	deathAnimation.PushBack({ 80, 96, 16, 16 });
}

WanderEnemy::~WanderEnemy(){}

void WanderEnemy::Update(float dt)
{

	switch (currentState)
	{
	case WanderEnemyStates::NONE:
		currentAnimation = &idleAnimation;

		if (CheckRadius(app->player->GetPosition()))
		{
			currentState = WanderEnemyStates::MOVE;
		}

	case WanderEnemyStates::MOVE:
		currentAnimation = &moveAnimation;
		
		if (!CheckRadius(app->player->GetPosition()))
		{
			currentState = WanderEnemyStates::NONE;
		}
	}

	Draw(dt);
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
	if (currentState == WanderEnemyStates::MOVE)
	{
		iPoint mapPos = Map::WorldToMap(pos.x, pos.y);
		if (Map::GetTileProperty((mapPos.y * Map::GetMapWidth() + mapPos.x), "Swap"))
		{ 
			maxSpeed = -maxSpeed;
			
		}

		Move();

	}
}

void WanderEnemy::Move()
{
	pos.x += maxSpeed;
}

void WanderEnemy::Die()
{

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