#ifndef plasma_animation_h
#define plasma_animation_h

#include "types.h"
#include "animation.h"

#define LF_ROWS      13
#define LF_COLS      13
#define NUM_LEDS     LF_ROWS * LF_COLS

class PlasmaAnimation : public Animation {

  #define dist(x1, y1, x2, y2) sqrt(double((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)))

 
public:
    PlasmaAnimation() {
      
    }
    
    virtual int animation(int*);
    virtual void setup();
    virtual void reset(); 
    
private:
    void calc_plasma();
};

#endif
