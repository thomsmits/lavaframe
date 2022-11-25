#include "animation.h"
#include "plasma_animation.h"
#include "FastLED.h"

void PlasmaAnimation::setup() {
  setup_palette();
  setup_plasma();
}

void PlasmaAnimation::reset() {
}

int PlasmaAnimation::animation(int *delay_in_msec) {

  *delay_in_msec = 30;

  if (lf_next_animation_requested() == true) {
    lf_reset_next_animation_request();
    return LF_ANIMATION_DONE;
  }
  
  calc_plasma();
  //to_leds();

  return LF_ANIMATION_CONTINUE;
}

void PlasmaAnimation::calc_plasma()
{
    int time = int(millis() / 50.0);
    for(int y = 0; y < LF_ROWS; y++)
      for(int x = 0; x < LF_COLS; x++)
      {
        double value = sin(dist(x + time, y, 128.0, 128.0) / 8.0)
               + sin(dist(x, y, 64.0, 64.0) / 8.0)
               + sin(dist(x, y + time / 7, 192.0, 64) / 7.0)
               + sin(dist(x, y, 192.0, 100.0) / 8.0);
        int color = int((4 + value)) * 32;
  
        rgb_pixel_t *px = lf_get_pixel(x, y);
        px->r = color;
        px->g = color * 2;
        px->b = 255 - color;
        //pset(x, y, ColorRGB(color, color * 2, 255 - color));
      }
  lf_push_to_strip();
}
