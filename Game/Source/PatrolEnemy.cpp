#include "PatrolEnemy.h"

enum class PatrolEnemyState
{

};

PatrolEnemy::PatrolEnemy(iPoint initialPos) : Dynamic(EntityType::PATROL_ENEMY)
{
	maxSpeed = 1.6f;
	a = 0.5f;
	terminalSpeed = 0.0f;
	pos = initialPos;
	initialPosition = initialPos;
}

PatrolEnemy::~PatrolEnemy(){}

void PatrolEnemy::Update(float dt)
{

}

void PatrolEnemy::Draw(float dt)
{

}

void PatrolEnemy::UpdateLogic()
{

}

void PatrolEnemy::Move()
{

}

void PatrolEnemy::Die()
{

}