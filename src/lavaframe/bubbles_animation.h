#ifndef bubbles_animation_h
#define bubbles_animation_h

#include "types.h"
#include "animation.h"
#include "hardware.h"

/**
 * Animation of color or monochrome bubbles bubbling up.
 */
class BubblesAnimation : public Animation {

  private:

    static const int bubbles = 6;  // how many moving bubbles?

    int scene = 0;                 // Scene currently displayed

    double bubble_x[bubbles];      // x-position of bubbles
    double bubble_y[bubbles];      // y-position of bubbles
    int bubble_radius[bubbles];    // sizes of bubbles
    double bubble_speed[bubbles];  // bubbles moving in y direction
    double bubble_size;            // size of the bubble

    rgb_pixel_t palette[bubbles];  // color palette for the bubbles

  public:

    /** Create a new instance. */
    BubblesAnimation() {
    }

    /** @see Animation::animation(int) */
    virtual PostAnimationAction animation(int*);

    /** @see Animation::setup() */
    virtual void setup();

    /** @see Animation::reset() */
    virtual void reset();

  private:

    void next_scene();
    void calc_bubble(int i);
    void draw_bubble(int i);
    byte col_interpolate(byte startValue, byte endValue, int steps, int pos);
};

#endif
