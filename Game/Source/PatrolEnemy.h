#pragma once
#include "Entity.h"
#include "Animation.h"

class PatrolEnemy : public Dynamic
{
public:

	PatrolEnemy(iPoint pos);
	~PatrolEnemy();
	void Update(float dt) override;
	void Draw(float dt)override;
	void UpdateLogic()override;

private:

	void Move()override;
	virtual void Die()override;
	iPoint initialPosition;
	Animations idleAnimation;
	Animations moveAnimation;
	Animations deathAnimation;
	Animations* currentAnimation = nullptr;
	float timeSinceSwap;

};
