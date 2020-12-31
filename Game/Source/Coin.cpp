#include "App.h"
#include "Coin.h"
#include "Render.h"
#include "Map.h"

Coin::Coin(iPoint pos) : Static(EntityType::ITEM, pos)
{
	initialPosition = pos;
	spinCoin.PushBack({ 50, 755, 10, 10 });
}

Coin::~Coin() {};

void Coin::Update(float dt)
{
	currentAnimation = &spinCoin;
	Draw(dt);
}

void Coin::UpdateLogic()
{

}

void Coin::Draw(float dt)
{
	app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);
}