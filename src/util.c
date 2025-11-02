#include <raymath.h>

typedef float f32;

Vector2 forward_rotated(float rad) {
    const Vector2 forward = {cosf(rad), sinf(rad)};
    return forward;
}

Vector2 rotate_point(Vector2 p, Vector2 center, f32 rad) {
    f32 s = sinf(rad);
    f32 c = cosf(rad);

    f32 x = p.x - center.x;
    f32 y = p.y - center.y;

    f32 xnew = x * c - y * s;
    f32 ynew = x * s + y * c;

    Vector2 result = {.x = xnew + center.x, .y = ynew + center.y};
    return result;
}
