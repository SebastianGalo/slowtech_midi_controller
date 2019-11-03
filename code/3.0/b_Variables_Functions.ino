


// TEENSY LED________________________________________
//blinks when alive!
//pin 13 has the LED on Teensy LC and 3.0
//ledState used to set the LED
const int led = 13;  
int ledState = LOW;             
//Generally, you should use "unsigned long" for variables that hold time,
//the value will quickly become too large for an int to store.
//will store last time LED was updated
unsigned long previousMillis = 0;
//constants won't change
//interval at which to blink (milliseconds)
const long interval = 800;



//MULTIPLEXER READING SIGNAL, INPUT PINS________________________________
//reading pins for signal from multiplexers
int analogSignalMux1 = 15; 
int digitalSignalMux1 = 0; 



//MULTIPLEXER CONTROL SIGNAL, OUTPUT PINS_______________________
int s0 = 1; //control pin A
int s1 = 2; //control pin B
int s2 = 3; //control pin C
int s3 = 4; //control pin D



//MULTIPLEXER INHIBITOR, OUTPUT PINS_____________________________
//is this right? Can the muxes share inhibitor pin input or do they have to be separate?
//reading pins for inhibitor from multiplexers
int inhibitAnalogMux1 = 5;
int inhibitDigitalMux1 = 9;



//SHIFT REGISTER FOR PUSHBUTTON LEDS, OUTPUT PINS_____________________
int latchPin = 10; // ST_CP, SS (Storage register clock pin)
int clockPin = 14; // SH_CP, SCK (Shift register clock pin) Set to alternative pin 14 instead of 13!!
int dataPin = 11;  // DS, MOSI (data out from Teensy, into 595)



//ANALOG IN MULTIPLEXER 1________________________________
int analogInsPrev1[16]; //array to hold previously read analog values - set all to zero for now
int tempAnalogInMux1 = 0; //array to hold previously read analog values
int tempAnalogInMap1 = 0;
int analogControlPin1[] = {s0, s1, s2, s3}; //set contol pins in array
//control array
int analogMuxChannel1[16][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0},
  {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
  {0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}
};
//function to read mux
int analogReadMux1(int analogChannel1) {
  //loop through the four control pins
  for (int i = 0; i < 4; i ++) {
    //turn on/off the appropriate control pins according to what channel we are trying to read
    digitalWrite(analogControlPin1[i], analogMuxChannel1[analogChannel1][i]);
  }
  //read the value of the pin
  int analogVal1 = analogRead(analogSignalMux1);
  //return the value
  return analogVal1;
}


//DIGITAL IN MULTIPLEXER 1________________________________
unsigned long timeHit[16];
boolean buttonState[numOfBytes * 8]; //array with stored state from mode 1,2,3
boolean toggleState[numOfBytes * 8]; //array with stored state from mode 4
boolean callbackState[numOfBytes * 8]; //array with stored state from PC

boolean tempDigitalRead1 = 0;//array to hold previously read values
int digitalControlPin1[] = {s0, s1, s2, s3}; //set contol pins in array
//control array
int digitalMuxChannel1[16][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0},
  {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
  {0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}
};
//function to read mux
int digitalReadMux1(int digitalChannel1) {
  //loop through the four control pins
  for (int i = 0; i < 4; i ++) {
    delayMicroseconds (20);
    //turn on/off the appropriate control pins according to what channel we are trying to read
    digitalWrite(digitalControlPin1[i], digitalMuxChannel1[digitalChannel1][i]);
  }
  //read the value of the pin
  int digitalVal1 = digitalRead(digitalSignalMux1);
  //return the value
  return digitalVal1;
}



