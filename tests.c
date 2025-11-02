#include <raymath.h>

#include "ruct/ruct.h"
#include "util.h"

RUCT_TEST_DEFINE(forward_right,
    Vector2 v  = forward_rotated(0);
    RUCT_ASSERT_EQ("vx = 1x", v.x, 1);
    RUCT_ASSERT_EQ("vy = 0y", v.y, 0);
)

RUCT_TEST_MODULE( "abc_test",
    RUCT_TEST("forward right", forward_right);
)
