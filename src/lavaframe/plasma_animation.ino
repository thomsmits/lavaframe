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

  if (lf_next_animation_requested() == true) {
    lava_animation_next_scene();
    if (lava_animation_scene == 1) {
      return LF_ANIMATION_DONE;
    }
  }

  return LF_ANIMATION_CONTINUE;
}
