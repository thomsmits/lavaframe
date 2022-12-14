
	 _                      ______
	| |                    |  ____|
	| |     __ ___   ____ _| |__ _ __ __ _ _ __ ___   ___
	| |    / _` \ \ / / _` |  __| '__/ _` | '_ ` _ \ / _ \
	| |___| (_| |\ V / (_| | |  | | | (_| | | | | | |  __/
	|______\__,_| \_/ \__,_|_|  |_|  \__,_|_| |_| |_|\___|

	     LavaFrame. A meditative stylish light object
	  created 2022 by Thomas Smits and Daniel Springwald

This light object was created in 2022 in a hackathon based on
a white Ikea Sannahed frame 25x25 cm, 169 RGB LED and an ESP32.

<img src="images/LavaFrame-01.jpg" style="width:500px; margin: 5px;"/>

  * [License](LICENSE.md)
  * [Sources](src/lavaframe/README.md)


## Button usage

| button | state | description                                            |
| ------ | ----- | ------------------------------------------------------ |
| 1      | press | next animation                                         |
| 1      | hold  | change mode between *slideshow* and *single animation* |
| 2      | press | change rotation                                        |
| 2      | hold  | change brightness                                      |

## Animations already built in

### Lava

<p float="left">
<img src="images/Lava-01.gif" style="width:240px; margin: 5px;"/>
<img src="images/Lava-02.gif" style="width:240px; margin: 5px;"/>
<img src="images/Lava-03.gif" style="width:240px; margin: 5px;"/>
<img src="images/Lava-04.gif" style="width:240px; margin: 5px;"/>
</p>

### Fire

<p float="left">
<img src="images/Fire-01.gif" style="width:240px; margin: 5px;"/>
<img src="images/Fire-02.gif" style="width:240px; margin: 5px;"/>
<img src="images/Fire-03.gif" style="width:240px; margin: 5px;"/>
<img src="images/Fire-04.gif" style="width:240px; margin: 5px;"/>
</p>

### Color Wheel

<p float="left">
<img src="images/ColorCycle-01.gif" style="width:240px; margin: 5px;"/>
<img src="images/ColorCycle-02.gif" style="width:240px; margin: 5px;"/>
<img src="images/ColorCycle-03.gif" style="width:240px; margin: 5px;"/>
<img src="images/ColorCycle-04.gif" style="width:240px; margin: 5px;"/>
</p>

### Bubbles

<p float="left">
<img src="images/Bubbles-01.gif" style="width:240px; margin: 5px;"/>
<img src="images/Bubbles-02.gif" style="width:240px; margin: 5px;"/>
</p>

### Plasma

<img src="images/Plasma-01.gif" style="width:240px; margin: 5px;"/>


### Circle

<img src="images/Circle.gif" style="width:240px; margin: 5px;"/>

### Startup

<img src="images/Startup.gif" style="width:240px; margin: 5px;"/>

## How to write an animation?

If you want to add your own animation, you have to perform the following steps:

  * Create a subclass of `Animation`.
  * Register the class in the `setup` function of `lavaframe.ino`.

The easiest way is to start from the `TestAnimation` class, defined in `test_animation.ino` and `test_animation.h`.
