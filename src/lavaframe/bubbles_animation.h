#ifndef bubbles_animation_h
#define bubbles_animation_h

#include "types.h"
#include "animation.h"
#include "hardware.h"

class BubblesAnimation : public Animation {

    static const int bubbles = 6;    // how many moving bubbles?
    
    int scene = 0;
    
    double bubble_x[bubbles];             // x-position of bubbles
    double bubble_y[bubbles];             // y-position of bubbles
    int bubble_radius[bubbles];           // sizes of bubbles
    double bubble_speed[bubbles];         // bubbles moving in y direction
    
    double bubble_size;
    
    rgb_pixel_t palette[bubbles];
    
  
  public:
  
    BubblesAnimation() {      
    }
    
    virtual int animation(int*);
    virtual void setup();
    virtual void reset(); 
    
  private:

    void next_scene();
    void calc_bubble(int i);
    void draw_bubble(int i);
    byte col_interpolate(byte startValue, byte endValue, int steps, int pos);
};


#endif
