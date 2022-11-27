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

    rgb_pixel_t colors1[] = {
      { 255, 0, 0 },
      { 0, 0, 255 },
      { 0, 255, 0 },
      { 255, 0, 128 },
      { 255, 255, 0 },
      { 0, 255, 255 },
      { 255, 128, 255 },
      { 255, 0, 0 } 
    };

    rgb_pixel_t colors2[] = {
      { 128, 255, 0 },
      { 255, 0, 0 },
      { 255, 255, 0 },
      { 0, 0, 255 },
      { 0, 255, 255 },
      { 255, 255, 0 },
      { 255, 0, 0 },
      { 255, 255, 0 },
      { 255, 0, 255 },
      { 0, 255, 255 },
      { 128, 255, 128 }
    };

    switch (scene) {
      
      case 2:
        sinFactor1 = 1.2;
        sinFactor2 = 1.0;
        sinFactor3 = 1.0;
        sinFactor4 = 1.0;
        slowDownFactor = 300;
        fill_palette(palette, 256,  colors1, 8); 
        break;

      default:
        sinFactor1 = 1.2;
        sinFactor2 = 1.2;
        sinFactor3 = 1.0;
        sinFactor4 = 1.0;
        slowDownFactor = 300;
        fill_palette(palette, 256,  colors2, 11); 
        scene = 1;
        break;
    }
}

void PlasmaAnimation::calc_plasma()
{
    double time =  millis() / slowDownFactor;
    double leds = LF_COLS;

    int sins = int(sinFactor1 + sinFactor2 + sinFactor3 + sinFactor4);

    for(int y = 0; y < LF_ROWS; y++) {
        for(int x = 0; x < LF_COLS; x++) {
            double value =
                      sin((dist(x + time, y, LF_COLS / 2, LF_ROWS / 2) / leds) * sinFactor1)
                    + sin((dist(x, y, LF_ROWS / 6, LF_ROWS / 5) / leds) * sinFactor2)
                    + sin((dist(x, y + time, LF_COLS, LF_ROWS) / leds ) * sinFactor3)
                    + sin((dist(x, y, LF_COLS / 10, LF_COLS / 7) / leds) * sinFactor4)
                  ;
              int colNo = int((sins + value) * (255.0 / (sins*2)));
              rgb_pixel_t *px = lavaFrame.get_pixel(x, y);
              px->r = palette[colNo].r;
              px->g = palette[colNo].g;
              px->b = palette[colNo].b;
        }
    }
    lavaFrame.push_to_strip();
}
