#include "animation.h"

lf_animation_t plasma_animation = {
  setup_plasma_animation,
  setup_plasma_animation,
  loop_plasma_animation
};


void setup_plasma_animation() {
 
}

int loop_plasma_animation(int *delay_in_msec) {

    *delay_in_msec = 30;

    return LF_ANIMATION_CONTINUE;
}
