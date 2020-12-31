#include "App.h"
#include "Chest.h"
#include "Render.h"
#include "Map.h"
#include "Player.h"
#include "Input.h"

Chest::Chest(iPoint pos) : Static(EntityType::CHEST, pos)
{
	initialPosition = pos;

	idleChest.PushBack({ 0, 691, 16, 12 });
	openChest.PushBack({ 0, 704, 16, 12 });
}

Chest::~Chest() {};

void Chest::Update(float dt)
{
	ChestState();

	Draw(dt);
}

void Chest::UpdateLogic()
{

}

void Chest::Draw(float dt)
{
	app->render->DrawTexture(this->sprite, pos.x, pos.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);
}

void Chest::ChestState()
{
	if (pos == app->player->GetPosition() && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		// The chest is open
		currentAnimation = &openChest;
	}

	//The chest is closed

	else currentAnimation = &idleChest;
}