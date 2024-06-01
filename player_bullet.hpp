#ifndef PLAYER_BULLET_HPP
#define PLAYER_BULLET_HPP

#include "entity.hpp"

class PlayerBullet : public Entity
{
	public:
		PlayerBullet(Vector2 pos, Vector2 spd);

		char* sprite_path = "../Graphics/bullet_player.png";

		void move(Vector2 amount);
		void delete_offscreen();
};

#endif