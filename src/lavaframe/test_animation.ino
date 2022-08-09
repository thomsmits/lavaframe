#include "animation.h"

// This animation
lf_animation_t test_animation = {
  setup_test_animation,
  setup_test_animation,
  loop_test_animation,
};

/**
 * Setup the animation.
 */
void setup_test_animation() {
}

/**
 * Function called in a loop.
 * 
 * @param (out) delay_in_msec the delay requested by the function
 * @return constant requesting the next action of the loop
 */
int loop_test_animation(int *delay_in_msec) {
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

    return LF_ANIMATION_CONTINUE;
}
