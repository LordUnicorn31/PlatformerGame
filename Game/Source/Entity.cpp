#include "Entity.h"

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

Dynamic::Dynamic(EntityType type, iPoint initialPos) :Entity(type, initialPos), targetSpeed({0,0}) 
{

}