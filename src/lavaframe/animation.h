/*
 * Base interface and definitions for all animations running
 * on the lava frame.
 */
#ifndef animation_h
#define animation_h

/**
 * Enumeration defining what the main program
 * should do after the animation function returns.
 */
enum PostAnimationAction {

    /**
     * Returned by the animation function if it wants to be called
     * in the next loop.
     */
    anim_continue = 1,

    /**
     * Animation is finished, it does not want to be called again.
     */
    anim_done = 2
};

/**
 * Base class (interface) for all animations.
 */
class Animation {

  public:

    /**
     * Setup the animation.
     */
    virtual void setup() = 0;

    /**
     * Run the animation.
     * @param (out) delay_in_msecs time the animation wants the external
     *              loop to delay
     * @return instructs the outer program what to do next
     */
    virtual PostAnimationAction animation(int* delay_in_msec) = 0;

    /**
     * Reset the animation.
     */
    virtual void reset() = 0;
};

#endif
