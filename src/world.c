#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

#include "types.h"
#include "conf.h"
#include "util.h"

void Projectile_spawn(World* world, Vector2 pos, f32 rot, enum Team team) {
    Vector2 mov = { .x = PROJECTILE_SPEED, .y = PROJECTILE_SPEED };
    Ruct_Projectile proj = {.pos = pos, .mov = mov, .rot = rot, .team = team };
    Ruct_push_VecProjectile(&world->projectiles, proj);
    Ruct_push_VecF32(&world->projectile_lifetime_timers, 0);
}

void Projectile_update(Ruct_Projectile* proj) {
    Vector2 forward = forward_rotated(proj->rot * DEG2RAD);
    proj->pos.x += proj->mov.x * forward.x * GetFrameTime();
    proj->pos.y += proj->mov.y * forward.y * GetFrameTime();
}

void Projectile_draw(Ruct_Projectile* proj, u8 alpha) {
    DrawCircle(proj->pos.x, proj->pos.y, PROJECTILE_RADIUS, (Color){ 255, 0, 128, alpha });
}

void Projectile_drawDir(Ruct_Projectile* proj) {
    Vector2 forward = forward_rotated(proj->rot * DEG2RAD);
    DrawLine(proj->pos.x, proj->pos.y, proj->pos.x + DRAW_DIR_LEN * forward.x, proj->pos.y + DRAW_DIR_LEN * forward.y, (Color){0, 200, 150, 255});
}

// -----

Ruct_Result_None Asteroid_spawn(World *world, Vector2 pos, f32 rot, enum AsteroidSize size) {
    Vector2 mov;
    Vector2 forward = forward_rotated(rot * DEG2RAD);
    switch(size) {
        case SMALL: {
            usize r1 = rand() % (ASTEROID_SMALL_SPEED_MAX - ASTEROID_SMALL_SPEED_MIN);
            usize r2 = r1 + ASTEROID_SMALL_SPEED_MIN;
            mov = (Vector2) { .x = r2 * forward.x, .y = r2 * forward.y };
            break;
        }
        case MEDIUM: {
            usize r1 = rand() % (ASTEROID_MEDIUM_SPEED_MAX - ASTEROID_MEDIUM_SPEED_MIN);
            usize r2 = r1 + ASTEROID_MEDIUM_SPEED_MIN;
            mov = (Vector2) { .x = r2 * forward.x, .y = r2 * forward.y };
            break;
        }
        case LARGE: {
            usize r1 = rand() % (ASTEROID_LARGE_SPEED_MAX - ASTEROID_LARGE_SPEED_MIN);
            usize r2 = r1 + ASTEROID_LARGE_SPEED_MIN;
            mov = (Vector2) { .x = r2 * forward.x, .y = r2 * forward.y };
            break;
        }
        default: {
            return Ruct_Err_None("invalid asteroid size");
        }
    }
    Ruct_Asteroid asteroid = {.pos = pos, .mov = mov, .rot = rot, .size = size };
    Ruct_push_VecAsteroid(&world->asteroids, asteroid);
    return RUCT_OK_NONE;
}

void Asteroid_update(Ruct_Asteroid* asteroid) {
    Vector2 forward = forward_rotated(asteroid->rot * DEG2RAD);
    asteroid->pos.x += asteroid->mov.x * forward.x * GetFrameTime();
    asteroid->pos.y += asteroid->mov.y * forward.y * GetFrameTime();
}

void Asteroid_draw(Ruct_Asteroid* asteroid) {
    DrawCircle(asteroid->pos.x, asteroid->pos.y, ASTEROID_RADIUS_LARGE, (Color){ 255, 0, 128, 255 });
}

void Asteroid_drawDir(Ruct_Asteroid* asteroid) {
    Vector2 forward = forward_rotated(asteroid->rot * DEG2RAD);
    DrawLine(asteroid->pos.x, asteroid->pos.y, asteroid->pos.x + DRAW_DIR_LEN * forward.x, asteroid->pos.y + DRAW_DIR_LEN * forward.y, (Color){0, 200, 150, 255});
}

// -----

Ruct_Result_None World_init(World* world) {
    srand(time(NULL));
    world->projectiles = RUCT_TRY_CONVERT(Ruct_new_VecProjectile(), Ruct_Err_None);
    world->projectile_lifetime_timers = RUCT_TRY_CONVERT(Ruct_new_VecF32(), Ruct_Err_None);
    world->asteroids = RUCT_TRY_CONVERT(Ruct_new_VecAsteroid(), Ruct_Err_None);
    return RUCT_OK_NONE;
}

void World_free(World* world) {
    Ruct_free_VecProjectile(&world->projectiles);
    Ruct_free_VecF32(&world->projectile_lifetime_timers);
    Ruct_free_VecAsteroid(&world->asteroids);
}

void World_removeProjectile(World* world, usize i) {
    Ruct_remove_VecF32(&world->projectile_lifetime_timers, i);
    Ruct_remove_VecProjectile(&world->projectiles, i);
}

void World_update(World* world) {
    usize i = 0;
    loop {
        Ruct_Option_pProjectile maybe_proj_ptr = Ruct_getPtr_VecProjectile(&world->projectiles, i);
        if (!maybe_proj_ptr.is_some) break;
        Projectile_update(maybe_proj_ptr.some);
        Ruct_Option_pF32 maybe_timer = Ruct_getPtr_VecF32(&world->projectile_lifetime_timers, i);
        if (!maybe_timer.is_some) RUCT_PANIC("World is missing timer for projectile\n");

        *maybe_timer.some += GetFrameTime();
        if (*maybe_timer.some >= PROJECTILE_LIFETIME_SECONDS) {
            World_removeProjectile(world, i);
            if (i == 0) continue;
        } else i++;

    }

    for (usize i = 0;;i++) {
        Ruct_Option_pAsteroid maybe_asteroid_ptr = Ruct_getPtr_VecAsteroid(&world->asteroids, i);
        if (!maybe_asteroid_ptr.is_some) break;
        Asteroid_update(maybe_asteroid_ptr.some);
    }
}

void World_draw(World* world) {
    for (usize i = 0;;i++) {
        Ruct_Option_pProjectile maybe_proj_ptr = Ruct_getPtr_VecProjectile(&world->projectiles, i);
        if (!maybe_proj_ptr.is_some) break;
        Ruct_Projectile* proj_ptr = maybe_proj_ptr.some;

        Ruct_Option_F32 maybe_proj_timer = Ruct_get_VecF32(&world->projectile_lifetime_timers, i);
        if (maybe_proj_timer.is_some) {
            const f32 proj_timer = maybe_proj_timer.some;
            const f32 fadeout_frac = (f32)PROJECTILE_LIFETIME_SECONDS * PROJECTILE_FADEOUT_START_REMAINING;
            const f32 fadout_start_time = PROJECTILE_LIFETIME_SECONDS - fadeout_frac;

            if (fadout_start_time < proj_timer) {
                const f32 alpha_f = 1 - ((proj_timer - fadout_start_time) / fadeout_frac);
                printf("ALPHA_F: %f\n", alpha_f);
                const u8 alpha = (u8) (alpha_f * 255);
                printf("ALPHA: %d\n", alpha);
                Projectile_draw(proj_ptr, alpha);
                continue;
            }
        }

        Projectile_draw(proj_ptr, 255);
        if (DRAW_DIR) Projectile_drawDir(proj_ptr);
    }

    for (usize i = 0;;i++) {
        Ruct_Option_pAsteroid maybe_asteroid_ptr = Ruct_getPtr_VecAsteroid(&world->asteroids, i);
        if (!maybe_asteroid_ptr.is_some) break;
        Asteroid_draw(maybe_asteroid_ptr.some);

        if (DRAW_DIR) Asteroid_drawDir(maybe_asteroid_ptr.some);
    }
}
