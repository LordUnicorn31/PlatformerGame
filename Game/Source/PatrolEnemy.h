#pragma once
#include "Entity.h"

class PatrolEnemy : public Dynamic
{
public:

	PatrolEnemy(iPoint initialPos);
	~PatrolEnemy();
	void Update(float dt) override;
	void Draw(float dt)override;
	void UpdateLogic()override;

private:

	void Move()override;
	virtual void Die()override;
	iPoint initialPosition;

};
