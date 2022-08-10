#include "animation.h"

lf_animation_t lava_animation = {
  setup_lava_animation,
  setup_lava_animation,
  loop_lava_animation
};


const int lava_animation_bubbles = 6;    // how many moving bubbles?

int lava_animation_scene = 0;

double lava_animation_bubble_x[lava_animation_bubbles];             // x-position of bubbles
double lava_animation_bubble_y[lava_animation_bubbles];             // y-position of bubbles
int lava_animation_bubble_radius[lava_animation_bubbles];           // sizes of bubbles
double lava_animation_bubble_direction_x[lava_animation_bubbles];   // bubbles moving in x direction
double lava_animation_bubble_direction_y[lava_animation_bubbles];   // bubbles moving in y direction

double lava_animation_bubble_size;

int lava_animation_field[LF_COLS][LF_ROWS];
rgb_pixel_t lava_animation_palette[255];

void setup_lava_animation() {
  lava_animation_scene = 0;
  lava_animation_next_scene();
}

void lava_animation_next_scene() {

  lava_animation_scene++;

  switch (lava_animation_scene) {
    case 2:
      lava_animation_setup_palette({0, 255, 0}, {0, 64, 0}, {128, 0, 128}); // green, purple background
      lava_animation_bubble_size = 0.35;
      break;

    case 3:
      lava_animation_setup_palette({255, 0, 0}, {64, 0, 0}, {0, 0, 0}); // red
      lava_animation_bubble_size = 0.35;
      break;

    case 4:
      lava_animation_setup_palette({255, 255, 255}, {0, 0, 0}, {0, 0, 64}); // white, blue background
      lava_animation_bubble_size = 0.4;
      break;

    default:
      lava_animation_setup_palette({255, 0, 255}, {0, 0, 255}, {0, 0, 0}); // purple, blue border
      lava_animation_bubble_size = 0.55;
      lava_animation_scene = 1;
      break;
  }

  int border = 3;

  for (int i = 0; i < lava_animation_bubbles; i++) {
    lava_animation_bubble_x[i] = border + (((LF_COLS - 2 - border * 2) * i ) / lava_animation_bubbles ) ;
    lava_animation_bubble_y[i] = (i * 3) % LF_ROWS;
    lava_animation_bubble_direction_x[i] = 0; //random(1,2);
    lava_animation_bubble_direction_y[i] = 0.01 + i / 200.0;
    lava_animation_bubble_radius[i] = (int)(1 + (i * lava_animation_bubble_size));
  }

  
}

int loop_lava_animation(int *delay_in_msec) {

  *delay_in_msec = 30;

  if (lf_buttons_animation_interaction_pressed == true) {
    // lava_animation_next_scene();
  }

  // clear field
  for (int x = 0; x < LF_COLS; x++) {
    for (int y = 0; y < LF_ROWS; y++) {
      lava_animation_field[x][y] = 0;
    }
  }

  // draw bubbles
  for (int i = 0; i < lava_animation_bubbles; i++) {
    calc_bubble(i);
    draw_bubble(i);
  }

  lava_animation_field_to_leds();

  if (lf_next_animation_requested() == true) {
    lf_reset_next_animation_request();
    lava_animation_next_scene();
    if (lava_animation_scene == 1) {
      return LF_ANIMATION_DONE;
    }
  }

  return LF_ANIMATION_CONTINUE;
}

void calc_bubble(int i) {

  lava_animation_bubble_y[i] += lava_animation_bubble_direction_y[i];

  if (lava_animation_bubble_y[i] < 1) {
    lava_animation_bubble_direction_y[i] = -lava_animation_bubble_direction_y[i];
    lava_animation_bubble_y[i] = 1;
  }

  if (lava_animation_bubble_y[i] >= LF_ROWS - 1 ) {
    lava_animation_bubble_direction_y[i] = -lava_animation_bubble_direction_y[i];
    lava_animation_bubble_y[i] = LF_ROWS - 1;
  }
}

void draw_bubble(int i) {

  double b_x = lava_animation_bubble_x[i];
  double b_y = lava_animation_bubble_y[i];
  double radius = lava_animation_bubble_radius[i];

  for (int x = 0; x < LF_COLS; x++) {
    for (int y = 0; y < LF_ROWS; y++) {
      double dist = sqrt((b_x - x) * (b_x - x) + (b_y - y) * (b_y - y));
      int col = 0;
      if (radius > dist)  {
        col = 255;
      } else {
        col = int(255 - (dist * 0.9 - radius) * 255);
      }
      lava_animation_field[x][y] = max(lava_animation_field[x][y], min(255, col));
    }
  }
}

void lava_animation_setup_palette(rgb_pixel_t color_lava, rgb_pixel_t color_border, rgb_pixel_t color_background)
{
  for (int i = 0; i <= 128; i++)
  {
    lava_animation_palette[255 - i].r = lava_animation_col_interpolate(color_lava.r, color_border.r, 128, i);
    lava_animation_palette[255 - i].g = lava_animation_col_interpolate(color_lava.g, color_border.g, 128, i);
    lava_animation_palette[255 - i].b = lava_animation_col_interpolate(color_lava.b, color_border.b, 128, i);

    lava_animation_palette[128 - i].r = lava_animation_col_interpolate(color_border.r, color_background.r, 128, i);
    lava_animation_palette[128 - i].g = lava_animation_col_interpolate(color_border.g, color_background.g, 128, i);
    lava_animation_palette[128 - i].b = lava_animation_col_interpolate(color_border.b, color_background.b, 128, i);
  }
}

byte lava_animation_col_interpolate(byte startValue, byte endValue, int steps, int pos) {
  return (byte)(startValue + ((endValue - startValue) * pos) / steps);
}

void lava_animation_field_to_leds()
{
  for (int x = 0; x < LF_COLS; x++) {
    for (int y = 0; y < LF_ROWS; y++) {
      byte value = lava_animation_field[x][y];
      rgb_pixel_t color = lava_animation_palette[value];
      rgb_pixel_t *px = lf_get_pixel(x, y);
      px->r = color.r;
      px->g = color.g;
      px->b = color.b;
    }
  }
  lf_push_to_strip();
}
