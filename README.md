# Slowtech midi controller

![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/source/Render_01.jpg)

// Based on https://github.com/FuzzyWobble/TeensyMonster


# To do:

- Class compliant. [check]

- Compatible with Teensy. [check]

- Multiplex potentiometers via analog input from IC 4067. [check]

- Show potentiometer value as a graph bar at an 128x64px oled display. Show midi channel and cc number of corresponding potentiometer. [check]

- Multiplex buttons via digital input from IC 4067. [check]

- Buss PCB layout with ribbon connectors for easy customisaton and scalability [v.3.0 hardware check]

- LED callback from DAW/synth through shift register IC 595. [need integration]

- Choose midi channel by holding one of the 16 buttons at startup, and store to EEPROM. [need research]

- Sync CC/note values with computer and send midi in both directions to correct the "starting point". [need research]

- Possibility to set led brightness. Button combination at startup to choose between 16 different levels. [need research]



# Design process

![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/source/2017-02-21%2009.27.03.jpg)

![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/source/Render.JPG)

Breadboard prototype v 1.0:
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/Breadboard.jpg)

Prototyping with two 595 and two 4067 IC's:
![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/hardware/designprocess/IMG_3610.JPG)

Buttons with 12 V LEDs:
![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/hardware/designprocess/IMG_3611.JPG)

Backside of Teensy board:
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/designprocess/IMG_3688.JPG)

Frontside of Teensy board:
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/designprocess/IMG_3695_1.jpg)

Prototype v 2.0:
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/designprocess/IMG_3693_1.jpg)

Prototype v 3.0, 2v LEDS and pull-up resistors:
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/designprocess/IMG_6982.JPG)

Prototype v 3.0, 2v LEDS and pull-up resistors:
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/designprocess/IMG_6981.JPG)

Testing potentiometers with Ableton Live throgh USB midi.

https://www.youtube.com/watch?v=3gG5vzuutMg

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
- Tactile non-toggle switches
- LEDs for feedback

The two 595 shift registers will be replaced with TLC5940NT 16 channel LED PWM driver. 

Only needs one resistor to the IC instead of one per LED.

Prototype one will be made with through hole components, afterwards designed with SMD to make everything neater and smaller.


# Diagrams


Complete diagram of version: slowtech_3.0 (needs revision on switch-board routing)
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/Slowtech_complete_3.0.jpg)


595 table over I/O and names:
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/595_tabel.png)


SPI table over Teensy LC I/O (we are using alternate pin 14 for SCK/clock): 
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/Teensy%20SPI.png)


I2C table:

![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/Teensy%20i2c.png)


# Open source is key

Big thanks to the Swedish facebook group Arduinobubblan and the Arduino community forum for supporting and helping along the way.

This project is free to develope and use according to MIT licence. Keep it open source for the future!
