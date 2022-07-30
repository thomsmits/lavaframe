
#ifndef animation_h
#define animation_h

// Returned by the animation function if it wants to be called
// in the next loop
#define ANIMATION_CONTINUE  1

// Animation is finished, it does not want to be called again
#define ANIMATION_DONE      2

typedef int  (*animation_function_t)(int, int*);
typedef void (*setup_function_t)(void);

typedef struct {
    setup_function_t     setup_f;
    animation_function_t animation_f;
} animation_t;

#endif
