#include <raylib.h>
#include <raymath.h>

#include "conf.h"
#include "types.h"
#include "util.h"
#include "world.h"

void Player_init(Player* player, World* world) {
    player->pos.x = (float)SCREEN_WIDTH / 2;
    player->pos.y = (float)SCREEN_HEIGHT / 2;
    player->rot = 0;

    player->input.holding_left = false;
    player->input.holding_right = false;
    player->input.holding_up = false;
    player->input.holding_down = false;

    player->input.pressed_fire = false;

    player->team = PLAYER;

    player->world = world;
}

void Player_updateMovement(Player* p) {
    // rotation
    if (p->input.holding_right) {
        p->rot += ((f32)PLAYER_ROTATION_SPEED * GetFrameTime());
    } else if (p->input.holding_left) {
        p->rot -= ((f32)PLAYER_ROTATION_SPEED * GetFrameTime());
    }
    // update mov
    const Vector2 forward_vec = forward_rotated(p->rot * DEG2RAD);

    if (p->input.holding_up) {
        p->mov.x += forward_vec.x * PLAYER_ACCEL_SPEED;
        p->mov.y += forward_vec.y * PLAYER_ACCEL_SPEED;
    } else if (p->input.holding_down) {
        p->mov.x -= forward_vec.x * PLAYER_ACCEL_SPEED;
        p->mov.y -= forward_vec.y * PLAYER_ACCEL_SPEED;
    }
    // drag
    if (p->mov.x > 0) {
        p->mov.x = fmaxf(0, p->mov.x - PLAYER_DECEL_SPEED);
    } else if (p->mov.x < 0) {
        p->mov.x = fminf(0, p->mov.x + PLAYER_DECEL_SPEED);
    }
    if (p->mov.y > 0) {
        p->mov.y = fmaxf(0, p->mov.y - PLAYER_DECEL_SPEED);
    } else if (p->mov.y  < 0) {
        p->mov.y = fminf(0, p->mov.y + PLAYER_DECEL_SPEED);
    }

    // cap movespeed
    if (p->mov.x > PLAYER_MOVEMENT_SPEED) {
        p->mov.x = PLAYER_MOVEMENT_SPEED;
    } else if (p->mov.x < -PLAYER_MOVEMENT_SPEED){
        p->mov.x = -PLAYER_MOVEMENT_SPEED;
    }

    if (p->mov.y > PLAYER_MOVEMENT_SPEED) {
        p->mov.y = PLAYER_MOVEMENT_SPEED;
    } else if (p->mov.y < -PLAYER_MOVEMENT_SPEED) {
        p->mov.y = -PLAYER_MOVEMENT_SPEED;
    }
    //printf("movy: %f, movx: %f\n", p->mov.y, p->mov.x);

    // apply mov
    p->pos.x += p->mov.x * GetFrameTime();
    p->pos.y += p->mov.y * GetFrameTime();
}

void Player_update(Player* p) {
    Player_updateMovement(p);

    if (p->input.pressed_fire) {
        Projectile_spawn(p->world, p->pos, p->rot, PLAYER);
    }
}

void Player_draw(Player* player) {
    Vector2 p1 = { .x = player->pos.x + 25, .y = player->pos.y };
    Vector2 p2 = { .x = player->pos.x - 25, .y = player->pos.y - 15 };
    Vector2 p3 = { .x = player->pos.x - 25, .y = player->pos.y + 15};
    Vector2 p1_rotated = rotate_point(p1, player->pos, player->rot * DEG2RAD);
    Vector2 p2_rotated = rotate_point(p2, player->pos, player->rot * DEG2RAD);
    Vector2 p3_rotated = rotate_point(p3, player->pos, player->rot * DEG2RAD);
    DrawTriangleLines(p1_rotated, p2_rotated, p3_rotated, WHITE);
    if (DRAW_DIR) {
        Vector2 forward = forward_rotated(player->rot * DEG2RAD);
        DrawLine(player->pos.x, player->pos.y, player->pos.x + DRAW_DIR_LEN * forward.x, player->pos.y + DRAW_DIR_LEN * forward.y, (Color){0, 200, 150, 255});
    }
}
