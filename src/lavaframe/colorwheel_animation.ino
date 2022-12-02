#include "animation.h"
#include "colorwheel_animation.h"
#include "FastLED.h"

#define TWO_PI 6.283185307179586476925286766559

void ColorWheelAnimation::setup() {
    scene = 0;
    next_scene();
}

void ColorWheelAnimation::reset() {
}

PostAnimationAction ColorWheelAnimation::animation(int *delay_in_msec) {

    *delay_in_msec = 2;

    if (lavaFrame.next_animation_requested() == true) {
        lavaFrame.reset_next_animation_request();
        next_scene();
        if (scene == 1) {
            return PostAnimationAction::anim_done;
        }
      }

  calc_wheel();

  return PostAnimationAction::anim_continue;
}

void ColorWheelAnimation::next_scene() {

    scene++;

    rgb_pixel_t colors1[] = {
          { 0, 255, 255 },
          { 255, 0, 255 },
          { 255, 0, 255 },
          { 255, 255, 0 },
          { 0, 255, 255 },
        };


    rgb_pixel_t colors2[] = {
          { 128, 255, 0 },
          { 255, 0, 0 },
          { 255, 255, 0 },
          { 0, 0, 255 },
          { 0, 255, 255 },
          { 255, 255, 0 },
          { 255, 0, 0 },
          { 255, 255, 0 },
          { 255, 0, 255 },
          { 0, 255, 255 },
          { 128, 255, 0 }
        };

    rgb_pixel_t colors3[] = {
          { 255, 0, 0 },
          { 255 , 255, 0 },
          { 0 , 255, 255 },
          { 0, 0, 255 },
          { 255, 0 , 255 },
          { 255, 255, 0 },
          { 0, 255, 0 },
          { 255, 0, 0 }
        };
        
    switch (scene) {
      
      case 2:
        slowDownFactor = 2000;
         fill_palette(palette, 256,  colors2, 11); 
        break;

      case 3:
        slowDownFactor = 2000;
        fill_palette(palette, 256,  colors3, 8); 
        break;

      default:
        slowDownFactor = 2000;
        fill_palette(palette, 256,  colors1, 5); 
        scene = 1;
        break;
    }
}

void ColorWheelAnimation::calc_wheel()
{
    double time =  millis() / slowDownFactor;

    double halfCols = (LF_COLS - (LF_COLS % 2)) * 0.5;
    double halfRows = (LF_ROWS - (LF_ROWS % 2)) * 0.5;

    for(int y = 0; y < LF_ROWS; y++) {
        for(int x = 0; x < LF_COLS; x++) {
            
            double calcX = (x - halfCols) / halfCols;
            double calcY = (y - halfRows) / halfRows; 

            double angleRadians = atan2(calcX, calcY) + time;

            while (angleRadians < 0) angleRadians += TWO_PI;
            while (angleRadians > TWO_PI) angleRadians -= TWO_PI;
            
            int colNo = int((angleRadians / TWO_PI) * 255);
            
            rgb_pixel_t *px = lavaFrame.get_pixel(x, y);
            px->r = palette[colNo].r;
            px->g = palette[colNo].g;
            px->b = palette[colNo].b;
        }
    }
    lavaFrame.push_to_strip();
}
