#include "animation.h"

lf_animation_t bubbles_animation = {
  setup_bubbles_animation,
  setup_bubbles_animation,
  loop_bubbles_animation
};


const int bubbles_animation_bubbles = 6;    // how many moving bubbles?

int bubbles_animation_scene = 0;

double bubbles_animation_bubble_x[bubbles_animation_bubbles];             // x-position of bubbles
double bubbles_animation_bubble_y[bubbles_animation_bubbles];             // y-position of bubbles
int bubbles_animation_bubble_radius[bubbles_animation_bubbles];           // sizes of bubbles
double bubbles_animation_bubble_speed[bubbles_animation_bubbles];         // bubbles moving in y direction

double bubbles_animation_bubble_size;

//int bubbles_animation_field[LF_COLS][LF_ROWS];
rgb_pixel_t bubbles_animation_palette[bubbles_animation_bubbles];

void setup_bubbles_animation() {
  bubbles_animation_scene = 0;
  bubbles_animation_next_scene();
}

void bubbles_animation_next_scene() {
  
  bubbles_animation_scene++;
  
  switch (bubbles_animation_scene) {
  /*  case 2:
      bubbles_animation_palette = {{0, 255, 0}, {0, 64, 0}, {128, 0, 128}, {255, 0,0}, {64, 0, 0}, {0, 0, 255}}; 
      bubbles_animation_bubble_size = 0.35;
      break;
       
    case 3:
	  bubbles_animation_palette = {{0, 255, 0}, {0, 64, 0}, {128, 0, 128}, {255, 0,0}, {64, 0, 0}, {0, 0, 255}}; 
      bubbles_animation_bubble_size = 0.35;
      break;

    case 4:
      bubbles_animation_palette = {{0, 255, 0}, {0, 64, 0}, {128, 0, 128}, {255, 0,0}, {64, 0, 0}, {0, 0, 255}}; 
      bubbles_animation_bubble_size = 0.35;
      break;*/
     
    default:
	    bubbles_animation_palette[0] = (rgb_pixel_t){0, 255, 0};
      bubbles_animation_palette[1] = (rgb_pixel_t){0, 64, 0};
      bubbles_animation_palette[2] = (rgb_pixel_t){128, 0, 128};
      bubbles_animation_palette[3] = (rgb_pixel_t){255, 0,0};
      bubbles_animation_palette[4] = (rgb_pixel_t){64, 0, 0};
      bubbles_animation_palette[5] = (rgb_pixel_t){0, 0, 255};
      bubbles_animation_palette[6] = (rgb_pixel_t){0, 255, 0};
      bubbles_animation_palette[7] = (rgb_pixel_t){0, 64, 0};
      bubbles_animation_palette[8] = (rgb_pixel_t){128, 0, 128};
      bubbles_animation_palette[9] = (rgb_pixel_t){255, 0,0};
      bubbles_animation_palette[10] = (rgb_pixel_t){64, 0, 0};
      bubbles_animation_palette[11] = (rgb_pixel_t){0, 0, 255};
      bubbles_animation_bubble_size = 0.35;
      bubbles_animation_scene = 1;
      break; 
  }

  int border = 2;

  for (int i=0; i < bubbles_animation_bubbles; i++) {
    bubbles_animation_bubble_x[i] = (i*7) % LF_COLS; //border + (((LF_COLS -2 -border*2) * i )/ bubbles_animation_bubbles ) ;
    bubbles_animation_bubble_y[i] = (i*5) % LF_ROWS; //(i * (LF_ROWS / 4)) % (LF_ROWS);
    bubbles_animation_bubble_speed[i] = 0.1 + i / 30.0;
    bubbles_animation_bubble_radius[i] = (int)(2 + (i * bubbles_animation_bubble_size / 3.0));
  }
}

int loop_bubbles_animation(int *delay_in_msec) {

    *delay_in_msec = 30;

    // clear field
    lf_clear();
    
    // draw bubbles
    for (int i=bubbles_animation_bubbles-1; i >=0; i--) {
      bubbles_calc_bubble(i);
      bubbles_draw_bubble(i);
    }

    lf_push_to_strip();

    if (lf_next_animation_requested() == true) {
      bubbles_animation_next_scene();
      if (bubbles_animation_scene == 1) {
        return LF_ANIMATION_DONE;
      }
  }

   return LF_ANIMATION_CONTINUE;
}

void bubbles_calc_bubble(int i) {

    bubbles_animation_bubble_y[i] -= bubbles_animation_bubble_speed[i];
    
    if (bubbles_animation_bubble_y[i] <= -LF_ROWS - bubbles_animation_bubble_radius[i] * 2 ) {
      bubbles_animation_bubble_y[i] = LF_ROWS + bubbles_animation_bubble_radius[i] * 2;
    }       
}

void bubbles_draw_bubble(int i) {

  double b_x = bubbles_animation_bubble_x[i];
  double b_y = bubbles_animation_bubble_y[i];
  double radius = bubbles_animation_bubble_radius[i];

  for (int x = 0; x < LF_COLS; x++) {
    for (int y = 0; y < LF_ROWS; y++) {
      double dist = sqrt((b_x - x) * (b_x - x) + (b_y - y) * (b_y - y));
      rgb_pixel_t color;
      color = bubbles_animation_palette[i];
      if (radius < dist)  {
        double factor = min(1.0, max (0.0, 1 - (dist-radius) / (radius / 2.0)));
        color.r = byte(color.r * factor);
        color.g = byte(color.g * factor);
        color.b = byte(color.b * factor);
      }
      rgb_pixel_t *px = lf_get_pixel(x, y);
      if (px->r + px->g + px->b < color.r + color.g + color.b) {
        px->r = color.r;
        px->g = color.g;
        px->b = color.b;
      }
    }  
  }
}

byte bubbles_animation_col_interpolate(byte startValue, byte endValue, int steps, int pos) {
  return (byte)(startValue + ((endValue - startValue) * pos) / steps);
}

/*void bubbles_animation_field_to_leds()
{
  for (int x = 0; x < LF_COLS; x++) {
    for (int y = 0; y < LF_ROWS; y++) {
      byte value = bubbles_animation_field[x][y];
      rgb_pixel_t color = bubbles_animation_palette[value];
      rgb_pixel_t *px = lf_get_pixel(x, y);
      px->r = color.r;
      px->g = color.g;
      px->b = color.b;
    } 
  }
  lf_push_to_strip();
}*/
