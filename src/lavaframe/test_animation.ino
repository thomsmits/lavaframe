#include "test_animation.h"

/**
 * Setup the animation.
 */
void TestAnimation::setup() {
}

/**
 * Reset the animation.
 */
void TestAnimation::reset() {
}


/**
 * Function called in a loop.
 * 
 * @param (out) delay_in_msec the delay requested by the function
 * @return constant requesting the next action of the loop
 */
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

    rgb_pixel_t *px = lf_get_pixel(x, y);
    px->r = 100;
    px->g = 100;
    px->b = 100;
    lf_push_to_strip();

    lf_clear();

    *delay_in_msec = 100;

     if (lf_next_animation_requested() == true) {
        lf_reset_next_animation_request();
        return LF_ANIMATION_DONE;
    }

    return LF_ANIMATION_CONTINUE;
}
