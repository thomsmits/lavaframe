#ifndef circle_animation_h
#define circle_animation_h

#include "types.h"
#include "animation.h"
#include "hardware.h"

class CircleAnimation : public Animation {

  public:
  
    CircleAnimation() {      
    }
    
    virtual int animation(int*);
    virtual void setup();
    virtual void reset(); 
    
  private:

    virtual int animation_2(int*);
    virtual int animation_3(int*);
};

#endif
