#include "animation.h"
#include "plasma_animation.h"
#include "FastLED.h"

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

void PlasmaAnimation::setup() {
  setup_palette();
  setup_plasma();
}

void PlasmaAnimation::reset() {
}

int PlasmaAnimation::animation(int *delay_in_msec) {

  *delay_in_msec = 30;

  if (lf_next_animation_requested() == true) {
    lf_reset_next_animation_request();
    return LF_ANIMATION_DONE;
  }

  return LF_ANIMATION_CONTINUE;
}

void PlasmaAnimation::calcPlasma()
{
    //the parameter to shift the palette varies with time
    paletteShift = int(millis() / 10.0);

    //draw every pixel again, with the shifted palette color
    for (int y = 0; y < LF_ROWS-1; y++)
    {
        for (int x = 0; x < LF_COLS; x++)
        {
            //field[x][y] = palette[(plasma[y][x] + paletteShift) % 256];
            field[x][y] = (plasma[y][x] + paletteShift) % 256;
        }
    }
}

void PlasmaAnimation::setup_palette()
{
    for (int i = 0; i < 64; i++)
    {
        fire_animation_palette[i].r = (byte)(i * 4);
        fire_animation_palette[i].g = (byte)(0);
        fire_animation_palette[i].b = (byte)(0);

        fire_animation_palette[i+64].r = (byte)(255);
        fire_animation_palette[i+64].g = (byte)(i * 4);
        fire_animation_palette[i+64].b = (byte)(0);

        fire_animation_palette[i+128].r = (byte)(255);
        fire_animation_palette[i+128].g = (byte)(255);
        fire_animation_palette[i+128].b = (byte)(i * 4);

        fire_animation_palette[i+192].r = (byte)(255 + - i * 4);
        fire_animation_palette[i+192].g = (byte)(255);
        fire_animation_palette[i+192].b = (byte)(i * 4);
    }    
}

void PlasmaAnimation::setup_plasma()
{
   //generate the plasma once
  for(int y = 0; y < LF_ROWS; y++)
    for(int x = 0; x < LF_COLS; x++)
    {
      //the plasma buffer is a sum of sines
      int color = int
      (
          128.0 + (128.0 * sin(x / 16.0))
        + 128.0 + (128.0 * sin(y / 16.0))
      ) / 2;
      plasma[y][x] = color;
    }
}

/*
void PlasmaAnimation::plasma() {                                                 // This is the heart of this program. Sure is short. . . and fast.

  int thisPhase = beatsin8(6,-64,64);                           // Setting phase change for a couple of waves.
  int thatPhase = beatsin8(7,-64,64);

  
  for (int k=0; k<NUM_LEDS; k++) {                              // For each of the LED's in the strand, set a brightness based on a wave as follows:

    int colorIndex = cubicwave8((k*23)+thisPhase)/2 + cos8((k*15)+thatPhase)/2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
    int thisBright = qsuba(colorIndex, beatsin8(7,0,96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

    leds[k] = ColorFromPalette(currentPalette, colorIndex, thisBright, currentBlending);  // Let's now add the foreground colour.
  }

} // plasma()*/
