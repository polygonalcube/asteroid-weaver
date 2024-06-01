#include <SDL.h>
#include <SDL_image.h> // A library for better utilization of images.
#include <cmath>
#include <stdio.h> // Included for printf(); printf() has greater thread-safety than std::cout (& I'll pretend that's the reason I'm using it).
#include <time.h>
#include <algorithm>
#include <memory>
#include <iostream>
#include <vector>

#include "types.hpp"
#include "functions.hpp"
#include "entity.hpp"
#include "asteroid.hpp"
#include "instructions.hpp"
#include "player.hpp"
#include "player_bullet.hpp"

// Variable declarations.
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

Vector2 game_resolution = {180, 240};
double frame_ms_delay = 16.667;

bool game_running = true;
bool in_gameplay = false;
double time_elapsed = 0.0;

Player player;

std::vector<std::unique_ptr<PlayerBullet>> player_bullets;

std::vector<std::unique_ptr<Asteroid>> asteroids;
std::vector<int> asteroid_speeds = {-1, -1, -1, -1, -1, -2, -2, -2, -2, -3};

double asteroid_spawn_delay = 0.0;
double asteroid_spawn_delay_set = 0.04;

Vector2 instructions_pos = {(90 - 32), (120 - 32)};
Instructions instructions(instructions_pos);

void reset_game()
{
    srand(time(0));
    player.position = {(90 - 16), (240 - 32)};
    player.health = 1;

    if (player_bullets.size() > 0)
    {
        for (int i = player_bullets.size() - 1; i >= 0; --i)
        {
            player_bullets.erase(player_bullets.end());
        }
    }
    if (asteroids.size() > 0)
    {
        for (int i = (asteroids.size() - 1); i >= 0; --i)
        {
            asteroids.erase(asteroids.end());
        }
    }
}

void end_session(int exit_status = 0)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;
    renderer = nullptr;

    IMG_Quit();
    SDL_Quit();
    printf("SDL terminated with exit status %d.\n", exit_status);
    exit(exit_status);
}

void get_key_down(SDL_KeyboardEvent* key_event_down)
{
    if (key_event_down->repeat == 0)
    {
        if (key_event_down->keysym.scancode == SDL_SCANCODE_A) player.movement_input_values.left = 1;
        if (key_event_down->keysym.scancode == SDL_SCANCODE_D) player.movement_input_values.right = 1;
        if (key_event_down->keysym.scancode == SDL_SCANCODE_S) player.movement_input_values.down = 1;
        if (key_event_down->keysym.scancode == SDL_SCANCODE_W) player.movement_input_values.up = 1;

        if (key_event_down->keysym.scancode == SDL_SCANCODE_LALT) player.is_pressing_shoot = true;
    }
}

void get_key_up(SDL_KeyboardEvent* key_event_up)
{
    if (key_event_up->repeat == 0)
    {
        if (key_event_up->keysym.scancode == SDL_SCANCODE_A) player.movement_input_values.left = 0;
        if (key_event_up->keysym.scancode == SDL_SCANCODE_D) player.movement_input_values.right = 0;
        if (key_event_up->keysym.scancode == SDL_SCANCODE_S) player.movement_input_values.down = 0;
        if (key_event_up->keysym.scancode == SDL_SCANCODE_W) player.movement_input_values.up = 0;

        if (key_event_up->keysym.scancode == SDL_SCANCODE_LALT) player.is_pressing_shoot = false;
    }
}

void handle_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                end_session(0);
                break;
            case SDL_KEYDOWN:
                if (in_gameplay) get_key_down(&event.key);
                else
                {
                    reset_game();
                    in_gameplay = true;
                }
                break;
            case SDL_KEYUP:
                get_key_up(&event.key);
                break;
            default:
                break;
        }
    }
}

void spawn_asteroid()
{
    asteroid_spawn_delay = asteroid_spawn_delay_set;
    Vector2 random_pos = {(rand() % 181), -32};
    Vector2 asteroid_vec = {((rand() % 5) - 2), asteroid_speeds[rand() % 10]};
    asteroids.push_back(std::make_unique<Asteroid>(random_pos, asteroid_vec));
}

void do_game_logic()
{
    Vector2 player_move = {((player.movement_input_values.right - player.movement_input_values.left) * player.speed.x), ((player.movement_input_values.up - player.movement_input_values.down) * player.speed.y)};
    player.move(player_move);

    if (player.position.x < 0) player.position.x = 0;
    else if (player.position.x > (game_resolution.x - 32)) player.position.x = (game_resolution.x - 32);
    if (player.position.y < 0) player.position.y = 0;
    else if (player.position.y > (game_resolution.y - 32)) player.position.y = (game_resolution.y - 32);

    player.shoot(frame_ms_delay);
    if (player.is_pressing_shoot && player.shot_delay <= 0)
    {
        player.shot_delay = player.shot_delay_set;
        player_bullets.push_back(std::make_unique<PlayerBullet>(player.position, player.bullet_speed));
    }

    for (std::unique_ptr<PlayerBullet>& bullet : player_bullets)
    {
        bullet->move(bullet->speed);
    }
    if (player_bullets.size() > 0)
    {
        for (int i = player_bullets.size() - 1; i >= 0; --i)
        {
            if (player_bullets[i]->can_delete)
            {
                auto offscreen_bullet = std::find(player_bullets.begin(), player_bullets.end(), player_bullets[i]); 
                player_bullets.erase(offscreen_bullet);
            }
        }
    }

    asteroid_spawn_delay -= (1.0/60.0);
    if (asteroid_spawn_delay <= 0) spawn_asteroid();

    for (std::unique_ptr<Asteroid>& asteroid : asteroids)
    {
        asteroid->move(asteroid->speed);

        if (distance_between(asteroid->position, player.position) < 12.0)
        {
            player.health--;
            asteroid->can_delete = true;
        }
        for (std::unique_ptr<PlayerBullet>& bullet : player_bullets)
        {
            if (distance_between(asteroid->position, bullet->position) < 20.0)
            {
                asteroid->health--;
                bullet->can_delete = true;
            }
        }
    }
    if (asteroids.size() > 0)
    {
        for (int i = asteroids.size() - 1; i >= 0; --i)
        {
            if (asteroids[i]->can_delete)
            {
                auto offscreen_asteroid = std::find(asteroids.begin(), asteroids.end(), asteroids[i]); 
                asteroids.erase(offscreen_asteroid);
            }
        }
    }

    if (player.health <= 0) in_gameplay = false;
}

void display(char* filename, int x, int y)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, filename);

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);

    SDL_RenderCopy(renderer, texture, nullptr, &dest);
}

void render_player()
{
    display(player.sprite_path, player.position.x, player.position.y);
}

void render_bullets()
{
    for (std::unique_ptr<PlayerBullet>& bullet : player_bullets)
    {
        display(bullet->sprite_path, bullet->position.x, bullet->position.y);
    }
}

void render_asteroids()
{
    for (std::unique_ptr<Asteroid>& asteroid : asteroids)
    {
        display(asteroid->sprite_path, asteroid->position.x, asteroid->position.y);
    }
}

void render_instructions()
{
    display(instructions.sprite_path, instructions.position.x, instructions.position.y);
}

void render_everything()
{
    if (in_gameplay)
    {
        render_asteroids();
        render_bullets();
        render_player();
    }
    else render_instructions();
}

void render_game()
{
    SDL_RenderClear(renderer);
    render_everything();
    SDL_RenderPresent(renderer);
    SDL_Delay(frame_ms_delay);
}

int main(int argc, char *argv[])
{
    // Initialize SDL2. A value below 0 indicates failure.
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL initialization failed.\n");
        printf(SDL_GetError(), "\n");
        end_session(1); 
    }
    printf("SDL initialized.\n");

    // Initialize SDL_Image. Like SDL_Init(), a value below 0 indicates failure.
    if (!IMG_Init(IMG_INIT_PNG) < 0)
    {
        printf("SDL_Image initialization failed.\n");
        printf(SDL_GetError(), "\n");
        end_session(1); 
    }
    printf("SDL_Image initialized.\n");

    // https://wiki.libsdl.org/SDL2/SDL_CreateWindow for documentation.
    //                        title                     X position              Y position              width height SDL_WINDOWFLAGS
    window = SDL_CreateWindow("Star Cutter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (game_resolution.x * 4), (game_resolution.y * 4), SDL_WINDOW_SHOWN);

    // https://wiki.libsdl.org/SDL2/SDL_CreateRenderer for documentation.
    //                            window
    //                                    index; '-1' means use the first graphics device found
    //                                        SDL_RENDERERFLAGS
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, game_resolution.x, game_resolution.y);
    //                               r  g  b  a
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    while (game_running)
    {
        handle_input();
        if (in_gameplay) do_game_logic();
        render_game();
        time_elapsed += (1.0/60.0);
        //if (time_elapsed >= 300.0) game_running = false;
    }
    end_session(0);
    return 0;
}