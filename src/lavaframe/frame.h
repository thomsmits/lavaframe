#ifndef frame_h
#define frame_h

#include "animation.h"

#define MIN_BRIGHTNESS  32  // 0 to 255
#define MAX_BRIGHTNESS  128 // 0 to 255

// Macro to transform x/y coordinates into an index in the the frame array
#define _src(x, y) ((y) * LF_COLS + (x))

// Macro to transform x/y coordinates into an index in the leds array
#define _tgt(x, y) (((y) % 2) ? ((y + 1) * LF_COLS - (x + 1)) : ((y) * LF_COLS + (x)))

// Maxium number of animations the frame can handle
#define MAX_ANIMATIONS 255

// time between animations when in slideshow mode (in seconds)
#define SLIDESHOW_DELAY 120  // 2 minutes

/**
 * Class representing the interface to the lava frame itself.
 */
class Frame {
  private:
  
    // Counter for the loops
    int loop_count = 0;

    // index of the current animation
    int animation_index = 0;
    
    // Brightness adjustment
    int actual_Brightness = MAX_BRIGHTNESS;

    // NEO-Pixel array - do not write to it directly
    CRGB leds[NUM_LEDS];

    // Number of known animations
    int registered_animations_count = 0;
    
    // Array of the animation functions
    Animation* animations[MAX_ANIMATIONS];

    // indicator for the slideshow mode
    bool slideshow_mode = true;

    // Last action
    unsigned long slideshow_last_action;

  public:
    // Internal representation of the pixels
    rgb_pixel_t frame[LF_ROWS * LF_COLS];

  public:
    void setup();
    void loop();
    void push_to_strip();
    void clear();
    void fill(byte r, byte g, byte b);
    bool next_animation_requested();
    bool reset_next_animation_request();
    void change_slideshow_mode();
    void adjust_global_brightness();
    void dump();
    void set_pixel(int x, int y, byte _r, byte _g, byte _b);
    rgb_pixel_t* get_pixel(int x, int y) { return &frame[_src(x, y)]; }
    void register_animation(Animation* animation);
};

#endif
