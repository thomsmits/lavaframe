/*
 * Lavaframe
 *
 * (c) 2022 Thomas Smits & Daniel Springwald
 *
 * Main file.
 */

#include <FastLED.h>
#include "types.h"
#include "buttons.h"
#include "hardware.h"
#include "frame.h"

#define DEBUG_OUTPUT
#include "trace.h"
#include "animation.h"

// Load headers for all custom animations here
#include "intro_animation.h"
#include "lava_animation.h"
#include "fire_animation.h"
#include "test_animation.h"
#include "bubbles_animation.h"
#include "circle_animation.h"
#include "plasma_animation.h"

// Access to the button state
Buttons buttons;

// Object representing the frame
Frame lavaFrame;

/**
 * Setup routine.
 */
void setup() {
    // Add all animations you want to show here
    lavaFrame.register_animation(new IntroAnimation);
    lavaFrame.register_animation(new LavaAnimation);
    lavaFrame.register_animation(new FireAnimation);
    lavaFrame.register_animation(new TestAnimation);
    lavaFrame.register_animation(new BubblesAnimation);
    lavaFrame.register_animation(new CircleAnimation);
    lavaFrame.register_animation(new PlasmaAnimation);

    // Add more animations here. See MAX_ANIMATIONS in frame.h for the maximum number
    // ...
   
    lavaFrame.setup();
}

/**
 * Main loop.
 */
void loop() {
  lavaFrame.loop();
}
