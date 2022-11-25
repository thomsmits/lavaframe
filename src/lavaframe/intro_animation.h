/*
 * Introduction animation, shown when the lavaframe starts.
 */
#ifndef intro_animation_h
#define intro_animation_h

#include "types.h"
#include "animation.h"

class IntroAnimation : public Animation {

  const int intro_animation_points = 40;  // how many moving points?
  rgb_pixel_t intro_animation_palette[5] = { (rgb_pixel_t){255,0,255},
                                          (rgb_pixel_t){186,85,211},
                                          (rgb_pixel_t){255,20,147},
                                          (rgb_pixel_t){0,191,255},
                                          (rgb_pixel_t){255,182,193}};
public:
    IntroAnimation() {      
    }
    
    virtual int animation(int*);
    virtual void setup();
    virtual void reset(); 
    
private:
    void draw_point(double b_x, double b_y, rgb_pixel_t *color);
};

#endif
