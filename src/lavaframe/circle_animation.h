#ifndef circle_animation_h
#define circle_animation_h

#include "types.h"
#include "animation.h"
#include "hardware.h"

/**
 * Animation of concentric circles.
 */
class CircleAnimation : public Animation {

  public:

    /** Create a new instance. */
    CircleAnimation() {
    }

   /** @see Animation::animation(int) */
    virtual int animation(int*);

    /** @see Animation::setup() */
    virtual void setup();

    /** @see Animation::reset() */
    virtual void reset();

  private:

    virtual int animation_2(int*);
    virtual int animation_3(int*);
};

#endif
