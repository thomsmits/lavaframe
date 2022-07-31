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
#define LED_TYPE        WS2811
#define COLOR_ORDER     GRB
#define MIN_BRIGHTNESS  32  // 0 to 255
#define MAX_BRIGHTNESS  255 // 0 to 255

// Pins for the buttons
#define SWITCH_1_PIN  27
#define SWITCH_2_PIN  26

// Data pin for neopixel stripe
#define DATA_PIN     15

// Pin for ambient light sensor
#define LDR_PIN      34

// Size of the display
#define LF_ROWS      13
#define LF_COLS      13
#define NUM_LEDS     LF_ROWS * LF_COLS

// Type declarations
typedef unsigned char byte;

// A single pixel
typedef struct {
    byte r;
    byte g;
    byte b;
} rgb_pixel_t;


// Internal representation of the pixels
static rgb_pixel_t frame[LF_ROWS * LF_COLS];

// NEO-Pixel array - do not write to it directly
static CRGB leds[NUM_LEDS];

// Macro to transform x/y coordinates into an index in the the frame array
#define _src(x, y) ((y) * LF_COLS + (x))

// Macro to transform x/y coordinates into an index in the leds array
#define _tgt(x, y) (((y) % 2) ? ((y + 1) * LF_COLS - (x + 1)) : ((y) * LF_COLS + (x)))

// Macro to set the RGB values of a pixel
#define lf_set_pixel(x, y, _r, _g, _b) \
            do { \
                rgb_pixel_t *pxl = &frame[_src((x), (y))]; \
                pxl->r = (byte) (_r); \
                pxl->g = (byte) (_g); \
                pxl->b = (byte) (_b); \
            } while (0)

// Macro to get the RGB value of a pixel
#define lf_get_pixel(x, y) \
            (&frame[_src((x), (y))])

// Debug function to dump the contents of the current frame
#ifdef DEBUG_OUTPUT
void lf_dump() {
    char buffer[50];
    for (int y = 0; y < LF_ROWS; y++) {
        for (int x = 0; x < LF_COLS; x++) {
            rgb_pixel_t *pxl = lf_get_pixel(x, y);
            sprintf(buffer, "[%d, %d] -> #%02x%02x%02x", x, y, pxl->r, pxl->g, pxl->b);
            tracenl(buffer);
        }
    }
}
#else
#define lf_dump()
#endif

/**
 * Clear the current frame and intialize it with pure black
 * #000000
 */
void lf_clear() {
    memset(frame, 0, sizeof(rgb_pixel_t) * LF_ROWS * LF_COLS);
}

/**
 * Fill the whole frame with a given color.
 *
 * @param r red value
 * @param g green value
 * @param b blue value
 */
void lf_fill(byte r, byte g, byte b) {
    for (int y = 0; y < LF_ROWS; y++) {
        for (int x = 0; x < LF_COLS; x++) {
            rgb_pixel_t *p = lf_get_pixel(x, y);
            p->r = r;
            p->g = g;
            p->b = b;
        }
    }
}

/**
 * Pushes the internal representation to the strip.
 */
void lf_push_to_strip() {
    for (int y = 0; y < LF_ROWS; y++) {
        for (int x = 0; x < LF_COLS; x++) {
            rgb_pixel_t *p = lf_get_pixel(x, y);
            leds[_tgt(x, y)] = CRGB(p->r, p->g, p->b);
        }
    }
    FastLED.show();
}


bool lf_animation_choice_switch_pressed() {
  return digitalRead(SWITCH_1_PIN) == HIGH;
}

bool lf_function_switch_1_pressed() {
   return digitalRead(SWITCH_2_PIN) == HIGH;
}


#define ANIMATION_FUNCTION_COUNT 3

// Array of the animation functions
lf_animation_t animations[ANIMATION_FUNCTION_COUNT];

// Animations
extern lf_animation_t fire_animation;
//extern lf_animation_t plasma_animation;
extern lf_animation_t circle_animation;
extern lf_animation_t test_animation;


/**
 * Register all known animations. New animations must be added here.
 */
static void register_animations() {
   int idx = 0;

   // Test animation
   animations[idx++]     = fire_animation;
   //animations[idx++]     = plasma_animation;
   animations[idx++]     = circle_animation;
   animations[idx++]     = test_animation;

   // Add more animations here. Increase ANIMATION_FUNCTION_COUNT
   // accordingly. Otherwise the functions will not be called
   // and the arrays overflow
   // ...
}

/**
 * Setup routine.
 */
void setup() {

    register_animations();

    delay(3000);
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

    setup_trace();

    pinMode(SWITCH_1_PIN, INPUT_PULLDOWN);
    pinMode(SWITCH_2_PIN, INPUT_PULLDOWN);
    pinMode(LDR_PIN, INPUT_PULLDOWN);

     adjust_global_brightness();

    // Initialize the animations
    for (int i = 0; i < ANIMATION_FUNCTION_COUNT; i++) {
        //setup_functions[i]();
        animations[i].setup_f();
    }
}

// Counter for the loops
static int loop_count = 0;

// index of the current animation
static int animation_index = 0;

/**
 * Main loop.
 */
void loop() {

    int delay_in_msec = 0;

    // Call the current animation function
    int ret_val = animations[animation_index].animation_f(&delay_in_msec);

    EVERY_N_MILLISECONDS( 500 ) { adjust_global_brightness(); }

    int last_animation_index = animation_index;

    if (lf_animation_choice_switch_pressed()) {
        // force next animation
        animation_index++; 
        while(lf_animation_choice_switch_pressed()) {delay(10);}
    } else if (ret_val == LF_ANIMATION_CONTINUE) {
        // sleep as requested by the animation
        delay(delay_in_msec);
    } else if (ret_val == LF_ANIMATION_DONE) {
        // Animation done, go to next animation
        animation_index++;
    }

    if (last_animation_index != animation_index) {
      if (animation_index >= ANIMATION_FUNCTION_COUNT) {
          // All animations done, start from beginning
          animation_index = 0;
      }
      animations[animation_index].reset_f();
    }

    loop_count++; // wrap around is intended!
}


/**
 * brightness adjustment
 */

static int actual_Brightness = MAX_BRIGHTNESS;

void adjust_global_brightness () {
    const int minSensor = 2100;  // sensor value when totally dark (0 to 4096)
    const int maxSensor = 3500;  // sensor value when totally bright (0 to 4096)
    int lightValue = analogRead(LDR_PIN); 
    int targetBrightness =  min(MAX_BRIGHTNESS,max(MIN_BRIGHTNESS,((lightValue-maxSensor) * MAX_BRIGHTNESS) / (maxSensor-minSensor))); 
    actual_Brightness = actual_Brightness + (targetBrightness - actual_Brightness) / 3;
    FastLED.setBrightness(actual_Brightness);   // set master brightness control
} 
