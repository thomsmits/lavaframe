#include "animation.h"

lf_animation_t fire_animation = {
  setup_fire_animation,
  loop_fire_animation
};

const int slowDown = 1;

const int emitters = 4;               // how many moving fire emitters on the baseline?
int emitter_x[emitters];              // x-position of all emitters
int emitter_y[emitters];              // y-position of all emitters
int emitter_counter[emitters];        // emitter movement counter
int emitter_direction_x[emitters];    // emitter moving in x direction
int emitter_direction_y[emitters];    // emitter moving in y direction
int emitter_maxtop_y = LF_ROWS-1;     // how high can an emitter go? 0 = max top, LF_ROWS-1 = baseline

int fire_animation_field[LF_COLS][LF_ROWS+1];
rgb_pixel_t fire_animation_palette[255];

void setup_fire_animation() {
  
  fire_animation_setup_palette();
  
  for (int x = 0; x < LF_COLS; x++) {
    for (int y = 0; y < LF_ROWS; y++) {
      fire_animation_field[x][y] = 0;
    } 
  }

  for (int i=0; i < emitters; i++) {
    emitter_direction_x[i] = random(1,2);
    emitter_direction_y[i] = random(1,2);
  }
  
}

int loop_fire_animation(int *delay_in_msec) {
  
    fire_animation_generate_emitter();
    fire_animation_calc_fire();
    fire_animation_fire_to_leds();

    *delay_in_msec = 30;

    return LF_ANIMATION_CONTINUE;
}


void fire_animation_calc_fire()
{
    for (int y = 0; y < LF_ROWS-1; y++)
    {
        for (int x = 0; x < LF_COLS; x++)
        {
            int leftVal;

            if (x == 0)
                leftVal = fire_animation_field[x][y];
            else
                leftVal = fire_animation_field[x - 1][y];

            int rightVal;
            if (x == LF_COLS - 1)
                rightVal = fire_animation_field[x][y];
            else
                rightVal = fire_animation_field[x + 1][y];

            int belowVal = fire_animation_field[x][y + 1];

            int sum = leftVal + rightVal + (belowVal * 2);
            int avg = (int)(0.9 * sum / 4);

            // auto reduce it so you get lest of the forced fade and more vibrant fire waves
            if (avg > 0)
                avg--;

            if (avg < 0 || avg > 255) {
                avg = 0; // Average color calc is out of range 0-255 ?!? 
            }

            fire_animation_field[x][y] = avg;
        }
    }
}

void fire_animation_generate_emitter()
{
  for (int x = 0; x < LF_COLS; x++) {
     fire_animation_field[x][LF_ROWS-1] = 0;
  }

  for (int i=0; i < emitters;i++) {
    emitter_counter[i]++;
    if (emitter_counter[i] > (max(i*2,1)) * slowDown) {
      
      emitter_counter[i] =0;
      
      emitter_x[i] += emitter_direction_x[i];
      emitter_y[i] += emitter_direction_y[i];

      if (emitter_x[i] >= LF_COLS) {
         emitter_direction_x[i] = -random(1,3);
         emitter_x[i] = LF_COLS-1;
      }       
      if (emitter_x[i] < 0) {
        emitter_direction_x[i] = random(1,3);
         emitter_x[i] = 0;
      }       
      
      if (emitter_y[i] >= LF_ROWS) {
         emitter_direction_y[i] = -random(1,3);
         emitter_y[i] = LF_ROWS-1;
      }       
      if (emitter_y[i] < emitter_maxtop_y) {
        emitter_direction_y[i] = random(1,3);
        emitter_y[i] = emitter_maxtop_y;
      }         
    }

    for (int x = -1; x <= 1; x++) {
      for (int y = 0; y <= 0; y++) {
        
        int e_x = emitter_x[i] + x;
        int e_y = emitter_y[i] + y;
        
        if (e_x >= 0 && e_x < LF_COLS && e_y >= 0 && e_y < LF_ROWS) { 
          fire_animation_field[e_x][e_y] = 255; // - abs(x) -abs(y);
        }
        
      }
    }
  }
}

void fire_animation_fire_to_leds()
{
  for (int x = 0; x < LF_COLS; x++) {
    for (int y = 0; y < LF_ROWS; y++) {
      byte value = fire_animation_field[x][y];
      rgb_pixel_t color = fire_animation_palette[value];
      rgb_pixel_t *px = lf_get_pixel(x, y);
      px->r = color.r;
      px->g = color.g;
      px->b = color.b;
    } 
  }
  lf_push_to_strip();
}

void fire_animation_setup_palette()
{
    for (int i = 0; i < 64; i++)
    {
        fire_animation_palette[i].r = (byte)(i * 4);
        fire_animation_palette[i].g = (byte)(0);
        fire_animation_palette[i].b = (byte)(0);

        fire_animation_palette[i+64].r = (byte)(255);
        fire_animation_palette[i+64].g = (byte)(i * 4);
        fire_animation_palette[i+64].b = (byte)(0);

        fire_animation_palette[i+128].r = (byte)(255);
        fire_animation_palette[i+128].g = (byte)(255);
        fire_animation_palette[i+128].b = (byte)(i * 4);

        fire_animation_palette[i+192].r = (byte)(255 + - i * 4);
        fire_animation_palette[i+192].g = (byte)(255);
        fire_animation_palette[i+192].b = (byte)(i * 4);
    }    
}


void fire_animation_setup_palette_blue()
{
    for (int i = 0; i < 64; i++)
    {
        fire_animation_palette[i].r = (byte)(i * 4);
        fire_animation_palette[i].g = (byte)(0);
        fire_animation_palette[i].b = (byte)(0);

        fire_animation_palette[i+64].r = (byte)(0);
        fire_animation_palette[i+64].g = (byte)(i * 4);
        fire_animation_palette[i+64].b = (byte)(255);

        fire_animation_palette[i+128].r = (byte)(i * 4);
        fire_animation_palette[i+128].g = (byte)(255);
        fire_animation_palette[i+128].b = (byte)(255);

        fire_animation_palette[i+192].r = (byte)(255 + - i * 4);
        fire_animation_palette[i+192].g = (byte)(i * 4);
        fire_animation_palette[i+192].b = (byte)(255);
    }    
}
