#pragma once
#include "Entity.h"
#include "Animation.h"
#include "EntityManager.h"
#include "Point.h"
#include "DynArray.h"

enum class FlyEnemyState : unsigned char;

class FlyEnemy : public Dynamic 
{
public:
	FlyEnemy(iPoint pos);
	~FlyEnemy();
	void Update(float dt) override;
	void Draw(float dt)override;
	void UpdateLogic()override;
private:
	void Move()override;
	virtual void Die();
	bool inRadius(iPoint pos);
	void CalculateNewPath(iPoint destination, float dt);
	void CalculateNewPathNow(iPoint destination);
	bool CalculateCurentDirection();
	bool ReachedTile();
	int attackRadius;
	iPoint initialPosition;
	Animations idleAnimation;
	Animations moveAnimation;
	Animations deathAnimation;
	Animations* currentAnimation = nullptr;
	DynArray<iPoint>path;
	iPoint currentDirection;
	FlyEnemyState currentState;
	iPoint destinationTile;

	float accumulatedTime;
	float updatePathms;
};