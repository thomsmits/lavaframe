#include "circle_animation.h"
#include "util.h"

void CircleAnimation::setup() {
}

void CircleAnimation::reset() {
}

PostAnimationAction CircleAnimation::animation(int *delay_in_msec) {

    static bool first_run = true;
    static double r = 1;
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
        lavaFrame.clear();
        first_run = false;
    }

    lf_draw_striped_circle(start_x, start_y, r, true, (colors + color_offset), sizeof(colors) - color_offset);
    lavaFrame.push_to_strip();

    r+= 0.1;

    if (r > max_radius) {
        r = 1;
        color_offset = random(0, sizeof(colors) - max_radius);
    }
    *delay_in_msec = 50;

    if (lavaFrame.next_animation_requested() == true) {
      lavaFrame.reset_next_animation_request();
      return PostAnimationAction::anim_done;
    }

    return PostAnimationAction::anim_continue;
}

int CircleAnimation::animation_2(int *delay_in_msec) {
    const int radius = 1;
    static float x_pos = LF_COLS / 2;
    static float y_pos = LF_ROWS / 2;

    static float v_x = 0.5;
    static float v_y = 1.0;

    lavaFrame.clear();

    rgb_pixel_t color = { 255, 255, 255 };

    lf_draw_solid_circle(x_pos, y_pos, radius, false, color);
    lavaFrame.push_to_strip();

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

    if (lavaFrame.next_animation_requested() == true) {
        return PostAnimationAction::anim_done;
    }

    return PostAnimationAction::anim_continue;
}

int CircleAnimation::animation_3(int *delay_in_msec) {
    static float x_pos = LF_COLS / 2;
    static float y_pos = LF_ROWS / 2;

    static float v_x = 0.5;
    static float v_y = 1.0;

    lavaFrame.clear();

    rgb_pixel_t *px = lavaFrame.get_pixel((int) x_pos, (int) y_pos);
    px->r = 255;
    px->g = 255;
    px->b = 255;

    lavaFrame.push_to_strip();

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

    if (lavaFrame.next_animation_requested() == true) {
        return PostAnimationAction::anim_done;
    }

    return PostAnimationAction::anim_continue;
}
