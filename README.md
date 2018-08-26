# Slowtech midi controller

![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/source/Render_01.jpg)

// Based on https://github.com/FuzzyWobble/TeensyMonster


# Plans

- Modify to work with a Teensy LC.

- Fix led lights corresponding to button presses.

- Toggle button sending midi at release. Choosable? 

- Also to multiplex buttons and to show potentiometer value as a graph bar at an 128x64px oled display.

- Choose midi channel by holding one of the 16 buttons at startup?

- Possibility to set led brightness in some way. Button combination at startup to choose between 16 different levels?

- Save last set midi channel and buttonstate(?) to EEPROM for later startup. 

- Function button?

- Encoders?

# Designprocess

![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/source/2017-02-21%2009.27.03.jpg)

![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/source/Render.JPG)

Prototyping with 2 x 595 and one 4067:
![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/hardware/designprocess/IMG_3610.JPG)

Buttons with 12 V LEDs.
![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/hardware/designprocess/IMG_3611.JPG)

Replaced the leds with 2.1 V.
![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/hardware/designprocess/IMG_3682.JPG)

# Code

Resources:

https://www.pjrc.com/teensy/teensyduino.html

https://www.pjrc.com/teensy/td_midi.html

https://www.pjrc.com/teensy/td_libs_SPI.html

https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf

# Hardware

- Teensy LC
- 595 shift registers (to be replaced with TLC5940NT or similar)
- 4067 multiplexers
- SSD 1306 128x64px oled i2c display
- Tactile non-toggle switches with LEDS. 

The two 595 shift registers will be replaced with TLC5940NT 16 channel LED PWM driver. 

Only needs one resistor to the IC instead of one per LED.

Prototype one will be made with through hole components, afterwards designed with SMD to make everything neater and smaller.


# Diagrams

Diagram over display and potentiometers are on the way.

Buttons and leds to the Teensy LC microcontroller.
![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/hardware/Buttons_leds_diagram.jpg)

Shift register 595 
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/595_cascade.png)

595 table over I/O and names
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/595_tabel.png)

SPI table over Teensy LC I/O (we are using alternate pin 14 for SCK/clock) 
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/Teensy%20SPI.png)

Multiplexer 4067
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/cd4067.png)

I2C table
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/Teensy%20i2c.png)


# Open source is key

Big thanks to the Swedish facebook group Arduinobubblan and the Arduino community forum for supporting and helping along the way.
