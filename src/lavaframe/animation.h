
#ifndef animation_h
#define animation_h

#define ANIMATION_CONTINUE  1
#define ANIMATION_DONE      2

typedef int (*animation_function_t) (int, int*);
typedef void (*setup_function_t)(void);

#endif
