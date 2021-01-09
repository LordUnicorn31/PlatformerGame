#pragma once
#include "Entity.h"
#include "Animation.h"

struct Collider;
class Coin : public Static
{
public:
	Coin(iPoint pos);
	~Coin();
	void Update(float dt) override;
	void Draw(float dt) override;
	void UpdateLogic() override;
	void Die() override;
private:

	Animations spinCoin;
	Animations* currentAnimation = nullptr;
	iPoint initialPosition;
};