#include "ruct_include.h"
#include "types.h"

void Projectile_spawn(World* world, Vector2 pos, f32 rot, enum Team team);
void Projectile_update(Ruct_Projectile* proj);
void Projectile_draw(Ruct_Projectile* proj);

Ruct_Result_None Asteroid_spawn(World* world, Vector2 pos, f32 rot, enum AsteroidSize);

// world->projectiles must be freed
// fixed capacity of 1024 (enough)
Ruct_Result_None World_init(World* world);
void World_free(World* world);
void World_update(World* world);
void World_draw(World* world);
