#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include "entity.hpp"

class Instructions : public Entity
{
	public:
		Instructions(Vector2 pos);
		char* sprite_path = "../Graphics/instructions.png";
};

#endif

