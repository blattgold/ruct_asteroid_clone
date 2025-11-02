#include <raymath.h>

#include "../ruct/ruct.h"
#include "../src/util.h"

RUCT_TEST_DEFINE(forward_right,
    Vector2 v  = forward_rotated(0);
    RUCT_ASSERT_EQ("vx = 1x", v.x, 1);
    RUCT_ASSERT_EQ("vy = 0y", v.y, 0);
)

RUCT_TEST_DEFINE(forward_down,
    Vector2 v  = forward_rotated(90 * DEG2RAD);
    RUCT_ASSERT_FEQ("vx = 1x", v.x, 0);
    RUCT_ASSERT_FEQ("vy = 0y", v.y, 1);
)

RUCT_TESTMODULE(abc_test,
    RUCT_TEST(forward_right);
    RUCT_TEST(forward_down);
)

int main() {
    RUCT_RUN_TESTMODULE(abc_test);
}
