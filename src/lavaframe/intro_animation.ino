#include "animation.h"

// This animation
lf_animation_t intro_animation = {
  setup_intro_animation,
  setup_intro_animation,
  loop_intro_animation,
};

/**
 * Setup the animation.
 */
void setup_intro_animation() {
}

/**
 * Function called in a loop.
 * 
 * @param (out) delay_in_msec the delay requested by the function
 * @return constant requesting the next action of the loop
 */
int loop_intro_animation(int *delay_in_msec) {

    static int count = 0;

    count++;
    
    if (count >= LF_ROWS * 4) {
         return LF_ANIMATION_DONE;
    }

    lf_clear();

    // show screen orientation
    for (int x = 0; x < LF_COLS; x++) {
      for (int y = 0; y < LF_ROWS; y++) {
        if ((y+count) % 6 == 0) {
          rgb_pixel_t *px = lf_get_pixel(x, y);
          px->r = 100;
          px->b = 100;  
          px->g = 100;
        }
      }
    }
    
    lf_push_to_strip();

    *delay_in_msec = 100;

    if (lf_next_animation_requested() == true) {
        return LF_ANIMATION_DONE;
    }

    return LF_ANIMATION_CONTINUE;
}
