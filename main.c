#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "ruct/ruct.h"
#include "types.h"
#include "conf.h"
#include "player.h"
#include "types.h"
#include "world.h"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib window");
    SetTargetFPS(155);

    World world;
    Player player;

    Ruct_unwrap_None(World_init(&world));
    Player_init(&player, &world);

    Ruct_VecUSize vec_usize = Ruct_unwrap_VecUSize(Ruct_new_VecUSize());
    Ruct_push_VecUSize(&vec_usize, 10);
    Ruct_push_VecUSize(&vec_usize, 20);
    Ruct_push_VecUSize(&vec_usize, 30);
    Ruct_push_VecUSize(&vec_usize, 40);
    Ruct_push_VecUSize(&vec_usize, 50);
    Ruct_push_VecUSize(&vec_usize, 60);

    for (usize i = 0;; i++) {
        Ruct_Option_USize opt = Ruct_get_VecUSize(&vec_usize, i);
        printf("vec len: %zu\n", vec_usize.len);

        if (opt.is_some) {
            printf("opt: %zu\n", opt.some);
        } else {
            printf("opt none.\n");
            break;
        }
    }

    printf("---\n");

    for (usize i = 0;; i++) {
        Ruct_Option_USize opt = Ruct_remove_VecUSize(&vec_usize, i);
        printf("vec len: %zu\n", vec_usize.len);

        if (opt.is_some) {
            printf("opt: %zu\n", opt.some);
        } else {
            printf("opt none.\n");
            break;
        }
    }

    Ruct_Option_USize opt_usize = Ruct_get_VecUSize(&vec_usize, 1);
    if (opt_usize.is_some) printf("opt_usize is Some: %zu\n", opt_usize.some);
    Ruct_free_VecUSize(&vec_usize);

    Asteroid_spawn(&world, (Vector2) {.x = 100, .y = 200 }, 35, LARGE);

    f32 timer = 0;
    f32 last_timer_hit = 0;
    const usize timer_interval = 2;
    while (!WindowShouldClose()) {
        timer += GetFrameTime();

        if (last_timer_hit + timer_interval < timer) {
            last_timer_hit = timer;
            printf("World: ");
            usize i = 0;
            loop {
                Ruct_Option_Projectile maybe_proj = Ruct_get_VecProjectile(&world.projectiles, i);
                Ruct_Option_F32 maybe_proj_timer = Ruct_get_VecF32(&world.projectile_lifetime_timers, i);
                Ruct_Option_Asteroid maybe_asteroid = Ruct_get_VecAsteroid(&world.asteroids, i);
                if(!maybe_proj.is_some && !maybe_proj_timer.is_some && !maybe_asteroid.is_some)
                    break;

                printf("---\n");
                printf("i: %ld\n", i);
                if (maybe_proj.is_some) {
                    const Ruct_Projectile proj = maybe_proj.some;
                    printf("Projectile pos x: %f\n", proj.pos.x);
                    printf("Projectile pos y: %f\n", proj.pos.y);
                    printf("Projectile mov x: %f\n", proj.mov.x);
                    printf("Projectile mov y: %f\n", proj.mov.y);
                    printf("Projectile rot deg: %f\n", proj.rot);
                    printf("Projectile team: %s\n", proj.team == PLAYER ? "PLAYER" : "ENEMY");
                }
                printf("---\n");

                if (maybe_proj_timer.is_some) {
                    const f32 t = maybe_proj_timer.some;
                    printf("Projectile Timer: %f", t);
                }
                printf("---\n");

                if (maybe_asteroid.is_some) {
                    const Ruct_Asteroid ast = maybe_asteroid.some;
                    printf("Asteroid pos x: %f\n", ast.pos.x);
                    printf("Asteroid pos y: %f\n", ast.pos.y);
                    printf("Asteroid mov x: %f\n", ast.mov.x);
                    printf("Asteroid mov y: %f\n", ast.mov.y);
                    printf("Asteroid rot deg: %f\n", ast.rot);
                    printf("Asteroid size: %s\n", ast.size == SMALL ? "SMALL" : ast.size == MEDIUM ? "MEDIUM" : "LARGE");
                }
                printf("---\n");

                i++;
            }
        }

        // update input
        player.input.holding_left = IsKeyDown(KEY_A);
        player.input.holding_right = IsKeyDown(KEY_D);
        player.input.holding_up = IsKeyDown(KEY_W);
        player.input.holding_down = IsKeyDown(KEY_S);
        player.input.pressed_fire = IsKeyPressed(KEY_SPACE);

        // update entities
        Player_update(&player);
        World_update(&world);

        BeginDrawing();
        ClearBackground(BLACK);

        // draw entities
        Player_draw(&player);
        World_draw(&world);

        EndDrawing();
    }

    World_free(&world);
    CloseWindow();
    return 0;
}
