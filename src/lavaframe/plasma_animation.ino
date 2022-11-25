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
  to_leds();

  return LF_ANIMATION_CONTINUE;
}

void PlasmaAnimation::calc_plasma()
{
    paletteShift = int(millis() / 30.0);

    //draw every pixel again, with the shifted palette color
    for (int y = 0; y < LF_ROWS; y++)
    {
        for (int x = 0; x < LF_COLS; x++)
        {
            field[x][y] =  (plasma[y][x] + paletteShift) % 256;
        }
    }
}

void PlasmaAnimation::setup_palette()
{
    for (int i = 0; i < 64; i++)
    {
        palette[i].r = (byte)(i * 4);
        palette[i].g = (byte)(0);
        palette[i].b = (byte)(i * 4);

        palette[i+64].r = (byte)(255 -  i*4);
        palette[i+64].g = (byte)(0);
        palette[i+64].b = (byte)(255);

        palette[i+128].r = (byte)(i * 4);
        palette[i+128].g = (byte)(i * 4);
        palette[i+128].b = (byte)(255);

        palette[i+192].r = (byte)(255);
        palette[i+192].g = (byte)(255);
        palette[i+192].b = (byte)(255);
    }    
}

void PlasmaAnimation::setup_plasma()
{
  for(int y = 0; y < LF_ROWS; y++)
    for(int x = 0; x < LF_COLS; x++)
    {
      int color = int
      (
          128.0 + (128.0 * sin(x / 1.0))
        + 128.0 + (128.0 * sin(y / 2.3))
      ) / 2;
      plasma[y][x] = color;
    }
}


void PlasmaAnimation::to_leds()
{
  for (int x = 0; x < LF_COLS; x++) {
    for (int y = 0; y < LF_ROWS; y++) {
      byte value = field[x][LF_ROWS-1-y];
      rgb_pixel_t color = palette[value];
      rgb_pixel_t *px = lf_get_pixel(x, y);
      px->r = color.r;
      px->g = color.g;
      px->b = color.b;
    } 
  }
  lf_push_to_strip();
}
