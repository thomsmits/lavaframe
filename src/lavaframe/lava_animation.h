#ifndef lava_animation_h
#define lava_animation_h

#include "types.h"
#include "animation.h"
#include "hardware.h"

/**
 * Animation of a lava animation.
 */
class LavaAnimation : public Animation {

  private:

    static const int bubbles = 6;       // how many moving bubbles?

    int scene = 0;                      // scene counter

    double bubble_x[bubbles];           // x-position of bubbles
    double bubble_y[bubbles];           // y-position of bubbles
    int bubble_radius[bubbles];         // sizes of bubbles
    double bubble_direction_x[bubbles]; // bubbles moving in x direction
    double bubble_direction_y[bubbles]; // bubbles moving in y direction

    double bubble_size;                 // size of the bubble

    int field[LF_COLS][LF_ROWS];
    rgb_pixel_t palette[255];

  public:

    /** Create a new instance. */
    LavaAnimation() {
    }

    /** @see Animation::animation(int) */
    virtual int animation(int*);

    /** @see Animation::setup() */
    virtual void setup();

    /** @see Animation::reset() */
    virtual void reset();

  private:

    byte col_interpolate(byte startValue, byte endValue, int steps, int pos);
    void field_to_leds();
    void setup_palette(rgb_pixel_t color_lava, rgb_pixel_t color_border, rgb_pixel_t color_background);
    void draw_bubble(int i);
    void calc_bubble(int i);
    void next_scene();
};

#endif
