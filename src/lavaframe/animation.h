
#ifndef animation_h
#define animation_h

// Returned by the animation function if it wants to be called
// in the next loop
#define LF_ANIMATION_CONTINUE  1

// Animation is finished, it does not want to be called again
#define LF_ANIMATION_DONE      2

typedef int  (*lf_animation_function_t)(int*);
typedef void (*lf_setup_function_t)(void);
typedef void (*lf_reset_function_t)(void);

/**
 * Struct containing the callback functions for an animation.
 */
typedef struct {
    /** Setup function */
    lf_setup_function_t     setup_f;

    /** Reset function */
    lf_reset_function_t     reset_f;

    /** The animation itself */
    lf_animation_function_t animation_f;
} lf_animation_t;

#endif
