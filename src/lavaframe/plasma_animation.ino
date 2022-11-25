#include "animation.h"
#include "plasma_animation.h"
#include "FastLED.h"

void PlasmaAnimation::setup() {
  scene = 0;
  next_scene();
}

void PlasmaAnimation::reset() {
}

PostAnimationAction PlasmaAnimation::animation(int *delay_in_msec) {

  *delay_in_msec = 2;

  if (lavaFrame.next_animation_requested() == true) {
      lavaFrame.reset_next_animation_request();
      next_scene();
      if (scene == 1) {
        return PostAnimationAction::anim_done;
      }
    }

  calc_plasma();

  return PostAnimationAction::anim_continue;
}


void PlasmaAnimation::next_scene() {

    scene++;

    switch (scene) {

     /*   case 2:
            slowDown = 1;
            intensity = 9;
            setup_palette_blue();
            emitter_maxtop_y = LF_ROWS-1;
            break;

        case 3:
            slowDown = 1;
            setup_palette_fire();
            intensity = 10;
            emitter_maxtop_y = 0;
            break;

        case 4:
            slowDown = 1;
            setup_palette_blue();
            intensity = 10;
            emitter_maxtop_y = LF_ROWS-1;
            break;*/

        default:
            sinFactor1 = 1.0;
            sinFactor2 = 0.0;
            sinFactor3 = 0.0;
            sinFactor4 = 0.0;
            slowDownFactor = 100;
            scene = 1;
            break;
    }
}


void PlasmaAnimation::calc_plasma()
{
    double time =  millis() / slowDownFactor;
    double leds = LF_COLS;

    for(int y = 0; y < LF_ROWS; y++) {
        for(int x = 0; x < LF_COLS; x++) {
            double value =
                      sin((dist(x + time, y, LF_COLS / 2, LF_ROWS / 2) / leds) * sinFactor1)
                    + sin((dist(x, y, LF_ROWS / 6, LF_ROWS / 2) / leds) * sinFactor2)
                    + sin((dist(x, y + time / 2.0 , LF_COLS, LF_ROWS) / leds ) * sinFactor3)
                    + sin((dist(x, y, LF_COLS * 2, LF_COLS * 2) / leds) * sinFactor4)
                  ;

            int color = int((4 + value) * 128.0);

            rgb_pixel_t *px = lavaFrame.get_pixel(x, y);
            px->r = color;
            px->g = color * 2;
            px->b = 255 - color;
        }
    }
    lavaFrame.push_to_strip();
}
