#ifndef WORLD_H
#define WORLD_H
#include "ruct_include.h"
#include "types.h"

void Projectile_spawn(World* world, Vector2 pos, f32 rot, enum Team team);
void Projectile_update(Ruct_Projectile* proj);
void Projectile_draw(Ruct_Projectile* proj);

Ruct_Result_None Asteroid_spawn(World* world, Vector2 pos, f32 rot, enum AsteroidSize);
Ruct_Result_None Asteroid_spawnRandom(World *world, Vector2 player_pos);
Vector2 Asteroid_spawnRandom_genPos(World *world, i32 random_num);

// world->projectiles must be freed
// fixed capacity of 1024 (enough)
Ruct_Result_None World_init(World* world);
void World_free(World* world);
void World_update(World* world);
void World_draw(World* world);

RUCT_TESTDECLARE(test_Asteroid_spawnRandom_genPos);

#endif
