#include "Entity.h"

Entity::Entity(EntityType type) : etype(type), width(16), height(16) {}

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

Dynamic::Dynamic(EntityType type) :Entity(type), targetSpeed({0,0}) 
{

}