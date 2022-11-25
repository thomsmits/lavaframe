/**
 * Utility functions for the lava frame
 */

#ifndef util_h
#define util_h

/**
 * Convert an HSV value to an RGB value and write it directly into the
 * given pixe.
 *
 * @param h hue
 * @param s saturation
 * @param v brightness
 * @param (out) rgb result
 */
void lf_hsv_to_rgb(byte h, byte s, byte v, rgb_pixel_t* rgb);

/**
 * Convert an RGB value to HSV values.
 *
 * @param rgb the RGB pixel
 * @param (out) h hue
 * @param (out) s saturation
 * @param (out) v brightness
 */
void lf_rgb_to_hsv(rgb_pixel_t* rgb, byte* h, byte* s, byte* v);

//typedef void (*lf_color_circle_function_t)(int, rgb_pixel_t*);

//void lf_draw_circle(int center_x, int center_y, int radius, byte r, byte g, byte b, int blend_colors, lf_color_circle_function_t fn);


/**
 * Draw a solid circle with one color.
 *
 * @param center_x center of the circle (x coordinate)
 * @param center_y center of the circle (y coordinate)
 * @param radius radius of the circle
 * @param blend_colors if set to true, pixels will be mixed otherwise overwritten
 * @param color the color to be used for filling the circle
 */
void lf_draw_solid_circle(int center_x, int center_y, int radius, bool blend_colors, rgb_pixel_t color);

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
void lf_draw_striped_circle(int center_x, int center_y, int radius, bool blend_colors, rgb_pixel_t* colors, int num_colors);

#endif
