#ifndef TYPES_HPP
#define TYPES_HPP

typedef struct Vector2
{
    int x = 0;
    int y = 0;
}
Vector2;

typedef struct Vector3
{
    int x = 0;
    int y = 0;
    int z = 0;
}
Vector3;

typedef struct Vector4
{
    int x = 0;
    int y = 0;
    int z = 0;
    int w = 0;

    int& left = x;
    int& right = y;
    int& down = z;
    int& up = w;
}
Vector4;

#endif