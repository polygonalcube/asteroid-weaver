#include <SDL.h>
#include <SDL_image.h> // A library for better utilization of images.
#include <cmath>
#include <stdio.h> // Included for printf(); printf() has greater thread-safety than std::cout (& I'll pretend that's the reason I'm using it).
#include <algorithm>
#include <memory>
#include <iostream>
#include <vector>

#include "types.hpp"
#include "entity.hpp"
#include "asteroid.hpp"
#include "instructions.hpp"
#include "player.hpp"
#include "player_bullet.hpp"

double distance_between(Vector2& p1, Vector2& p2)
{
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}