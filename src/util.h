#ifndef UTIL_H
#define UTIL_H

#include <raymath.h>
#define RUCT_STRIP_PREFIX
#include "ruct_include.h"

Vector2 forward_rotated(float rad);
Vector2 rotate_point(Vector2 p, Vector2 center, f32 rad);
#endif
