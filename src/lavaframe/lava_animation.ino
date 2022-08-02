#include "animation.h"

lf_animation_t lava_animation = {
  setup_lava_animation,
  setup_lava_animation,
  loop_lava_animation
};


const int lava_animation_bubbles    = 6;    // how many moving bubbles?

double lava_animation_bubble_x[lava_animation_bubbles];             // x-position of bubbles
double lava_animation_bubble_y[lava_animation_bubbles];             // y-position of bubbles
int lava_animation_bubble_radius[lava_animation_bubbles];           // sizes of bubbles
double lava_animation_bubble_direction_x[lava_animation_bubbles];   // bubbles moving in x direction
double lava_animation_bubble_direction_y[lava_animation_bubbles];   // bubbles moving in y direction

int lava_animation_field[LF_COLS][LF_ROWS];
rgb_pixel_t lava_animation_palette[255];

void setup_lava_animation() {
  
  for (int i=0; i < lava_animation_bubbles; i++) {
    lava_animation_bubble_x[i] = LF_COLS * 0.25 + (i * 5) % (LF_COLS / 2);
    lava_animation_bubble_y[i] = (i * 3) % LF_ROWS;
    lava_animation_bubble_direction_x[i] = 0; //random(1,2);
    lava_animation_bubble_direction_y[i] = 0.01 + i / 200.0;
    lava_animation_bubble_radius[i] = 1 + (i / 2);
  }

  //lava_animation_setup_palette();
  lava_animation_setup_palette2();
  
}

int loop_lava_animation(int *delay_in_msec) {

    *delay_in_msec = 30;

    // clear field
    for (int x = 0; x < LF_COLS; x++) {
      for (int y = 0; y < LF_ROWS; y++) {
        lava_animation_field[x][y] = 0;
      } 
    }

   // draw bubbles
   for (int i=0; i < lava_animation_bubbles; i++) {
      calc_bubble(i);
      draw_bubble(i);
   }

   lava_animation_field_to_leds();

   return LF_ANIMATION_CONTINUE;
}

void calc_bubble(int i) {

    lava_animation_bubble_y[i] += lava_animation_bubble_direction_y[i];
    
    if (lava_animation_bubble_y[i] < 1) {
       lava_animation_bubble_direction_y[i] = -lava_animation_bubble_direction_y[i];
       lava_animation_bubble_y[i] = 1;
    }       
    
    if (lava_animation_bubble_y[i] >= LF_ROWS-1 ) {
       lava_animation_bubble_direction_y[i] = -lava_animation_bubble_direction_y[i];
       lava_animation_bubble_y[i] = LF_ROWS-2;
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
      } else{
        col = int(255 - (dist-radius) * 255);  
      }
     // col = col + lava_animation_field[x][y];
     // lava_animation_field[x][y]= max(0, min(255, col));

      
      lava_animation_field[x][y] = max(lava_animation_field[x][y], min(255, col));
    }  
  }
}


void lava_animation_setup_palette()
{
    for (int i = 0; i <= 255; i++)
    {
        lava_animation_palette[i].r = (byte)(i);
        lava_animation_palette[i].g = (byte)(0);
        lava_animation_palette[i].b = (byte)(i);
    }    
}

void lava_animation_setup_palette2()
{
 int dim = 2;
  
    for (int i = 0; i < 128; i++)
    {
        lava_animation_palette[255-i].r = (byte)(255 - i * 2) / dim; 
        lava_animation_palette[255-i].g = (byte)(0);
        lava_animation_palette[255-i].b = (byte)(255)/ dim;

        lava_animation_palette[128-i].r = (byte)(0);
        lava_animation_palette[128-i].g = (byte)(0);
        lava_animation_palette[128-i].b = (byte)(255 - i * 2)/ dim;
    }


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
