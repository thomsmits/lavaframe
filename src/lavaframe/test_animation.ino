#include "test_animation.h"

void TestAnimation::setup() {
}

void TestAnimation::reset() {
}

int TestAnimation::animation(int *delay_in_msec) {
    static int x = 0;
    static int y = 0;

    if (x >= LF_COLS) {
        x = 0;
        y++;
    }

    if (y >= LF_ROWS) {
        x = 0;
        y = 0;
        return LF_ANIMATION_DONE;
    }

    x++;

    rgb_pixel_t *px = lavaFrame.get_pixel(x, y);
    px->r = 100;
    px->g = 100;
    px->b = 100;
    lavaFrame.push_to_strip();

    lavaFrame.clear();

    *delay_in_msec = 100;

     if (lavaFrame.next_animation_requested() == true) {
        lavaFrame.reset_next_animation_request();
        return LF_ANIMATION_DONE;
    }

    return LF_ANIMATION_CONTINUE;
}
