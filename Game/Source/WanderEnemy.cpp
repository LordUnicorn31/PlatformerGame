#include "WanderEnemy.h"
#include "App.h"
#include "Render.h"


enum class WanderEnemyStates
{

};

WanderEnemy::WanderEnemy(iPoint pos) : Dynamic(EntityType::WANDER_ENEMY)
{
	maxSpeed = 1.9f;
	a = 0.8f;
	terminalSpeed = 0.0f;
	initialPosition = pos;
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
	currentAnimation = &idleAnimation;
	Draw(dt);
}

void WanderEnemy::Draw(float dt)
{
	app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);
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