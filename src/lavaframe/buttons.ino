#include "buttons.h"

void Buttons::update() {

    long since_last_update = millis() -  last_update;
    last_update = millis();

    button_1_was_pressed = button_1_is_pressed;
    button_2_was_pressed = button_2_is_pressed;

    if (digitalRead(BUTTON_1_PIN) == HIGH) {
        if (button_1_is_pressed == false) {
            button_1_pressed_time = 0; // restart the pressed-counter
        } else {
            button_1_pressed_time += since_last_update;
        }
        button_1_is_pressed = true;
    } else {
        button_1_is_pressed = false;
    }

    if (digitalRead(BUTTON_2_PIN) == HIGH) {
        if (button_2_is_pressed == false) {
            button_2_pressed_time = 0; // restart the pressed-counter
        } else {
            button_2_pressed_time += since_last_update;
        }
        button_2_is_pressed = true;
    } else {
        button_2_is_pressed = false;
    }

    update_interactions();

}

void Buttons::update_interactions() {

    const int hold_ms = 700;

    next_animation_requested =
    		button_1_was_pressed == true &&
    		button_1_is_pressed == false &&
    		button_1_pressed_time < hold_ms; // short press button 1

  	slideshow_mode_is_hold =
    		button_1_was_pressed == true &&
    		button_1_is_pressed == true &&
    		button_1_pressed_time > hold_ms; // hold button 1

  	animation_interaction_pressed =
    		button_2_was_pressed == true &&
    		button_2_is_pressed == false &&
    		button_2_pressed_time < hold_ms; // short press button 2

  	max_brightness_is_hold =
    		button_2_was_pressed == true &&
    		button_2_is_pressed == true &&
    		button_2_pressed_time > hold_ms; // hold button 2
}
