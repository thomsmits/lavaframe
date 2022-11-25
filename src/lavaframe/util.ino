/**
 * Utility functions for the lava frame
 */

#include "util.h"

void lf_hsv_to_rgb(byte h, byte s, byte v, rgb_pixel_t* rgb) {

    if (s == 0) {
        rgb->r = v;
        rgb->g = v;
        rgb->b = v;
        return;
    }

    int region = h / 43;
    int remainder = (h - (region * 43)) * 6;

    int p = (v * (255 - s)) >> 8;
    int q = (v * (255 - (s * remainder) >> 8)) >> 8;
    int t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb->r = v; rgb->g = t; rgb->b = p;
            break;
        case 1:
            rgb->r = q; rgb->g = v; rgb->b = p;
            break;
        case 2:
            rgb->r = p; rgb->g = v; rgb->b = t;
            break;
        case 3:
            rgb->r = p; rgb->g = q; rgb->b = v;
            break;
        case 4:
            rgb->r = t; rgb->g = p; rgb->b = v;
            break;
        default:
            rgb->r = v; rgb->g = p; rgb->b = q;
            break;
    }
}

void lf_rgb_to_hsv(rgb_pixel_t* rgb, byte* h, byte* s, byte* v) {

    byte rgb_min, rgb_max;

    rgb_min = rgb->r < rgb->g ? (rgb->r < rgb->b ? rgb->r : rgb->b) : (rgb->g < rgb->b ? rgb->g : rgb->b);
    rgb_max = rgb->r > rgb->g ? (rgb->r > rgb->b ? rgb->r : rgb->b) : (rgb->g > rgb->b ? rgb->g : rgb->b);

    *v = rgb_max;
    if (*v == 0)
    {
        *h = 0;
        *s = 0;
        return;
    }

    *s = 255 * long(rgb_max - rgb_min) / *v;
    if (*s == 0) {
        *h = 0;
    }

    if (rgb_max == rgb->r) {
        *h = 0 + 43 * (rgb->g - rgb->b) / (rgb_max - rgb_min);
    }
    else if (rgb_max == rgb->g) {
        *h = 85 + 43 * (rgb->b - rgb->r) / (rgb_max - rgb_min);
    }
    else {
        *h = 171 + 43 * (rgb->r - rgb->g) / (rgb_max - rgb_min);
    }
}

// Prototype declaration as workaround for IDE bug.
// See: https://forum.arduino.cc/t/is-this-a-compiler-bug-typedefd-function-pointer-used-as-parameter-type-fails/325357/2
//void lf_draw_circle(int center_x, int center_y, int radius, lf_color_circle_function_t fn);

/**
 * Draw a solid circle with one color.
 *
 * @param center_x center of the circle (x coordinate)
 * @param center_y center of the circle (y coordinate)
 * @param radius radius of the circle
 * @param blend_colors if set to true, pixels will be mixed otherwise overwritten
 * @param color the color to be used for filling the circle
 */
void lf_draw_solid_circle(int center_x, int center_y, int radius, bool blend_colors, rgb_pixel_t color) {

    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {

                int x_pos = center_x + x;
                int y_pos = center_y + y;

                // Allow for offscreen coordinates
                if ((x_pos < LF_COLS) && (y_pos < LF_ROWS)) {

                    rgb_pixel_t *px = lavaFrame.get_pixel(x_pos, y_pos);

                    if (blend_colors) {
                        px->r = (px->r + color.r) / 2;
                        px->g = (px->g + color.g) / 2;
                        px->b = (px->b + color.b) / 2;
                    }
                    else {
                        px->r = color.r;
                        px->g = color.g;
                        px->b = color.b;
                    }
                }
            }
        }
    }
}


/**
 * Draw a circle with concentric colors.
 *
 * @param center_x center of the circle (x coordinate)
 * @param center_y center of the circle (y coordinate)
 * @param radius radius of the circle
 * @param blend_colors if set to true, pixels will be mixed otherwise overwritten
 * @param colors the colors to be used for filling the circle
 * @param num_colors size of the colors array
 */
void lf_draw_striped_circle(int center_x, int center_y, int radius, bool blend_colors, rgb_pixel_t* colors, int num_colors) {

  for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {

                int x_pos = center_x + x;
                int y_pos = center_y + y;

                // Allow for offscreen coordinates
                if ((x_pos < LF_COLS) && (y_pos  < LF_ROWS)) {

                    int r = (int) sqrt(x*x + y*y);
                    int color_idx = r - 1;

                    if ((color_idx < 0) ||(color_idx >= num_colors)) {
                      color_idx = 0;
                    }

                    rgb_pixel_t *px = lavaFrame.get_pixel(x_pos, y_pos);

                    if (blend_colors) {
                        px->r = (px->r + colors[color_idx].r) / 2;
                        px->g = (px->g + colors[color_idx].g) / 2;
                        px->b = (px->b + colors[color_idx].b) / 2;
                    }
                    else {
                        px->r = colors[color_idx].r;
                        px->g = colors[color_idx].g;
                        px->b = colors[color_idx].b;
                    }
                }
            }
        }
    }
}
