#include "asteroid.hpp"

Asteroid::Asteroid(Vector2 pos, Vector2 spd)
{
	position = pos;
	speed = spd;
	health = 3;
	can_delete = false;
}

void Asteroid::delete_offscreen()
{
	if (position.y >= 240 || position.x <= -32 || position.x >= 180) can_delete = true;
}

void Asteroid::move(Vector2 amount)
{
	position.x += amount.x;
	position.y -= amount.y;

	Entity::check_health();
	Asteroid::delete_offscreen();
}