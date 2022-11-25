/*
 * 
 */
#ifndef animation_h
#define animation_h

// Returned by the animation function if it wants to be called
// in the next loop
#define LF_ANIMATION_CONTINUE  1

// Animation is finished, it does not want to be called again
#define LF_ANIMATION_DONE      2

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
    virtual int animation(int* delay_in_msec) = 0;

    /**
     * Reset the animation.
     */
    virtual void reset() = 0;
};

#endif
