#pragma once
#include "Entity.h"
#include "Animation.h"

class FlyEnemy : public Dynamic {
public:
	FlyEnemy(iPoint initialPos);
	~FlyEnemy();
	void Update(float dt) override;
	void Draw(float dt)override;
	void UpdateLogic()override;
private:
	void Move()override;
	virtual void Die()override;
	bool inRadius();
	int attackRadius;
	iPoint initialPosition;
	Animations idleAnimation;
	Animations moveAnimation;
	Animations deathAnimation;
	Animations* currentAnimation = nullptr;
};