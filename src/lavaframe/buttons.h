/**
 * Interpretation of the button states
 */

#ifndef buttons_h
#define buttons_h

static bool lf_buttons_next_animation_requested = false;
static bool lf_buttons_animation_interaction_pressed = false;
static bool lf_buttons_slideshow_mode_is_hold = false;
static bool lf_buttons_max_brightness_is_hold = false;



void lf_update_buttons();

#endif
