#include "bubbles_animation.h"

void BubblesAnimation::setup() {
    scene = 0;
    next_scene();
}

void BubblesAnimation::reset() {
}

void BubblesAnimation::next_scene() {

    scene++;

    double slowDown = 300.0;

    switch (scene) {

      case 2:
          for (int i=0; i < 12; i++) {
              palette[i] = (rgb_pixel_t){255 - i * 10, 0, 255 - i * 10}; // purple
          }
          bubble_size = 0.2;
          break;

      default:
          palette[0] = (rgb_pixel_t){0, 255, 0};  // rainbow
          palette[1] = (rgb_pixel_t){0, 64, 0};
          palette[2] = (rgb_pixel_t){128, 0, 128};
          palette[3] = (rgb_pixel_t){255, 0,0};
          palette[4] = (rgb_pixel_t){64, 0, 0};
          palette[5] = (rgb_pixel_t){0, 0, 255};
          palette[6] = (rgb_pixel_t){0, 255, 0};
          palette[7] = (rgb_pixel_t){0, 64, 0};
          palette[8] = (rgb_pixel_t){128, 0, 128};
          palette[9] = (rgb_pixel_t){255, 0,0};
          palette[10] = (rgb_pixel_t){64, 0, 0};
          palette[11] = (rgb_pixel_t){0, 0, 255};
          bubble_size = 0.35;
          scene = 1;
          break;
    }

    for (int i=0; i < bubbles; i++) {
        bubble_x[i] = (i*5) % LF_COLS;
        bubble_y[i] = (i*7) % LF_ROWS; 
        bubble_speed[i] = 9 / slowDown + i / slowDown;
        bubble_radius[i] = (int)(2 + (i * bubble_size / 3.0));
    }
}

PostAnimationAction BubblesAnimation::animation(int *delay_in_msec) {

    *delay_in_msec = 10;

    lavaFrame.clear();    // clear field

    // draw bubbles
    for (int i=bubbles-1; i >=0; i--) {
        calc_bubble(i);
        draw_bubble(i);
    }

    lavaFrame.push_to_strip();

    if (lavaFrame.next_animation_requested() == true) {
        lavaFrame.reset_next_animation_request();
        next_scene();
        if (scene == 1) {
            return PostAnimationAction::anim_done;
        }
  }

  return PostAnimationAction::anim_continue;
}

void BubblesAnimation::calc_bubble(int i) {

    bubble_y[i] -= bubble_speed[i];

    if (bubble_y[i] <= -bubble_radius[i]) {
        bubble_y[i] = LF_ROWS + bubble_radius[i];
    }
}

void BubblesAnimation::draw_bubble(int i) {

    double b_x = bubble_x[i];
    double b_y = bubble_y[i];
    double radius = bubble_radius[i];

    for (int x = 0; x < LF_COLS; x++) {
        for (int y = 0; y < LF_ROWS; y++) {
            double dist = sqrt((b_x - x) * (b_x - x) + (b_y - y) * (b_y - y));
            rgb_pixel_t color;
            color = palette[i];
            if (radius < dist)  {
                double factor = min(1.0, max (0.0, 1 - (dist-radius) / (radius / 2.0)));
                color.r = byte(color.r * factor);
                color.g = byte(color.g * factor);
                color.b = byte(color.b * factor);
            }
            rgb_pixel_t *px = lavaFrame.get_pixel(x, LF_ROWS - 1 - y);
            if (px->r + px->g + px->b < color.r + color.g + color.b) {
                px->r = color.r;
                px->g = color.g;
                px->b = color.b;
            }
          }
    }
}

byte BubblesAnimation::col_interpolate(byte startValue, byte endValue, int steps, int pos) {
    return (byte)(startValue + ((endValue - startValue) * pos) / steps);
}
