/**
 * Interpretation of the button states
 */

#ifndef buttons_h
#define buttons_h

class Buttons {

  private:
    unsigned long last_update;
    int button_1_pressed_time = 0;
    int button_2_pressed_time = 0;
    bool button_1_is_pressed = false;
    bool button_2_is_pressed = false;
    bool button_1_was_pressed = false;
    bool button_2_was_pressed = false;

  public:
    bool next_animation_requested = false;
    bool animation_interaction_pressed = false;
    bool slideshow_mode_is_hold = false;
    bool max_brightness_is_hold = false;

  public:
    void update();

  private:
    void update_interactions();
};
#endif
