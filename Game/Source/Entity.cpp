#include "Entity.h"
#include "Collisions.h"

Entity::Entity(EntityType type, iPoint initialPos) : etype(type), pos(initialPos), width(16), height(16) {}

Entity::~Entity() 
{

}

void Entity::Update(float dt) 
{

}

void Entity::Draw(float dt) 
{

}

void Entity::UpdateLogic() 
{

}

void Entity::SetEntityCollider()
{
	entityCollider->SetPos(pos.x + 2, pos.y + 2);
}

Dynamic::Dynamic(EntityType type, iPoint initialPos) :Entity(type, initialPos), targetSpeed({0,0}) 
{

}

Static::Static(EntityType type, iPoint initialPos) : Entity(type, initialPos)
{

}