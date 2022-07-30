#include "animation.h"

lf_animation_t fire_animation = {
  setup_fire_animation,
  loop_fire_animation
};

const int slowDown = 1;

const int baselines = 4;                // how many moving fire emitters on the baseline?
int baseline_x[baselines];              // position of all emitters
int baseline_counter[baselines];        // emitter movement counter
int baseline_direction[baselines];      // emitter moving direction

int fire_animation_field[LF_COLS][LF_ROWS];
rgb_pixel_t fire_animation_palette[255];


void setup_fire_animation() {
  
  fire_animation_setup_palette();
  
  for (int x = 0; x < LF_COLS; x++) {
    for (int y = 0; y < LF_ROWS; y++) {
      fire_animation_field[x][y] = 0;
    } 
  }

  for (int i=0; i <baselines; i++) {
    baseline_direction[i] = 1;
  }
  
}

int loop_fire_animation(int *delay_in_msec) {
  
    fire_animation_generateBaseline();
    fire_animation_calc_fire();
    fire_animation_fire_to_leds();

    *delay_in_msec = 30 * slowDown;

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
            int avg = (int)(0.85 * sum / 4);

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

void fire_animation_generateBaseline()
{
  for (int x = 0; x < LF_COLS; x++) {
     fire_animation_field[x][LF_ROWS-1] = 0;
  }

  for (int i=0; i < baselines;i++) {
    baseline_counter[i]++;
    if (baseline_counter[i] > (i+2) * slowDown) {
      baseline_counter[i] =0;
      baseline_x[i] += baseline_direction[i];
      if (baseline_x[i] >= LF_COLS || baseline_x[i]==0) baseline_direction[i] = -baseline_direction[i];
    }

    for (int x = 0; x < LF_COLS; x++) {
      if (x >= baseline_x[i] - 1 && x <= baseline_x[i] + 1) {
        fire_animation_field[x][LF_ROWS-1] = 255; // - (abs(baseline_x[i]-x)*5);
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
