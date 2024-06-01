#include "player.hpp"

Player::Player()
{
	position = {(90 - 16), (240 - 32)};
	speed = {4, 4};
	is_pressing_shoot = false;
	shot_delay_set = 150.0;
	bullet_speed = {0, 16};
	health = 1;
}

void Player::move(Vector2 amount)
{
	position.x += amount.x;
	position.y -= amount.y;
}

void Player::shoot(double dec)
{
	if (shot_delay > 0.0) shot_delay -= dec;
}