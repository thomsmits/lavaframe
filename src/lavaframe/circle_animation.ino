#include "animation.h"
#include "util.h"

// This animation
lf_animation_t circle_animation = {
  setup_circle_animation,
  reset_circle_animation,
  loop_circle_animation_1
};

/**
 * Setup the animation.
 */
void setup_circle_animation() {
}


/**
 * reset the animation.
 */
void reset_circle_animation() {
}


/**
 * Function called in a loop.
 * 
 * @param (out) delay_in_msec the delay requested by the function
 * @return constant requesting the next action of the loop
 */
int loop_circle_animation_1(int *delay_in_msec) {
    static bool first_run = true;
    static int r = 1;
    static int color_offset = 0;
    
    static rgb_pixel_t colors[] = {
        { 0xff, 0x00, 0x00 },
        { 0xff, 0xa5, 0x00 },
        { 0xff, 0xff, 0x00 },
        { 0x00, 0x80, 0x00 },
        { 0x00, 0x00, 0xff },
        { 0x4b, 0x00, 0x82 },
        { 0xee, 0x82, 0xee },
        { 0xff, 0x00, 0x00 },
        { 0xff, 0xa5, 0x00 },
        { 0xff, 0xff, 0x00 },
        { 0x00, 0x80, 0x00 },
        { 0x00, 0x00, 0xff },
        { 0x4b, 0x00, 0x82 },
        { 0xee, 0x82, 0xee },
        { 0xff, 0x00, 0x00 },
        { 0xff, 0xa5, 0x00 },
        { 0xff, 0xff, 0x00 },
        { 0x00, 0x80, 0x00 },
        { 0x00, 0x00, 0xff },
        { 0x4b, 0x00, 0x82 },
        { 0xee, 0x82, 0xee }     
    };
    
    //int start_x = random(0, LF_COLS);
    //int start_y = random(0, LF_ROWS);

    int start_x = LF_COLS / 2;
    int start_y = LF_ROWS / 2;

    const int max_radius = LF_COLS / 2 + 1;
    
    if (first_run) {
        lf_clear();
        first_run = false;
    }
    
    lf_draw_striped_circle(start_x, start_y, r, true, (colors + color_offset), sizeof(colors) - color_offset);
    lf_push_to_strip();
    
   /* static int radius = LF_COLS / 2;
    static int start_x = LF_COLS / 2;
    static int start_y = LF_ROWS / 2;
    static int runs = 0;
    
    byte r, g, b;

    byte h, s, v;

    h = random(0, 255);
    s = 100;
    v = 100;

    rgb_pixel_t px;

    lf_hsv_to_rgb(h, s, v, &px);

    r = px.r;
    g = px.g;
    b = px.b;
    
    /*
    if (radius % 3 == 0) {
        r = random(50, 255);
        g = random(0, 100);
        b = random(0, 100);
    }
    else if (radius % 3 == 1) {
        r = random(0, 100);
        g = random(50, 255);
        b = random(0, 100);
    }
    else {
        r = random(0, 100);
        g = random(0, 100);
        b = random(50, 255);
    }
*/
/*
    if (runs % 4 == 0) {
        start_x = 0;
        start_y = LF_ROWS / 2;
    }
    else if (runs % 4 == 1) {
        start_x = LF_COLS;
        start_y = LF_ROWS / 2;
    }
    else if (runs % 4 == 2) {
        start_x = LF_COLS / 2;
        start_y = 0;
    }
    else if (runs % 4 == 3) {
        start_x = LF_COLS / 2;
        start_y = LF_ROWS;
    }
*/
/*
    start_x = random(0, LF_COLS);
    start_y = random(0, LF_ROWS);
    
    lf_draw_circle(start_x, start_y, radius, r, g, b, 0);
    lf_push_to_strip();
    radius--;   

    lf_clear();
    if (radius <= 0) {
        radius = LF_ROWS / 2;
    }

    runs++;

    if (runs > 4) {
      runs = 0;
    }
*/
    r++;

    if (r > max_radius) {
        r = 1;
        color_offset = random(0, sizeof(colors) - max_radius);
    }
    *delay_in_msec = 500;

    if (lf_next_animation_requested() == true) {
        return LF_ANIMATION_DONE;
    }

    return LF_ANIMATION_CONTINUE;
}

/**
 * Function called in a loop.
 * 
 * @param (out) delay_in_msec the delay requested by the function
 * @return constant requesting the next action of the loop
 */
int loop_circle_animation_2(int *delay_in_msec) {
    const int radius = 1;
    static float x_pos = LF_COLS / 2;
    static float y_pos = LF_ROWS / 2;

    static float v_x = 0.5;
    static float v_y = 1.0;
    
    lf_clear();

    rgb_pixel_t color = { 255, 255, 255 };
    
    lf_draw_solid_circle(x_pos, y_pos, radius, false, color);
    lf_push_to_strip();
        
    x_pos += v_x;
    y_pos += v_y;

    if ((x_pos + radius > LF_COLS) || 
        (x_pos < radius) ||
        (y_pos + radius > LF_ROWS) || 
        (y_pos < radius)) {
          
      v_x *= ((float) random(8, 12)) / -10.0;
      v_y *= ((float) random(8, 12)) / -10.0;

      x_pos += v_x;
      y_pos += v_y;
    }

    
    *delay_in_msec = 10;

    if (lf_next_animation_requested() == true) {
        return LF_ANIMATION_DONE;
    }

    return LF_ANIMATION_CONTINUE;
}

/**
 * Function called in a loop.
 * 
 * @param (out) delay_in_msec the delay requested by the function
 * @return constant requesting the next action of the loop
 */
int loop_circle_animation_3(int *delay_in_msec) {
    static float x_pos = LF_COLS / 2;
    static float y_pos = LF_ROWS / 2;
    
    static float v_x = 0.5;
    static float v_y = 1.0;
    
    lf_clear();

    rgb_pixel_t *px = lf_get_pixel((int) x_pos, (int) y_pos);
    px->r = 255;
    px->g = 255;
    px->b = 255;
        
    lf_push_to_strip();
        
    x_pos += v_x;
    y_pos += v_y;

    if ((x_pos + 1 > LF_COLS) || 
        (x_pos < 1) ||
        (y_pos + 1 > LF_ROWS) || 
        (y_pos < 1)) {
          
      v_x *= ((float) random(9, 11)) / -10.0;
      v_y *= ((float) random(9, 11)) / -10.0;

      x_pos += v_x;
      y_pos += v_y;
    }
    
    *delay_in_msec = 5;

    if (lf_next_animation_requested() == true) {
        return LF_ANIMATION_DONE;
    }

    return LF_ANIMATION_CONTINUE;
}
