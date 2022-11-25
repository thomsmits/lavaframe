#ifndef test_animation_h
#define test_animation_h

#include "types.h"
#include "animation.h"
#include "hardware.h"

/**
 * Test animation, simply turning one pixel after the other on and off.
 */
class TestAnimation : public Animation {

  public:

    /** Create a new instance. */
    TestAnimation() {
    }

    /** @see Animation::animation(int) */
    virtual int animation(int*);

    /** @see Animation::setup() */
    virtual void setup();

    /** @see Animation::reset() */
    virtual void reset();

  private:

};


#endif
