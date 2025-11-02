#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"
#define RUCT_STRIP_PREFIX
#include "ruct_include.h"
#include <raymath.h>

void Player_init(Player* player, World* world);
void Player_updateMovement(Player* p);
void Player_update(Player* p);
void Player_draw(Player* player);

#endif
