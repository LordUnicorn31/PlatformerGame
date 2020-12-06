#include "App.h"
#include "FlyEnemy.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Render.h"



enum class FlyEnemyState {

};

FlyEnemy::FlyEnemy() : Dynamic(EntityType::FLY_ENEMY)
{
	a = 0.7f;
	maxSpeed = 2.0f;
	terminalSpeed = 0.0f;
	// Random initial position.
	initialPosition = { 54, 2992 };
	pos = initialPosition;
	idleAnimation.PushBack({ 192, 131, 16, 8 });
	moveAnimation.PushBack({ 192, 131, 16, 8 });
	moveAnimation.PushBack({ 208, 131, 16, 8 });
	deathAnimation.PushBack({ 192, 131, 16, 8 });
	deathAnimation.PushBack({ 80, 80,  16, 8 });
	// attackradius
}

FlyEnemy::~FlyEnemy(){}

void FlyEnemy::Update(float dt)
{
	currentAnimation = &idleAnimation;

	if (inRadius()) 
	{
		//s'ha de timejar
		/*Pathfinding::CreatePath(pos, app->player->GetPosition());*/
	}
	
	Draw(dt);

}

bool FlyEnemy::inRadius()
{
	return ((app->player->GetPosition().x - initialPosition.x) ^ 2 + (app->player->GetPosition().y - initialPosition.y) ^ 2) < (attackRadius ^ 2);
}

void FlyEnemy::Draw(float dt)
{
	app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);
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
