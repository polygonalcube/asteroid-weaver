#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "entity.hpp"

class Asteroid : public Entity
{
	public:
		Asteroid(Vector2 pos, Vector2 spd);

		char* sprite_path = "../Graphics/asteroid.png";

		void move(Vector2 amount);
		void delete_offscreen();
};

#endif