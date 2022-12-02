#ifndef colorwheel_animation_h
#define colorwheel_animation_h

#include "types.h"
#include "animation.h"

class ColorWheelAnimation : public Animation {

    #define dist(x1, y1, x2, y2) sqrt(double((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)))

    int scene;

    double slowDownFactor;

    rgb_pixel_t palette[256];

public:

    /** Create a new instance. */
    ColorWheelAnimation() {
    }

    /** @see Animation::animation(int) */
    virtual PostAnimationAction animation(int*);

    /** @see Animation::setup() */
    virtual void setup();

    /** @see Animation::reset() */
    virtual void reset();

private:
    void next_scene();
    void calc_wheel();
};

#endif
