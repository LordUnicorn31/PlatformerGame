#pragma once
#include "Entity.h"
#include "Animation.h"

class Chest : public Static
{
public:
	Chest(iPoint pos);
	~Chest();
	void Update(float dt) override;
	void Draw(float dt)override;
	void UpdateLogic()override;
private:

	void ChestState();
	Animations idleChest;
	Animations openChest;
	Animations* currentAnimation = nullptr;
	iPoint initialPosition;
};
