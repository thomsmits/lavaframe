#include "animation.h"

lf_animation_t fire_animation = {
  fire_animation_calc_fire,
  loop_fire_animation
};

void setup_fire_animation() {
}

int loop_fire_animation(int *delay_in_msec) {
    static int x = 0;
    static int y = 0;

    if (x >= LF_COLS) {
        x = 0;
        y++;
    }

    if (y >= LF_ROWS) {
        x = 0;
        y = 0;
        return LF_ANIMATION_DONE;
    }

    x++;

    rgb_pixel_t *px = lf_get_pixel(x, y);
    px->r = 10;
    px->g = 10;
    px->b = 100;
    lf_push_to_strip();

    lf_clear();

    *delay_in_msec = 100;

    return LF_ANIMATION_CONTINUE;
}

/*
void fire_animation_generateBaseline()
{
  for (int x = 0; x < LF_COLS; x++) {
    int nBytePos = GetBytePos(x, (int)finger.Y);
    _fireData[nBytePos] = GetRandomNumber();
  }
}
*/


void fire_animation_calc_fire()
{
  /*
    GenerateBaseline();

    for (int y = 0; y < _height - 1; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            int leftVal;

            if (x == 0)
                leftVal = _fireData[GetBytePos(_width - 1, y)];
            else
                leftVal = _fireData[GetBytePos(x - 1, y)];

            int rightVal;
            if (x == _width - 1)
                rightVal = _fireData[GetBytePos(0, y)];
            else
                rightVal = _fireData[GetBytePos(x + 1, y)];

            int belowVal = _fireData[GetBytePos(x, y + 1)];

            int sum = leftVal + rightVal + (belowVal * 2);
            int avg = sum / 4;

            // auto reduce it so you get lest of the forced fade and more vibrant fire waves
            if (avg > 0)
                avg--;

            if (avg < 0 || avg > 255)
                throw new Exception("Average color calc is out of range 0-255");

            _fireData[GetBytePos(x, y)] = (byte)avg;
        }
    }
    */
}
