#include "entity.hpp"

void Entity::move(Vector2 amount)
{
	position.x += amount.x;
	position.y -= amount.y;
}

void Entity::check_health()
{
	if (health <= 0) can_delete = true;
}