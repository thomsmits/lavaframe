#ifndef hardware_h
#define hardware_h

// Data pin for neopixel stripe
#define DATA_PIN     15

// Size of the display
#define LF_ROWS      13
#define LF_COLS      13
#define NUM_LEDS     LF_ROWS * LF_COLS


// Pins for the buttons
#define BUTTON_1_PIN  27   // short press: next animation; hold: switch between automatic animation slideshow and a single static animation
#define BUTTON_2_PIN  26   // short press: interaction with actual animation; hold: adjust maximum brightness

// Pin for ambient light sensor (define only if LDR exists!)
// #define LDR_PIN      34

// Settings for the FastLED library
#define LED_TYPE        WS2811
#define COLOR_ORDER     GRB

#endif
