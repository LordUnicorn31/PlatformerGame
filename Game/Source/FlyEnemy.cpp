#include "App.h"
#include "FlyEnemy.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Render.h"
#include "Timer.h"
#include "Map.h"

#define ATTACK_RADIUS 10

enum class FlyEnemyState : unsigned char 
{
	NONE,
	ATTACK,
	RETURN
};

FlyEnemy::FlyEnemy(iPoint pos) : Dynamic(EntityType::FLY_ENEMY)
{
	a = 0.7f;
	maxSpeed = 1.0f;
	terminalSpeed = 0.0f;
	// Random initial position.
	initialPosition = pos;
	idleAnimation.PushBack({ 192, 131, 16, 8 });
	moveAnimation.PushBack({ 192, 131, 16, 8 });
	moveAnimation.PushBack({ 208, 131, 16, 8 });
	deathAnimation.PushBack({ 192, 131, 16, 8 });
	deathAnimation.PushBack({ 80, 80,  16, 8 });
	
	accumulatedTime = 0.0f;
	updatePathms = 16.66666666f; //A 60 fps
	currentState = FlyEnemyState::NONE;
	currentDirection = { 0,0 };
	destinationTile = { 0,0 };
	attackRadius = ATTACK_RADIUS * Map::GetTileWidth();
}

FlyEnemy::~FlyEnemy(){}

void FlyEnemy::Update(float dt)
{

	switch (currentState) 
	{
	case FlyEnemyState::NONE:
		currentAnimation = &idleAnimation;
		if (inRadius(app->player->GetPosition()))
		{
			calculateNewPathNow(app->player->GetPosition());
			currentState = FlyEnemyState::ATTACK;
			break;
		}
		break;
	case FlyEnemyState::ATTACK:
		currentAnimation = &moveAnimation;
		if (!inRadius(app->player->GetPosition()) && !inRadius(pos))
		{
			calculateNewPathNow(initialPosition);
			currentState = FlyEnemyState::RETURN;
			break;
		}
		calculateNewPath(app->player->GetPosition(),dt);
		//Mourem cap aquell path
		break;
	case FlyEnemyState::RETURN:
		currentAnimation = &moveAnimation;
		if (inRadius(app->player->GetPosition()))
		{
			calculateNewPathNow(app->player->GetPosition());
			currentState = FlyEnemyState::ATTACK;
			break;
		}
		else if (pos == initialPosition) 
		{
			currentState = FlyEnemyState::NONE;
			break;
		}
		calculateNewPath(initialPosition,dt);
		break;
	}
	
	Draw(dt);
}



bool FlyEnemy::inRadius(iPoint pos)
{
	return ((pos.x - initialPosition.x) ^ 2 + (pos.y - initialPosition.y) ^ 2) < (attackRadius ^ 2);
}

void FlyEnemy::calculateNewPath(iPoint destination, float dt)
{
	accumulatedTime += dt * 1000.0f;
	if (accumulatedTime > updatePathms) 
	{
		Pathfinding::CreatePath(Map::WorldToMap(pos.x, pos.y), Map::WorldToMap(destination.x, destination.y));
		path = *Pathfinding::GetLastPath();
		iPoint first;
		path.Pop(first);
		accumulatedTime = 0.0f;
		CalculateCurentDirection();
	}
}

void FlyEnemy::calculateNewPathNow(iPoint destination)
{
	Pathfinding::CreatePath(Map::WorldToMap(pos.x, pos.y), Map::WorldToMap(destination.x, destination.y));
	path = *Pathfinding::GetLastPath();
	iPoint first;
	path.Pop(first);
	accumulatedTime = 0.0f;
	CalculateCurentDirection();
}

bool FlyEnemy::CalculateCurentDirection()
{
	if (path.Count() <= 0) 
	{
		return false;
	}
	iPoint mapPosition = Map::WorldToMap(pos.x, pos.y);
	path.Pop(destinationTile);
	currentDirection.x = destinationTile.x - mapPosition.x;
	currentDirection.y = mapPosition.y - destinationTile.y;
	return true;
}

bool FlyEnemy::ReachedTile()
{	
	iPoint destinationPos;
	destinationPos.x = destinationTile.x * Map::GetTileWidth();
	destinationPos.y = destinationTile.y * Map::GetTileWidth();

	if (currentDirection.x == 1 && currentDirection.y == 0)
	{
		if (pos.x > destinationPos.x) 
		{
			pos = destinationPos;
			return true;
		}
	}
	else if (currentDirection.x == 0 && currentDirection.y == 1)
	{
		if (pos.y > destinationPos.y) 
		{
			pos = destinationPos;
			return true;
		}
	}
	else if (currentDirection.x == 1 && currentDirection.y == 1)
	{
		if (pos.x > destinationPos.x && pos.y > destinationPos.y) 
		{
			pos = destinationPos;
			return true;
		}
	}
	else if (currentDirection.x == -1 && currentDirection.y == 0)
	{
		if (pos.x < destinationPos.x)
		{
			pos = destinationPos;
			return true;
		}
	}
	else if (currentDirection.x == 0 && currentDirection.y == -1)
	{
		if (pos.y < destinationPos.y)
		{
			pos = destinationPos;
			return true;
		}
	}
	else if (currentDirection.x == -1 && currentDirection.y == -1)
	{
		if (pos.x < destinationPos.x && pos.y < destinationPos.y)
		{
			pos = destinationPos;
			return true;
		}
	}
	else if (currentDirection.x == -1 && currentDirection.y == 1)
	{
		if (pos.x < destinationPos.x && pos.y > destinationPos.y)
		{
			pos = destinationPos;
			return true;
		}
	}
	else if (currentDirection.x == 1 && currentDirection.y == -1)
	{
		if (pos.x > destinationPos.x && pos.y < destinationPos.y)
		{
			pos = destinationPos;
			return true;
		}
	}

	return false;
}



void FlyEnemy::Draw(float dt)
{
	app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);
	/*for (int i = 0; i != path.Count(); ++i) {
		app->render->DrawRectangle({ path[i].x * 16,path[i].y * 16,16,16 }, 255, 0, 0, 127, true);
	}*/
}

void FlyEnemy::UpdateLogic()
{
	/*speed.x += a * currentDirection.x;
	speed.y += a * currentDirection.y;
	if (Sign(targetSpeed.x - speed.x) != currentDirection.x)
		speed.x = targetSpeed.x;
	if (Sign(targetSpeed.y - speed.y) != currentDirection.y)
		speed.y = targetSpeed.y;*/
	
	if(currentState != FlyEnemyState::NONE)
		Move();
}

void FlyEnemy::Move()
{
	//pos.x += maxSpeed * currentDirection.x;
	//pos.y += maxSpeed * currentDirection.y;

	if(ReachedTile())
	{
		if (!CalculateCurentDirection()) 
		{
			currentState = FlyEnemyState::NONE;
		}
	}
}

void FlyEnemy::Die()
{

}
