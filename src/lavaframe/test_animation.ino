#include "animation.h"

void setup_test_animation() {
  
}

int loop_test_animation(int loop_count, int *delay_in_msec) {
    static int x = 0;
    static int y = 0;

    if (x >= COLS) {
        x = 0;
        y++;
    }

    if (y >= ROWS) {
        x = 0;
        y = 0;
        return ANIMATION_DONE;
    }

    x++;

    rgb_pixel_t *px = get_pixel(x, y);
    px->r = 10;
    px->g = 10;
    px->b = 100;
    push_to_strip();
    
    clear();

    *delay_in_msec = 100;
    
    return ANIMATION_CONTINUE;
}
