
#include "animation.h"
#include "lava_animation.h"

void LavaAnimation::setup() {
    scene = 0;
    next_scene();
}

void LavaAnimation::reset() {
}

void LavaAnimation::next_scene() {

    scene++;

    switch (scene) {
        case 2:
            setup_palette({0, 255, 0}, {0, 64, 0}, {128, 0, 128}); // green, purple background
            bubble_size = 0.35;
            break;

        case 3:
            setup_palette({255, 0, 0}, {64, 0, 0}, {0, 0, 0}); // red
            bubble_size = 0.35;
            break;

        case 4:
            setup_palette({255, 255, 255}, {0, 0, 0}, {0, 0, 64}); // white, blue background
            bubble_size = 0.4;
            break;

        default:
            setup_palette({255, 0, 255}, {0, 0, 255}, {0, 0, 0}); // purple, blue border
            bubble_size = 0.55;
            scene = 1;
            break;
    }

    int border = 3;

    for (int i = 0; i < bubbles; i++) {
        bubble_x[i] = border + (((LF_COLS - 2 - border * 2) * i ) / bubbles ) ;
        bubble_y[i] = (i * 3) % LF_ROWS;
        bubble_direction_x[i] = 0; //random(1,2);
        bubble_direction_y[i] = 0.01 + i / 200.0;
        bubble_radius[i] = (int)(1 + (i * bubble_size));
    }
}

int LavaAnimation::animation(int *delay_in_msec) {

    *delay_in_msec = 30;

    if (lf_buttons_animation_interaction_pressed == true) {
        // next_scene();
    }

    // clear field
    for (int x = 0; x < LF_COLS; x++) {
        for (int y = 0; y < LF_ROWS; y++) {
            field[x][y] = 0;
        }
    }

    // draw bubbles
    for (int i = 0; i < bubbles; i++) {
        calc_bubble(i);
        draw_bubble(i);
    }

    field_to_leds();

    if (lf_next_animation_requested() == true) {
        lf_reset_next_animation_request();
        next_scene();
        if (scene == 1) {
            return LF_ANIMATION_DONE;
        }
    }

    return LF_ANIMATION_CONTINUE;
}

void LavaAnimation::calc_bubble(int i) {

    bubble_y[i] += bubble_direction_y[i];

    if (bubble_y[i] < 1) {
        bubble_direction_y[i] = -bubble_direction_y[i];
        bubble_y[i] = 1;
    }

    if (bubble_y[i] >= LF_ROWS - 1 ) {
        bubble_direction_y[i] = -bubble_direction_y[i];
        bubble_y[i] = LF_ROWS - 1;
    }
}

void LavaAnimation::draw_bubble(int i) {

    double b_x = bubble_x[i];
    double b_y = bubble_y[i];
    double radius = bubble_radius[i];

    for (int x = 0; x < LF_COLS; x++) {
        for (int y = 0; y < LF_ROWS; y++) {
            double dist = sqrt((b_x - x) * (b_x - x) + (b_y - y) * (b_y - y));
            int col = 0;
            if (radius > dist)  {
                col = 255;
            } else {
                col = int(255 - (dist * 0.9 - radius) * 255);
            }
            field[x][y] = max(field[x][y], min(255, col));
        }
    }
}

void LavaAnimation::setup_palette(rgb_pixel_t color_lava, rgb_pixel_t color_border, rgb_pixel_t color_background) {

    for (int i = 0; i <= 128; i++)   {
        palette[255 - i].r = col_interpolate(color_lava.r, color_border.r, 128, i);
        palette[255 - i].g = col_interpolate(color_lava.g, color_border.g, 128, i);
        palette[255 - i].b = col_interpolate(color_lava.b, color_border.b, 128, i);

        palette[128 - i].r = col_interpolate(color_border.r, color_background.r, 128, i);
        palette[128 - i].g = col_interpolate(color_border.g, color_background.g, 128, i);
        palette[128 - i].b = col_interpolate(color_border.b, color_background.b, 128, i);
    }
}

byte LavaAnimation::col_interpolate(byte startValue, byte endValue, int steps, int pos) {
    return (byte)(startValue + ((endValue - startValue) * pos) / steps);
}

void LavaAnimation::field_to_leds() {
    for (int x = 0; x < LF_COLS; x++) {
        for (int y = 0; y < LF_ROWS; y++) {
            byte value = field[x][y];
            rgb_pixel_t color = palette[value];
            rgb_pixel_t *px = lf_get_pixel(x, y);
            px->r = color.r;
            px->g = color.g;
            px->b = color.b;
        }
    }
    lf_push_to_strip();
}
