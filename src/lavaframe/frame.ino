#include "frame.h"


/**
 * Clear the current frame and initialize it with pure black
 * #000000
 */
void Frame::clear() {
    memset(frame, 0, sizeof(rgb_pixel_t) * LF_ROWS * LF_COLS);
}

/**
 * Fill the whole frame with a given color.
 *
 * @param r red value
 * @param g green value
 * @param b blue value
 */
void Frame::fill(byte r, byte g, byte b) {
    for (int y = 0; y < LF_ROWS; y++) {
        for (int x = 0; x < LF_COLS; x++) {
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
void Frame::push_to_strip() {
    for (int y = 0; y < LF_ROWS; y++) {
        for (int x = 0; x < LF_COLS; x++) {

            #if ORIENTATION == 0
              rgb_pixel_t *p = get_pixel(x, y);
            #elif ORIENTATION == 1
              rgb_pixel_t *p = get_pixel(y, LF_COLS - 1 - x);
            #elif ORIENTATION == 2
              rgb_pixel_t *p = get_pixel(LF_ROWS - 1 - x, LF_COLS - 1 - y);
            #elif ORIENTATION == 3
              rgb_pixel_t *p = get_pixel(LF_ROWS - 1 - y, x);
            #endif

            leds[_tgt(x, y)] = CRGB(p->r, p->g, p->b);
        }
    }
    FastLED.show();
}


void Frame::setup() {

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

    reset_next_animation_request();
}

void Frame::loop() {

    int delay_in_msec = 0;

    // Call the current animation function
    int ret_val = animations[animation_index]->animation(&delay_in_msec);

    buttons.update();

    if (buttons.slideshow_mode_is_hold == true) {
      change_slideshow_mode();
      while(buttons.slideshow_mode_is_hold == true) {
          buttons.update();
      }
    }

    EVERY_N_MILLISECONDS( 500 ) { adjust_global_brightness(); }

    int last_animation_index = animation_index;

    if (ret_val == PostAnimationAction::anim_continue) {
        // sleep as requested by the animation
        delay(delay_in_msec);
    } else if (ret_val == PostAnimationAction::anim_done) {
        // Animation done, go to next animation
        animation_index++;
    }

    if (last_animation_index != animation_index) {
      if (animation_index >= registered_animations_count) {
          // All animations done, start from beginning
          animation_index = 1; // not 0 to skip the intro animation
      }
      animations[animation_index]->reset();
      reset_next_animation_request();
    }

    loop_count++; // wrap around is intended!
}


// find out, if it is time for the next animation.
// however, the decision should be made in animation when it is a good moment to change and not be forced.
bool Frame::next_animation_requested() {
    if (buttons.next_animation_requested == true)  {
        return true;
    }
    if (slideshow_mode == true && millis() - slideshow_last_action > SLIDESHOW_DELAY * 1000) {
        reset_next_animation_request();
        return true;
    }
    return false;
}

bool Frame::reset_next_animation_request() {
    slideshow_last_action = millis();
}


void Frame::change_slideshow_mode () {

    slideshow_mode = !slideshow_mode;

    int borderX = LF_COLS * 0.25;
    int borderY = LF_ROWS * 0.25;

    for (int i = 0; i < 3; i++) { // blink 3 times

        clear();
        if (slideshow_mode == true) {
            // green triangle
            for (int x = 0; x < LF_COLS - 2 * borderX; x++) {
                int heightDiv2 = max(1, (int)(x * 0.75));
                for (int y = -heightDiv2; y < heightDiv2-1 ; y++) {
                    rgb_pixel_t *pxl = get_pixel(LF_COLS - x - borderX, LF_ROWS / 2 + y);
                    pxl->r = 0;
                    pxl->g = 255;
                    pxl->b = 0;
                }
            }
        } else {
          // red square
            for (int x = borderX; x < LF_COLS - borderX; x++) {
                for (int y = borderY; y < LF_ROWS - borderY; y++) {
                    rgb_pixel_t *pxl = get_pixel(x, y);
                    pxl->r = 255;
                    pxl->g = 0;
                    pxl->b = 0;
                }
            }
        }
        push_to_strip();
        delay(300);

        clear();  // clear neopixel field
        push_to_strip();
        delay(200);
    }
    reset_next_animation_request();
}


void Frame::adjust_global_brightness() {

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

// Debug function to dump the contents of the current frame
#ifdef DEBUG_OUTPUT
void Frame::dump() {
    char buffer[50];
    for (int y = 0; y < LF_ROWS; y++) {
        for (int x = 0; x < LF_COLS; x++) {
            rgb_pixel_t *pxl = get_pixel(x, y);
            sprintf(buffer, "[%d, %d] -> #%02x%02x%02x", x, y, pxl->r, pxl->g, pxl->b);
            tracenl(buffer);
        }
    }
}
#else
#define lf_dump()
#endif

void Frame::set_pixel(int x, int y, byte _r, byte _g, byte _b) {
    rgb_pixel_t *pxl = &frame[_src((x), (y))];
    pxl->r = (byte) (_r);
    pxl->g = (byte) (_g);
    pxl->b = (byte) (_b);
}

void Frame::register_animation(Animation* animation) {
    animations[registered_animations_count++] = animation;
}
