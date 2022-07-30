/**
 * Lavaframe
 * 
 * (c) 2022 Thomas Smits & Daniel Springwald
 * 
 * Main file.
 */
#include <FastLED.h>

#define DEBUG_OUTPUT
#include "trace.h"
#include "animation.h"

// Settings for the FastLED library
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS          96

// Pins 
#define SWITCH_1     27
#define SWITCH_2     26
#define DATA_PIN     15

// Size of the display
#define ROWS 13
#define COLS 13
#define NUM_LEDS     ROWS * COLS

// Type declarations
typedef unsigned char byte;

typedef struct {
    byte r;
    byte g;
    byte b;
} rgb_pixel_t;


// Internal representation of the pixels
static rgb_pixel_t frame[ROWS * COLS];

// NEO-Pixel array - do not write to it directly
static CRGB leds[NUM_LEDS];

// Macro to transform x/y coordinates into an index in the the frame array
#define _src(x, y) ((y) * COLS + (x))

// Macro to transform x/y coordinates into an index in the leds array
#define _tgt(x, y) (((y) % 2) ? ((y + 1) * COLS - (x + 1)) : ((y) * COLS + (x)))

// Macro to set the RGB values of a pixel
#define set_pixel(x, y, _r, _g, _b) \
            do { \
                rgb_pixel_t *pxl = &frame[_src((x), (y))]; \
                pxl->r = (byte) (_r); \
                pxl->g = (byte) (_g); \
                pxl->b = (byte) (_b); \
            } while (0)

// Macro to get the RGB value of a pixel
#define get_pixel(x, y) \
            (&frame[_src((x), (y))])

// Debug function to dump the contents of the current frame
#ifdef DEBUG_OUTPUT
void dump() {
    char buffer[50];
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            rgb_pixel_t *pxl = get_pixel(x, y);
            sprintf(buffer, "[%d, %d] -> #%02x%02x%02x", x, y, pxl->r, pxl->g, pxl->b);
            tracenl(buffer);         
        }
    }
}
#else
#define dump()
#endif

/**
 * Clear the current frame and intialize it with pure black
 * #000000
 */
void clear() {
    memset(frame, 0, sizeof(rgb_pixel_t) * ROWS * COLS);
}

/**
 * Fill the whole frame with a given color.
 * 
 * @param r red value
 * @param g green value
 * @param b blue value
 */
void fill(byte r, byte g, byte b) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            rgb_pixel_t *p = get_pixel(x, y);
            p->r = r;
            p->g = g;
            p->b = b;
        }
    }
}

/**
 * Pushes the internal representation to the strip.
 */
void push_to_strip() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            rgb_pixel_t *p = get_pixel(x, y);
            leds[_tgt(x, y)] = CRGB(p->r, p->g, p->b);
        }
    }
    FastLED.show();
}


#define ANIMATION_FUNCTION_COUNT 1

animation_function_t animation_functions[ANIMATION_FUNCTION_COUNT];
setup_function_t     setup_functions[ANIMATION_FUNCTION_COUNT];

/**
 * Register all known animations. New animations must be added here.
 */
void register_animations() {
   int idx = 0;
   
   // Test animation
   animation_functions[idx] = loop_test_animation;
   setup_functions[idx] = setup_test_animation;
   idx++;

   // Add more animations here. Increase ANIMATION_FUNCTION_COUNT
   // accordingly. Otherwise the functions will not be called
   // and the arrays overflow
   // ...
}

/**
 * Setup routine.
 */
void setup() {

    delay(3000);
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 
    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);

    setup_trace();
    
    pinMode(SWITCH_1, INPUT_PULLDOWN);
    pinMode(SWITCH_2, INPUT_PULLDOWN);

    // Initialize the animations
    for (int i = 0; i < ANIMATION_FUNCTION_COUNT; i++) {
        setup_functions[i]();
    }
}


static int loop_count = 0;
static int animation_index = 0;


/**
 * Main loop.
 */
void loop() {

    int switch_1 = digitalRead(SWITCH_1);
    int switch_2 = digitalRead(SWITCH_2);

    int delay_in_msec = 0;

    int ret_val = animation_functions[animation_index](loop_count, &delay_in_msec);

    if (rev_val == ANIMATION_CONTINUE) {
        // sleep as requested by the animation
        delay(delay_in_msec);
    } else if (ret_val == ANIMATION_DONE) {
        // Animation done, go to next animation
        animation_index++;  
    }

    if (animation_index >= ANIMATION_FUNCTION_COUNT) {
        // All animations done, start from beginning
        animation_index = 0;
    }
    
    loop_count++; // wrap around is intended!
}
