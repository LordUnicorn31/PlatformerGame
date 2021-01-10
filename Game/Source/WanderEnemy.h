#pragma once
#include "Entity.h"
#include "Animation.h"

class WanderEnemy : public Dynamic
{
public:

	WanderEnemy(iPoint pos);
	~WanderEnemy();
	void Update(float dt) override;
	void Draw(float dt)override;
	void UpdateLogic()override;

private:

	void Move()override;
	virtual void Die()override;
	bool Limits();
	bool CheckRadius(iPoint pos);
	iPoint initialPosition;
	Animations idleAnimation;
	Animations moveAnimation;
	Animations deathAnimation;
	Animations* currentAnimation = nullptr;
	int radius;

};