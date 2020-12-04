#include "App.h"
#include "FlyEnemy.h"
#include "Pathfinding.h"
#include "Map.h"
#include "Player.h"

enum class FlyEnemyState {

};

FlyEnemy::FlyEnemy(iPoint initialPos): Dynamic(EntityType::FLY_ENEMY)
{
	a = 0.7f;
	maxSpeed = 2.0f;
	terminalSpeed = 0.0f;
	pos = initialPos;
	initialPosition = initialPos;
	//attackRadius = 
}

FlyEnemy::~FlyEnemy(){}

void FlyEnemy::Update(float dt)
{
	if (inRadius()) 
	{
		//s'ha de timejar
		Pathfinding::CreatePath(pos, app->player->GetPosition());
	}

}

bool FlyEnemy::inRadius()
{
	return ((app->player->GetPosition().x - initialPosition.x) ^ 2 + (app->player->GetPosition().y - initialPosition.y) ^ 2) < (attackRadius ^ 2);
}

void FlyEnemy::Draw(float dt)
{
}

void FlyEnemy::UpdateLogic()
{
	Move();
}

void FlyEnemy::Move()
{

}

void FlyEnemy::Die()
{
}
