#ifndef test_animation_h
#define test_animation_h

#include "types.h"
#include "animation.h"
#include "hardware.h"

class TestAnimation : public Animation {
  
  public:
  
    TestAnimation() {      
    }
    
    virtual int animation(int*);
    virtual void setup();
    virtual void reset(); 
    
  private:

};


#endif
