#pragma once
#include "Entity.h"

class WanderEnemy : public Dynamic
{
public:

	WanderEnemy(iPoint initialPos);
	~WanderEnemy();
	void Update(float dt) override;
	void Draw(float dt)override;
	void UpdateLogic()override;

private:

	void Move()override;
	virtual void Die()override;
	bool Limits();
	iPoint initialPosition;

};