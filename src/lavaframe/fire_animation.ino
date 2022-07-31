#include "animation.h"

lf_animation_t fire_animation = {
  setup_fire_animation,
  setup_fire_animation,
  loop_fire_animation
};

int fire_animation_intensity        = 9;    // 1-10
const int fire_animation_emitters   = 4;    // how many moving fire emitters on the baseline?
int fire_animation_slowDown         = 1;    // emitter movement; 1 = full speed, 2 = half speed and so on...

int fire_animation_emitter_x[fire_animation_emitters];              // x-position of all emitters
int fire_animation_emitter_y[fire_animation_emitters];              // y-position of all emitters
int fire_animation_emitter_counter[fire_animation_emitters];        // emitter movement counter
int fire_animation_emitter_direction_x[fire_animation_emitters];    // emitter moving in x direction
int fire_animation_emitter_direction_y[fire_animation_emitters];    // emitter moving in y direction
int fire_animation_emitter_maxtop_y = LF_ROWS-1;                    // how high can an emitter go? 0 = max top, LF_ROWS-1 = baseline

int fire_animation_field[LF_COLS][LF_ROWS+1];
rgb_pixel_t fire_animation_palette[255];

int fire_animation_scene = 0;

void setup_fire_animation() {
  fire_animation_scene = 0;
  fire_animation_next_scene();
}

int loop_fire_animation(int *delay_in_msec) {

    if (lf_function_switch_1_pressed()) {
      while (lf_function_switch_1_pressed())  { delay(10); }
      fire_animation_next_scene();
    }
     
    fire_animation_calc_emitter();
    fire_animation_calc_fire();
    fire_animation_fire_to_leds();

    *delay_in_msec = 30;

    return LF_ANIMATION_CONTINUE;
}

void fire_animation_next_scene() {
  
  fire_animation_scene++;
  
  switch (fire_animation_scene) {
    
    case 2:
       fire_animation_slowDown = 1;
       fire_animation_intensity = 9;
       fire_animation_setup_palette_blue();
       fire_animation_emitter_maxtop_y = LF_ROWS-1; 
       break;
       
    case 3:
      fire_animation_slowDown = 1;
      fire_animation_setup_palette_fire();
      fire_animation_intensity = 10;
      fire_animation_emitter_maxtop_y = 0; 
      break;

    case 4:
      fire_animation_slowDown = 1;
      fire_animation_setup_palette_blue();
      fire_animation_intensity = 10;
      fire_animation_emitter_maxtop_y = LF_ROWS-1; 
      break;
     
    default:
      // standard fire
      fire_animation_slowDown = 1;
      fire_animation_setup_palette_fire();
      fire_animation_intensity = 9;
      fire_animation_emitter_maxtop_y = LF_ROWS-1; 
      fire_animation_scene = 1;
      break; 
  }


  for (int i=0; i < fire_animation_emitters; i++) {
    fire_animation_emitter_direction_x[i] = random(1,2);
    fire_animation_emitter_direction_y[i] = random(1,2);
  }
  
  for (int x = 0; x < LF_COLS; x++) {
    for (int y = 0; y < LF_ROWS; y++) {
      fire_animation_field[x][y] = 0;
    } 
  }
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
            int avg = (int)((fire_animation_intensity * sum) / 40);

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

void fire_animation_calc_emitter()
{
  for (int x = 0; x < LF_COLS; x++) {
     fire_animation_field[x][LF_ROWS-1] = 0;
  }

  for (int i=0; i < fire_animation_emitters;i++) {
    fire_animation_emitter_counter[i]++;
    if (fire_animation_emitter_counter[i] > (max(i*2,1)) * fire_animation_slowDown) {
      
      fire_animation_emitter_counter[i] =0;
      
      fire_animation_emitter_x[i] += fire_animation_emitter_direction_x[i];
      fire_animation_emitter_y[i] += fire_animation_emitter_direction_y[i];

      if (fire_animation_emitter_x[i] >= LF_COLS) {
         fire_animation_emitter_direction_x[i] = -random(1,3);
         fire_animation_emitter_x[i] = LF_COLS-1;
      }       
      if (fire_animation_emitter_x[i] < 0) {
         fire_animation_emitter_direction_x[i] = random(1,3);
         fire_animation_emitter_x[i] = 0;
      }       
      
      if (fire_animation_emitter_y[i] >= LF_ROWS) {
         fire_animation_emitter_direction_y[i] = -random(1,3);
         fire_animation_emitter_y[i] = LF_ROWS-1;
      }       
      if (fire_animation_emitter_y[i] < fire_animation_emitter_maxtop_y) {
        fire_animation_emitter_direction_y[i] = random(1,3);
       fire_animation_emitter_y[i] = fire_animation_emitter_maxtop_y;
      }         
    }

    for (int x = -1; x <= 1; x++) {
      for (int y = 0; y <= 0; y++) {
        
        int e_x = fire_animation_emitter_x[i] + x;
        int e_y = fire_animation_emitter_y[i] + y;
        
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

void fire_animation_setup_palette_fire()
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
        fire_animation_palette[i].b = (byte)(i * 4);

        fire_animation_palette[i+64].r = (byte)(255 -  i*4);
        fire_animation_palette[i+64].g = (byte)(0);
        fire_animation_palette[i+64].b = (byte)(255);

        fire_animation_palette[i+128].r = (byte)(i * 4);
        fire_animation_palette[i+128].g = (byte)(i * 4);
        fire_animation_palette[i+128].b = (byte)(255);

        fire_animation_palette[i+192].r = (byte)(255 - i * 4);
        fire_animation_palette[i+192].g = (byte)(255 - i * 4);
        fire_animation_palette[i+192].b = (byte)(255 - i * 4);
    }    
}
