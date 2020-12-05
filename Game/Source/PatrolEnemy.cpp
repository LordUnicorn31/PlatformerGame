#include "PatrolEnemy.h"
#include "App.h"
#include "Render.h"

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
	idleAnimation.PushBack({ 0, 80, 16, 16 });
	moveAnimation.PushBack({ 16, 80, 16, 16 });
	moveAnimation.PushBack({ 32, 80, 16, 16 });
	deathAnimation.PushBack({ 0, 80, 16, 16 });
	deathAnimation.PushBack({ 80, 80, 16, 16 });
}

PatrolEnemy::~PatrolEnemy(){}

void PatrolEnemy::Update(float dt)
{
	//Idle animation by definition
	currentAnimation = &idleAnimation;
}

void PatrolEnemy::Draw(float dt)
{
	app->render->DrawTexture(this->sprite, initialPosition.x, initialPosition.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);
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