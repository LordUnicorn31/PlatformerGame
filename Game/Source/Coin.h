#pragma once
#include "Entity.h"
#include "Animation.h"

class Coin : public Static
{
public:
	Coin(iPoint pos);
	~Coin();
	void Update(float dt) override;
	void Draw(float dt)override;
	void UpdateLogic()override;
private:

	Animations spinCoin;
	Animations* currentAnimation = nullptr;
	iPoint initialPosition;
};