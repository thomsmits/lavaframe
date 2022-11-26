#include "animation.h"
#include "fire_animation.h"

void FireAnimation::setup() {
    scene = 0;
    next_scene();
}

void FireAnimation::reset() {
    setup();
}

PostAnimationAction FireAnimation::animation(int *delay_in_msec) {

    if (buttons.animation_interaction_pressed == true) {
      // next_scene();
    }

    calc_emitter();
    calc_fire();
    fire_to_leds();

    *delay_in_msec = 30;

    if (lavaFrame.next_animation_requested() == true) {
      lavaFrame.reset_next_animation_request();
      next_scene();
      if (scene == 1) {
        return PostAnimationAction::anim_done;
      }
    }

    return PostAnimationAction::anim_continue;
}

void FireAnimation::next_scene() {

    scene++;

    switch (scene) {

        case 2:
            // standard fire
            slowDown = 1;
            setup_palette_fire();
            intensity = 9;
            emitter_maxtop_y = LF_ROWS-1;
            break;

        /*case 2:
            slowDown = 1;
            intensity = 9;
            setup_palette_blue();
            emitter_maxtop_y = LF_ROWS-1;
            break;*/

        case 3:
            slowDown = 1;
            setup_palette_fire();
            intensity = 10;
            emitter_maxtop_y = 0;
            break;

        case 4:
            slowDown = 1;
            setup_palette_blue();
            intensity = 10;
            emitter_maxtop_y = LF_ROWS-1;
            break;

        default:
            // standard fire
            slowDown = 1;
            setup_palette_fire2();
            intensity = 9;
            emitter_maxtop_y = LF_ROWS-1;
            scene = 1;
            break;
    }

    for (int i=0; i < emitters; i++) {
        emitter_direction_x[i] = random(1,2);
        emitter_direction_y[i] = random(1,2);
    }

    for (int x = 0; x < LF_COLS; x++) {
        for (int y = 0; y < LF_ROWS; y++) {
            field[x][y] = 0;
        }
    }
}

void FireAnimation::calc_fire() {
    for (int y = 0; y < LF_ROWS-1; y++) {
        for (int x = 0; x < LF_COLS; x++) {
            int leftVal;

            if (x == 0)
                leftVal = field[x][y];
            else
                leftVal = field[x - 1][y];

            int rightVal;
            if (x == LF_COLS - 1)
                rightVal = field[x][y];
            else
                rightVal = field[x + 1][y];

            int belowVal = field[x][y + 1];

            int sum = leftVal + rightVal + (belowVal * 2);
            int avg = (int)((intensity * sum) / 40);

            // auto reduce it so you get lest of the forced fade and more vibrant fire waves
            if (avg > 0)
                avg--;

            if (avg < 0 || avg > 255) {
                avg = 0; // Average color calc is out of range 0-255 ?!?
            }

            field[x][y] = avg;
        }
    }
}

void FireAnimation::calc_emitter() {
    for (int x = 0; x < LF_COLS; x++) {
        field[x][LF_ROWS-1] = 0;
    }

    for (int i = 0; i < emitters;i++) {
        emitter_counter[i]++;

        if (emitter_counter[i] > (max(i*2, 1)) * slowDown) {

            emitter_counter[i] = 0;

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
                    field[e_x][e_y] = 255; // - abs(x) -abs(y);
                }
            }
        }
    }
}

void FireAnimation::fire_to_leds() {
    for (int x = 0; x < LF_COLS; x++) {
        for (int y = 0; y < LF_ROWS; y++) {
            byte value = field[x][LF_ROWS-1-y];
            rgb_pixel_t color = palette[value];
            rgb_pixel_t *px = lavaFrame.get_pixel(x, y);
            px->r = color.r;
            px->g = color.g;
            px->b = color.b;
        }
    }
    lavaFrame.push_to_strip();
}

void FireAnimation::setup_palette_fire2() {


  
  rgb_pixel_t colors[] = {
        { 0, 0, 0 },
        { 255, 0, 0 },
        { 255, 255, 0 },
        { 255, 255, 255 },
        { 0, 255, 255 }
    };

  fill_palette(palette, 256,  colors, 5); 
}


void FireAnimation::setup_palette_fire() {

    for (int i = 0; i < 64; i++) {
        palette[i].r = (byte)(i * 4);
        palette[i].g = (byte)(0);
        palette[i].b = (byte)(0);

        palette[i+64].r = (byte)(255);
        palette[i+64].g = (byte)(i * 4);
        palette[i+64].b = (byte)(0);

        palette[i+128].r = (byte)(255);
        palette[i+128].g = (byte)(255);
        palette[i+128].b = (byte)(i * 4);

        palette[i+192].r = (byte)(255 + - i * 4);
        palette[i+192].g = (byte)(255);
        palette[i+192].b = (byte)(i * 4);
    }
    return;
}

void FireAnimation::setup_palette_blue() {
    rgb_pixel_t colors[] = {
        { 0, 0, 0 },
        { 255, 0, 255 },
        { 0, 0, 255 },
        { 255, 255, 255 },
        { 255, 255, 255 }
    };
  fill_palette(palette, 256,  colors, 5);
}
