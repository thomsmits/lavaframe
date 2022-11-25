/**
 * Lavaframe
 *
 * (c) 2022 Thomas Smits & Daniel Springwald
 *
 * Main file.
 */
 
#include <FastLED.h>
#include "types.h"
#include "buttons.h"

#define DEBUG_OUTPUT
#include "trace.h"
#include "animation.h"
#include "intro_animation.h"

// Settings for the FastLED library
#define LED_TYPE        WS2811
#define COLOR_ORDER     GRB
#define MIN_BRIGHTNESS  32  // 0 to 255
#define MAX_BRIGHTNESS  128 // 0 to 255

// Data pin for neopixel stripe
#define DATA_PIN     15

// Size of the display
#define LF_ROWS      13
#define LF_COLS      13
#define NUM_LEDS     LF_ROWS * LF_COLS

// Orientation of the display
#define ORIENTATION  2  // sets the rotation of the display (0-3)

// Pins for the buttons
#define BUTTON_1_PIN  27   // short press: next animation; hold: switch between automatic animation slideshow and a single static animation
#define BUTTON_2_PIN  26   // short press: interaction with actual animation; hold: adjust maximum brightness

// Pin for ambient light sensor (define only if LDR exists!)
#define LDR_PIN      34

// time between animations when in slideshow mode (in seconds)
#define SLIDESHOW_DELAY 120  // 2 minutes



static bool lf_slideshow_mode = true;
static unsigned long lf_slideshow_last_action;

// find out, if it is time for the next animation.
// however, the decision should be made in animation when it is a good moment to change and not be forced.
static bool lf_next_animation_requested() {
  if (lf_buttons_next_animation_requested == true)  {
    return true;
  }
  if (lf_slideshow_mode == true && millis() - lf_slideshow_last_action > SLIDESHOW_DELAY * 1000) {
    lf_reset_next_animation_request();
    return true;
  }
  return false;
}

static bool lf_reset_next_animation_request() {
  lf_slideshow_last_action = millis();
}



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
#define lf_get_pixel(x, y) (&frame[_src((x), (y))])

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
 * Clear the current frame and initialize it with pure black
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

            #if ORIENTATION == 0
              rgb_pixel_t *p = lf_get_pixel(x, y);
            #elif ORIENTATION == 1
              rgb_pixel_t *p = lf_get_pixel(y, LF_COLS - 1 - x);
            #elif ORIENTATION == 2
              rgb_pixel_t *p = lf_get_pixel(LF_ROWS - 1 - x, LF_COLS - 1 - y);
            #elif ORIENTATION == 3
              rgb_pixel_t *p = lf_get_pixel(LF_ROWS - 1 - y, x);
            #endif

            leds[_tgt(x, y)] = CRGB(p->r, p->g, p->b);
        }
    }
    FastLED.show();
}




#define MAX_ANIMATIONS 20  // Max number of animations the frame can handle

// Array of the animation functions
Animation* animations[MAX_ANIMATIONS];

// Animations
//extern lf_animation_t intro_animation;
extern lf_animation_t bubbles_animation;
extern lf_animation_t lava_animation;
extern lf_animation_t fire_animation;
//extern lf_animation_t plasma_animation;
extern lf_animation_t circle_animation;
extern lf_animation_t test_animation;

int registered_animations_count = 0;

/**
 * Register all known animations. New animations must be added here.
 */
static void register_animations() {


   animations[registered_animations_count++]     = new IntroAnimation;
   ;
   /*
   //animations[registered_animations_count++]   = test_animation;
   animations[registered_animations_count++]     = lava_animation;
   animations[registered_animations_count++]     = bubbles_animation;
   animations[registered_animations_count++]     = fire_animation;
   //animations[registered_animations_count++]   = plasma_animation;
   //animations[registered_animations_count++]   = circle_animation;
*/
   // Add more animations here. Increase MAX_ANIMATIONS
   // accordingly if you exceed the number of slots in the array.
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

    pinMode(BUTTON_1_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_2_PIN, INPUT_PULLDOWN);

    #ifdef LDR_PIN
      pinMode(LDR_PIN, INPUT_PULLDOWN);
    #endif

     adjust_global_brightness();

    // Initialize the animations
    for (int i = 0; i < registered_animations_count; i++) {
        animations[i]->setup();
    }

    lf_reset_next_animation_request();
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
    int ret_val = animations[animation_index]->animation(&delay_in_msec);

    lf_update_buttons();

    if (lf_buttons_slideshow_mode_is_hold == true) {
      change_slideshow_mode();
      while(lf_buttons_slideshow_mode_is_hold == true) {
          lf_update_buttons();
      }
    }

    EVERY_N_MILLISECONDS( 500 ) { adjust_global_brightness(); }

    int last_animation_index = animation_index;

    if (ret_val == LF_ANIMATION_CONTINUE) {
        // sleep as requested by the animation
        delay(delay_in_msec);
    } else if (ret_val == LF_ANIMATION_DONE) {
        // Animation done, go to next animation
        animation_index++;
    }

    if (last_animation_index != animation_index) {
      if (animation_index >= registered_animations_count) {
          // All animations done, start from beginning
          animation_index = 1; // not 0 to skip the intro animation
      }
      animations[animation_index]->reset();
      lf_reset_next_animation_request();
    }

    loop_count++; // wrap around is intended!
}


void change_slideshow_mode () {

    lf_slideshow_mode = !lf_slideshow_mode;

    int borderX = LF_COLS * 0.25;
    int borderY = LF_ROWS * 0.25;

    for (int i = 0; i < 3; i++) { // blink 3 times

        lf_clear();
        if (lf_slideshow_mode == true) {
          // green triangle
          for (int x = 0; x < LF_COLS - 2 * borderX; x++) {
            int heightDiv2 = max(1, (int)(x * 0.75));
            for (int y = -heightDiv2; y < heightDiv2-1 ; y++) {
               rgb_pixel_t *pxl = lf_get_pixel(LF_COLS - x - borderX, LF_ROWS / 2 + y);
               pxl->r = 0;
               pxl->g = 255;
               pxl->b = 0;
            }
          }
        } else {
          // red square
          for (int x = borderX; x < LF_COLS - borderX; x++) {
            for (int y = borderY; y < LF_ROWS - borderY; y++) {
               rgb_pixel_t *pxl = lf_get_pixel(x, y);
               pxl->r = 255;
               pxl->g = 0;
               pxl->b = 0;
            }
          }
        }
        lf_push_to_strip();
        delay(300);

        lf_clear();  // clear neopixel field
        lf_push_to_strip();
        delay(200);
    }
    lf_reset_next_animation_request();
}


/**
 * brightness adjustment
 */

static int actual_Brightness = MAX_BRIGHTNESS;

void adjust_global_brightness () {

  #ifdef LDR_PIN
    const int minSensor = 2100;  // sensor value when totally dark (0 to 4096)
    const int maxSensor = 3500;  // sensor value when totally bright (0 to 4096)
    int lightValue = analogRead(LDR_PIN);
    int targetBrightness =  min(MAX_BRIGHTNESS,max(MIN_BRIGHTNESS,((lightValue-maxSensor) * MAX_BRIGHTNESS) / (maxSensor-minSensor)));
    actual_Brightness = actual_Brightness + (targetBrightness - actual_Brightness) / 3;
    FastLED.setBrightness(actual_Brightness);  // set master brightness control
  #else
    FastLED.setBrightness(MAX_BRIGHTNESS);   // set master brightness control
  #endif

}
