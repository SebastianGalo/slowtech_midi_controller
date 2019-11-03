



//MIDI CHANNEL_____________________________________________
int channelNumber = 1; 



//POTENTIOMETER THRESHOLD_______________________________
//jitter smoothing
int analogThreshold = 4;



//VAL+PIN TO DISPLAY__________________________________
//for displaying potentiometer values
int global_val, global_pin;



//BUTTON DEBOUNCE TIME___________________________________
//150 millisecond originally
uint32_t pbBounce = 150; 



//ANALOG MULTIPLEXER 1 READING, FOR POTENTIOMETERS________________
//'1' for multiplexer I/O you want to send signal, else enter '0'
//IC pin number are written below
int toReadAnalogMux1[16] = {
  1, 1, 1, 1, //0-3
  1, 1, 1, 1, //4-7
  1, 1, 1, 1, //8-11
  1, 1, 1, 1 //12-15
};



//DIGITAL MULTIPLEXER 1 READING, FOR PUSHBUTTONS______________
//'1' for multiplexer I/O you want to send signal, else enter '0'
//IC pin number are written below
int toReadDigitalMux1[16] = {
  1, 1, 1, 1, //0-3
  1, 1, 1, 1, //4-7
  1, 1, 1, 1, //8-11
  1, 1, 1, 1 //12-15
};



//PUSHBUTTON MODE______________________________________
//'1' - standard mode, when pushbutton is engaged note is turned on, when pushbutton is released, note is turned off
//'2' - on mode, note is only turned on with each click
//'3' - off mode, note is only turned off with each click
//'4' - toggle mode, note is switched between on and off with each click
//IC pin number are written below
int pushbuttonMode[16] = {
  2, 2, 2, 2, //0-3
  2, 2, 2, 2, //4-7
  2, 2, 2, 2, //8-11
  2, 2, 2, 2 //12-15 (NOTE: comma after last row/number for shift function to continue down under)
};



//PUSHBUTTON LEDS__________________________________________
//IC pin number are written below
//'1' for shift register pin you want to send signal to, else enter '0'
int toDisplayLed[16] = {
  1, 1, 1, 1, //0-3
  1, 1, 1, 1, //4-7
  1, 1, 1, 1, //8-11
  1, 1, 1, 1 //12-15
};



//SHIFT REGISTERS FOR PUSHBUTTON LEDS__________________________
//set number of shift registers and number of bytes in buttonState + toggleState array
const int numOfBytes = 2; 



