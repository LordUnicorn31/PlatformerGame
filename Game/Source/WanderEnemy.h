#pragma once
#include "Entity.h"
#include "Animation.h"

class WanderEnemy : public Dynamic
{
public:

	WanderEnemy();
	~WanderEnemy();
	void Update(float dt) override;
	void Draw(float dt)override;
	void UpdateLogic()override;

private:

	void Move()override;
	virtual void Die()override;
	bool Limits();
	iPoint initialPosition;
	Animations idleAnimation;
	Animations moveAnimation;
	Animations deathAnimation;
	Animations* currentAnimation = nullptr;

};