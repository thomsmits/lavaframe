#ifndef fire_animation_h
#define fire_animation_h

#include "types.h"
#include "animation.h"
#include "hardware.h"

/**
 * Animation of flames.
 */
class FireAnimation : public Animation {

  private:

    static const int emitters   = 4;   // how many moving fire emitters on the baseline?

    int intensity        = 9;          // 1-10

    int slowDown         = 1;          // emitter movement; 1 = full speed, 2 = half speed and so on...

    int emitter_x[emitters];           // x-position of all emitters
    int emitter_y[emitters];           // y-position of all emitters
    int emitter_counter[emitters];     // emitter movement counter
    int emitter_direction_x[emitters]; // emitter moving in x direction
    int emitter_direction_y[emitters]; // emitter moving in y direction
    int emitter_maxtop_y = LF_ROWS-1;  // how high can an emitter go? 0 = max top, LF_ROWS-1 = baseline
    int field[LF_COLS][LF_ROWS+1];
    rgb_pixel_t palette[255];

    int scene = 0;

  public:

    /** Create a new instance. */
    FireAnimation() {
    }

    /** @see Animation::animation(int) */
    virtual int animation(int*);

    /** @see Animation::setup() */
    virtual void setup();

    /** @see Animation::reset() */
    virtual void reset();

  private:

    void next_scene();
    void calc_fire();
    void calc_emitter();
    void fire_to_leds();
    void setup_palette_fire();
    void setup_palette_blue();
};

#endif
