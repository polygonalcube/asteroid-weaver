#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "types.hpp"

class Entity
{
	public:
		Vector2 position;
		Vector2 speed;
		int health = 1;
		char* sprite_path = "";
		bool can_delete;

		void move(Vector2 amount);
		void check_health();
};

#endif

