/**
 * Utility functions for the lava frame
 */

#ifndef util_h
#define util_h

static int lf_button_1_pressed_time = 0;
static int lf_button_2_pressed_time = 0;

static bool lf_button_1_is_pressed = false;
static bool lf_button_2_is_pressed = false;

static bool lf_button_1_was_pressed = false;
static bool lf_button_2_was_pressed = false;

void lf_update_buttons();

#endif
