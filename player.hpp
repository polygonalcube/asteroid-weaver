#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Player : public Entity
{
	public:
		Player();

		char* sprite_path = "../Graphics/star_cutter.png";

		Vector4 movement_input_values;
		bool is_pressing_shoot;

		bool can_shoot = true;
		double shot_delay;
		double shot_delay_set;
		Vector2 bullet_speed;

		void move(Vector2 amount);
		void shoot(double dec);
};

#endif