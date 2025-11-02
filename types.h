#ifndef TYPES_H
#define TYPES_H

#include <raylib.h>
#include <raymath.h>


#include "ruct_include.h"

enum Team {
    PLAYER,
    ENEMY,
};

typedef struct {
    Vector2 pos;
    Vector2 mov;
    f32 rot;
    enum Team team;
} RUCT_PREFIX(Projectile);
RUCT_DEFINE_CUSTOM_TYPE(Projectile);

enum AsteroidSize {
    SMALL,
    MEDIUM,
    LARGE,
};

typedef struct {
    Vector2 pos;
    Vector2 mov;
    f32 rot;
    enum AsteroidSize size;
} RUCT_PREFIX(Asteroid);
RUCT_DEFINE_CUSTOM_TYPE(Asteroid);

typedef struct {
    Ruct_VecProjectile projectiles;
    Ruct_VecF32 projectile_lifetime_timers;
    Ruct_VecAsteroid asteroids;
} World;

typedef struct {
    bool holding_right;
    bool holding_left;
    bool holding_up;
    bool holding_down;

    bool pressed_fire;
} PlayerInput;

typedef struct {
    Vector2 pos;
    Vector2 mov;
    f32 rot; // degrees
    PlayerInput input;
    enum Team team;
    World* world;
} Player;

#endif
