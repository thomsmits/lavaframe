#include "buttons.h"

static unsigned long lf_last_button_update;

static int lf_button_1_pressed_time = 0;
static int lf_button_2_pressed_time = 0;

static bool lf_button_1_is_pressed = false;
static bool lf_button_2_is_pressed = false;

static bool lf_button_1_was_pressed = false;
static bool lf_button_2_was_pressed = false;

void lf_update_buttons() {

  long since_last_update = millis() -  lf_last_button_update;
  lf_last_button_update = millis();

  lf_button_1_was_pressed = lf_button_1_is_pressed;
  lf_button_2_was_pressed = lf_button_2_is_pressed;

  if (digitalRead(BUTTON_1_PIN) == HIGH) {
    if (lf_button_1_is_pressed == false) {
      lf_button_1_pressed_time = 0; // restart the pressed-counter
    } else {
      lf_button_1_pressed_time += since_last_update;
    }
    lf_button_1_is_pressed = true;
  } else{
    lf_button_1_is_pressed = false;
  }

  if (digitalRead(BUTTON_2_PIN) == HIGH) {
    if (lf_button_2_is_pressed == false) {
      lf_button_2_pressed_time = 0; // restart the pressed-counter
    } else {
      lf_button_2_pressed_time += since_last_update;
    }
    lf_button_2_is_pressed = true;
  } else{
    lf_button_2_is_pressed = false;
  }
  
  lf_update_interactions();

}

void lf_update_interactions() {

    lf_buttons_next_animation_requested = 
		lf_button_1_was_pressed == true && 
		lf_button_1_is_pressed == false && 
		lf_button_1_pressed_time < 1000; // short press button 1
		
	lf_buttons_slideshow_mode_is_hold = 
		lf_button_1_was_pressed == true && 
		lf_button_1_is_pressed == true && 
		lf_button_1_pressed_time > 1000; // hold button 1
		
	lf_buttons_animation_interaction_pressed =  
		lf_button_2_was_pressed == true && 
		lf_button_2_is_pressed == false && 
		lf_button_2_pressed_time < 1000; // short press button 2

	lf_buttons_max_brightness_is_hold = 
		lf_button_2_was_pressed == true && 
		lf_button_2_is_pressed == true && 
		lf_button_2_pressed_time > 1000; // hold button 2
}
