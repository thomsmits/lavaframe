#ifndef intro_animation_h
#define intro_animation_h

#include "types.h"
#include "animation.h"

/**
 * Introduction animation, shown when the lava frame starts.
 */
class IntroAnimation : public Animation {

  private:

    static const int intro_animation_points = 40;  // how many moving points?

    rgb_pixel_t intro_animation_palette[5] = { (rgb_pixel_t){255,0,255},
                                               (rgb_pixel_t){186,85,211},
                                               (rgb_pixel_t){255,20,147},
                                               (rgb_pixel_t){0,191,255},
                                               (rgb_pixel_t){255,182,193}};
  public:

    /** Create a new instance. */
    IntroAnimation() {
    }

    /** @see Animation::animation(int) */
    virtual PostAnimationAction animation(int*);

    /** @see Animation::setup() */
    virtual void setup();

    /** @see Animation::reset() */
    virtual void reset();

private:

    void draw_point(double b_x, double b_y, rgb_pixel_t *color);
};

#endif
