#include "player_bullet.hpp"

PlayerBullet::PlayerBullet(Vector2 pos, Vector2 spd)
{
	position = pos;
	speed = spd;
	can_delete = false;
}

void PlayerBullet::delete_offscreen()
{
	if (position.y < -32) can_delete = true;
}

void PlayerBullet::move(Vector2 amount)
{
	position.x += amount.x;
	position.y -= amount.y;

	PlayerBullet::delete_offscreen();
}