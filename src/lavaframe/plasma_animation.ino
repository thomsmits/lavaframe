#include "animation.h"
#include "plasma_animation.h"
#include "FastLED.h"

void PlasmaAnimation::setup() {
}

void PlasmaAnimation::reset() {
}

int PlasmaAnimation::animation(int *delay_in_msec) {

  *delay_in_msec = 5;

  if (lf_next_animation_requested() == true) {
    lf_reset_next_animation_request();
    return LF_ANIMATION_DONE;
  }
  
  calc_plasma();

  return LF_ANIMATION_CONTINUE;
}

void PlasmaAnimation::calc_plasma()
{
    int time =  int(millis() / 50.0);
    double leds = LF_COLS;
    double sinSources = 2;
    
    for(int y = 0; y < LF_ROWS; y++)
      for(int x = 0; x < LF_COLS; x++)
      {
        double value = 
                 sin((dist(x + time, y, LF_COLS, LF_ROWS) / leds) * 5.0)
               //+ sin((dist(x, y, LF_ROWS / 6, LF_ROWS / 2) / leds) * 3.0)
               // + sin((dist(x, y + time / 2.0 , LF_COLS, LF_ROWS) / leds ) * 2.0)
                + sin(dist(x, y, LF_COLS / 2, LF_COLS / 3))
               ;
               
        int color = int((sinSources + value) * 128.0 / sinSources);
       
        rgb_pixel_t *px = lf_get_pixel(x, y);
        px->r = color;
        px->g = color * 2;
        px->b = 255 - color;
      }
  lf_push_to_strip();
}
