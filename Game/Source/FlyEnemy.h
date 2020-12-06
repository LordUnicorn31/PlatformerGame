#pragma once
#include "Entity.h"
#include "Animation.h"
#include "EntityManager.h"


class FlyEnemy : public Dynamic {
public:
	FlyEnemy();
	~FlyEnemy();
	void Update(float dt) override;
	void Draw(float dt)override;
	void UpdateLogic()override;
private:
	void Move();
	virtual void Die();
	bool inRadius();
	int attackRadius;
	iPoint initialPosition;
	Animations idleAnimation;
	Animations moveAnimation;
	Animations deathAnimation;
	Animations* currentAnimation = nullptr;
	
};