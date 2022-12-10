#ifndef frame_h
#define frame_h

#include "animation.h"
#include <EEPROM.h>

#define MIN_BRIGHTNESS  32  // 0 to 255
#define MAX_BRIGHTNESS  128 // 0 to 255

// Macro to transform x/y coordinates into an index in the the frame array
#define _src(x, y) ((y) * LF_COLS + (x))

// Macro to transform x/y coordinates into an index in the leds array
#define _tgt(x, y) (((y) % 2) ? ((y + 1) * LF_COLS - (x + 1)) : ((y) * LF_COLS + (x)))

// Maximum number of animations the frame can handle
#define MAX_ANIMATIONS 255

// time between animations when in slideshow mode (in seconds)
#define SLIDESHOW_DELAY 120  // 2 minutes

#define EEPROM_SIZE 512

#define EEPROM_ORIENTATION 0

/**
 * Class representing the interface to the lava frame itself.
 */
class Frame {
  private:

    // Orientation of the display
    byte orientation = 0;  // the rotation of the display (0-3), can be changed by button 

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
    /**
     * Register an animation with the frame. This method must be called
     * directly after constructing the object BEFORE all other methods.
     *
     * @param animation the animation to be registered.
     */
    void register_animation(Animation* animation);

    /**
     * Setup the frame. This method must be called AFTER register_animation
     * but before all other methods.
     */
    void setup();

    /**
     * This method has to be periodically called to drive the animation.
     */
    void loop();

    /**
     * Sends all changes to the strip.
     */
    void push_to_strip();

    /**
     * Clears the display.
     */
    void clear();

    /**
     * Fills the whole display with the given color.
     *
     * @param r red component of the color
     * @param g green component of the color
     * @param b blue component of the color
     */
    void fill(byte r, byte g, byte b);

    /**
     * Find out, if it is time for the next animation.
     * The decision should be made in animation when it is a good moment to change and not be forced.
     *
     * @return true if a shutdown was requested
     * @return false animation can continue
     */
    bool next_animation_requested();

    /**
     * Reset the flag checked by next_animation_requested()
     */
    void reset_next_animation_request();

    /**
     * Change the slideshow mode.
     */
    void change_slideshow_mode();

    /**
     * Dump the state of the display (for debugging purposes).
     */
    void dump();

    /**
     * Set a given pixel.
     *
     * @param x x-coordinate
     * @param y y-coordinate
     * @param r red component of the color
     * @param g green component of the color
     * @param b blue component of the color
     */
    void set_pixel(int x, int y, byte r, byte g, byte b);

    /**
     * Get a pointer to the pixel object at the given coordinate.
     *
     * @param x x-coordinate
     * @param y y-coordinate
     * @return pointer to the pixel
     */
    rgb_pixel_t* get_pixel(int x, int y) { return &frame[_src(x, y)]; }

  private:

    /**
     * This method is periodically called to adjust the brightness
     * depending on the ambient light sensor if present.
     */
    void adjust_global_brightness();

    void testEEProm();
};

#endif
