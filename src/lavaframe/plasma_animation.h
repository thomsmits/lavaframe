#ifndef plasma_animation_h
#define plasma_animation_h

#include "types.h"
#include "animation.h"

#define LF_ROWS      13
#define LF_COLS      13
#define NUM_LEDS     LF_ROWS * LF_COLS

class PlasmaAnimation : public Animation {

  int field[LF_COLS][LF_ROWS+1];
  uint32_t plasma[LF_COLS][LF_ROWS+1];
  rgb_pixel_t palette[255];
  int paletteShift;
  
public:
    PlasmaAnimation() {
      
    }
    
    virtual int animation(int*);
    virtual void setup();
    virtual void reset(); 
    
private:
    void calcPlasma();
    void setup_palette();
    void setup_plasma();
};

#endif