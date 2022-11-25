#include "intro_animation.h"

void IntroAnimation::setup() {
}

void IntroAnimation::reset() {
}

int IntroAnimation::animation(int *delay_in_msec) {

    lf_clear();

    const double targetHeight = 0.8 * LF_ROWS;
    static double actualHeight = -2;
    const double explosionLength = 50.0;
    static double explosion = 0;

    if (actualHeight < targetHeight) {

        // up to the sky
        double x = LF_COLS / 2;
        double y = actualHeight += LF_ROWS * 0.05;
        for (int i=0; i < intro_animation_points; i++) {
            draw_point(x, y, &intro_animation_palette[i % (sizeof(intro_animation_palette))]);
        }

    } else if (explosion++ <= explosionLength * 1.2) {
        // explode

        for (int i=0; i < intro_animation_points; i++) {
            float angle = i * 180.0 / intro_animation_points;
            double radius = LF_COLS / explosionLength * explosion  + i * 0.1;
            double x = LF_COLS / 2 + radius * sin(angle);
            double y = actualHeight + radius * cos(angle);
            draw_point(x, y, &intro_animation_palette[i % (sizeof(intro_animation_palette))]);
        }

    } else {
        return LF_ANIMATION_DONE;
    }

    lf_push_to_strip();

    *delay_in_msec = 10;

    if (lf_next_animation_requested() == true) {
        return LF_ANIMATION_DONE;
    }

    return LF_ANIMATION_CONTINUE;
}


void IntroAnimation::draw_point(double b_x, double b_y, rgb_pixel_t *color) {

    double radius = 1.5; //max(LF_COLS / 7.0, LF_ROWS / 7.0);

    rgb_pixel_t col;

    for (int x = 0; x < LF_COLS; x++) {
        for (int y = 0; y < LF_ROWS; y++) {
            col = (rgb_pixel_t){color->r,color->g,color->b};
            double dist = sqrt((b_x - x) * (b_x - x) + (b_y - y) * (b_y - y));
            if (radius < dist) {
                double factor = min(0.1, max (0.0, 1 - (dist-radius) / (radius / 2))) / 3.0;
                col.r = byte(col.r * factor);
                col.g = byte(col.g * factor);
                col.b = byte(col.b * factor);
            }
            rgb_pixel_t *px = lf_get_pixel(x, y);
            if (px->r + px->g + px->b < col.r + col.g + col.b) {
                px->r = col.r;
                px->g = col.g;
                px->b = col.b;
            }
        }
    }
}
