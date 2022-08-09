

#include "buttons.h"

unsigned long lf_last_button_update;

void lf_update_buttons() {

  long since_last_update = millis() -  lf_last_button_update;
  lf_last_button_update = millis();

  lf_button_1_was_pressed = lf_button_1_is_pressed;
  lf_button_2_was_pressed = lf_button_2_is_pressed;

  if (digitalRead(BUTTON_1_PIN) == HIGH) {
    if (lf_button_1_is_pressed == false) {
      lf_button_1_pressed_time = 0; // restart the pressed-counter
    } else {
      since_last_update += since_last_update;
    }
    lf_button_1_is_pressed = true;
  } else{
    lf_button_1_is_pressed = false;
  }

  if (digitalRead(BUTTON_2_PIN) == HIGH) {
    if (lf_button_2_is_pressed == false) {
      lf_button_2_pressed_time = 0; // restart the pressed-counter
    } else {
      since_last_update += since_last_update;
    }
    lf_button_2_is_pressed = true;
  } else{
    lf_button_2_is_pressed = false;
  }

}
