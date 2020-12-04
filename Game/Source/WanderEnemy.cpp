#include "WanderEnemy.h"

enum class WanderEnemyStates
{

};

WanderEnemy::WanderEnemy(iPoint initialPos) : Dynamic(EntityType::WANDER_ENEMY)
{
	maxSpeed = 1.9f;
	a = 0.8f;
	terminalSpeed = 0.0f;
	pos = initialPos;
	initialPosition = initialPos;
}

WanderEnemy::~WanderEnemy(){}

void WanderEnemy::Update(float dt)
{

}

void WanderEnemy::Draw(float dt)
{

}

void WanderEnemy::UpdateLogic()
{

}

void WanderEnemy::Move()
{

}

void WanderEnemy::Die()
{

}

bool WanderEnemy::Limits()
{
	// Funcio per fer check dels limits on l'enemy pot caminar.
	return true;
}