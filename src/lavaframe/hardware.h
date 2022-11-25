#ifndef hardware_h
#define hardware_h

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

#endif
