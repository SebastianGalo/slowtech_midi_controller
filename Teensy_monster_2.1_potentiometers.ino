// Teensy Monster V1
// teensymonster.cc
// Code v1.0
// last updated April, 2014
// license __ http://opensource.org/licenses/MIT
// Edit: Sebastian Galo 2018

// Use name.c as tab in the sketch to show custom midi name
// Code for the project with 16 potentiometers tested, working great beside some fluxuation and jumping values.
// It's probably due to low quality hardware. Possible to make the values even smoother in the code?


//libraries
#include <Encoder.h> //download from http://www.pjrc.com/teensy/td_libs_Encoder.html
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define  OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

// ------------------------------------------------------------------------------------------------------------------------------------
// EDIT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//THRESHOLD
//you can increase this if you have some jitter problems on your analog devices
//it is best to solve your jitter problems according to the Instructable
int analogThreshold = 10;

//....................................................
//DEBUG_______________________________________________
//enable if you want to test your output in the serial monitor
boolean enableDebug = 0;  // 1 for enable, 0 for disable. **Remember to disable debug when running MIDI**.

//....................................................
//CHANNEL_____________________________________________
int channelNumber = 1; //each controller should have a unique channel number between 1 and 10

//....................................................
//PIN+VAL TO DISPLAY__________________________________
int global_val, global_pin; 

//LEDS BRIGHTNESS__________________________________
int pwmLed = 255;

//...................................................
//DEBOUNCE BUTTONS___________________________________
//debounce is a measurement of the time in which a pushbutton is unresponsive after it is pressed
//this is important to prevent unwanted double clicks
uint32_t pbBounce = 300; //150 millisecond debounce duration - you may want to change this value depending on the mechanics of your pushbuttons

//....................................................
//SHIFT FUNCTION BUTTONS______________________________
//shift buttons offer dual functionality to your pushbuttons and encoders
//if using a shift button enter the pin number here, else put 0
int shiftPin = 0;

//....................................................
//ROTARY ENCODER______________________________________
//encoders require two digital pins
//encoders can be read in two modes: best performance, good performance
//for best performance, two digital iterrupt pins are required
//for good performance, one digitial interrupt pin and one regular digital pin are required
//you can read three encoders in the best perfomance mode
//you can read six encoders in the good performance mode
//Teensy interrupt (INT) pins are 2-12,13-15,20-23
//{2,5} - example of good performance read (0 is an interrupt pin, and 5 is a regular digital pin)
//note that interrupt pin MUST come first - {5,0} would not work
//{2,3} - example of best performance read (both 18 and 19 are digital interrupt pins)
//**ONLY BEST PERFORMANCE MODE IS CURRENTLY WORKING WITH THIS RELEASE (ERROR IN ENCODER LIBRARY)**
//enter the pin number if in use, else '99'
//encoders have dual functionality enabled by the shift button
int encoderPins[3][2] = {
  {99, 99}, //encoder 1
  {99, 99}, //encoder 2
  {99, 99}, //encoder 3
};

//.........................................................
//MULTIPLEXER READING_________________________________________
//CD4067BE - http://www.ti.com/lit/ds/symlink/cd4067b.pdf
//'1' for multiplexer I/O you want to send signal, else enter '0'

//MULTIPLEXER 1_______________________________________________
int toReadAnalogMux1[16] = {
  //IC pin number are written below
  1, 1, 1, 1, //0-3
  1, 1, 1, 1, //4-7
  1, 1, 1, 1, //8-11
  1, 1, 1, 1 //12-15
};

//MULTIPLEXER 2_______________________________________________
int toReadAnalogMux2[16] = {
  //IC pin number are written below
  0, 0, 0, 0, //0-3
  0, 0, 0, 0, //4-7
  0, 0, 0, 0, //8-11
  0, 0, 0, 0 //12-15
};

//MULTIPLEXER 3_______________________________________________
int toReadAnalogMux3[16] = {
  //IC pin number are written below
  0, 0, 0, 0, //0-3
  0, 0, 0, 0, //4-7
  0, 0, 0, 0, //8-11
  0, 0, 0, 0 //12-15
};

//MULTIPLEXER 4_______________________________________________
int toReadAnalogMux4[16] = {
  //IC pin number are written below
  0, 0, 0, 0, //0-3
  0, 0, 0, 0, //4-7
  0, 0, 0, 0, //8-11
  0, 0, 0, 0 //12-15
};

//MULTIPLEXER 5, DIGITAL FOR PUSHBUTTONS__________________________
int toReadDigitalMux5[16] = {
  //IC pin number are written below
  0, 0, 0, 0, //0-3
  0, 0, 0, 0, //4-7
  0, 0, 0, 0, //8-11
  0, 0, 0, 0 //12-15
};

//PUSHBUTTON MODE______________________________________
//there are a few different modes in which you may wish for your pushbutton to behave
//'1' - standard mode, when pushbutton is engaged note is turned on, when pushbutton is released, note is turned off
//'2' - on mode, note is only turned on with each click
//'3' - off mode, note is only turned off with each click
//'4' - toggle mode, note is switched between on and off with each click
int pushbuttonMode[32] = {
  //Pin number are written below
  4, 4, 0, 0, 0, //0-4
  0, 0, 0, 0, 0, //5-9
  0, 0, 0, 0, 0, //10-14
  0,             //16
  //When shift button is held, the following pushbuttons are enabled (these are NOT pin numbers)
  0, 0, 0, 0, 0, //17-21 SHIFT
  0, 0, 0, 0, 0, //22-26 SHIFT
  0, 0, 0, 0, 0, //27-31 SHIFT
  0              //32

};

//..................................................
//LED_______________________________________________
//For choosing which shift register channels is active
//'1' for pins which have LEDs hooked up to them, else '0'
int toDisplayLED[16] = {
  //Pin number are written below
  0, 0, 0, 0, 0, //0-4
  0, 0, 0, 0, 0, //5-9
  0, 0, 0, 0, 0, //10-14
  0              //15
};


//TEENSY ________________________________________________________
//directly from Teensy analog pins
//analog inputs require three pins: power, ground, and input
//enter '1' for analog inputs you want to read, else enter '0'
int toReadAnalogTeensy[10] = {
  //Pin number are written below
  0, 0, 0, 0, //14,15,16,17
  0, 0, 0, 0, //18,19,20,21
  0, 0     //22,23
};


// END EDIT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ------------------------------------------------------------------------------------------------------------------------------------




// VARIABLES AND FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//ENCODER_______________________________________________
Encoder *encoders[3];
boolean toReadEncoder[3];
long encPosition[12];
long tempEncPosition;

//MULTIPLEXER CONTROL SIGNAL_____________________________
int s0 = 0; //control pin A - only need one
int s1 = 1; //control pin B - only need one
int s2 = 2; //control pin C - only need one
int s3 = 3; //control pin D - only need one

int INH_pin = 13; //Inhibitor pin for multiplexers


//MULTIPLEXER ANALOG IN PINS____________________________
//(64 potetiometers in total)
int SIG_pin1 = 14; //analog mux read pin for mux1
int SIG_pin2 = 15; //analog mux read pin for mux2
int SIG_pin3 = 16; //analog mux read pin for mux3
int SIG_pin4 = 17; //analog mux read pin for mux4

//MULTIPLEXER DIGITAL IN SIGNAL PIN FOR BUTTONS________
//(16 buttons in total)
int SIG_pin5 = 11; //digital mux read pin for muxButtons

//SHIFT REGISTER PIN FOR LED___________
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

//ARRAY FOR HOW MANY USED SHIFT REGISTERS_______________
int numOfRegisters = 0;
byte* registerState;


//ANALOG IN MULTIPLEXER 1________________________________
int analogInsPrev1[16]; //array to hold previously read analog values - set all to zero for now
int tempAnalogInMux1 = 0; //array to hold previously read analog values
int tempAnalogInMap1 = 0;
int controlPin1[] = {s0, s1, s2, s3}; //set contol pins in array
//control array
int muxChannel1[16][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0}, 
{1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
{0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}};
//function to read mux
int readMux1(int channel1) {
  //loop through the four control pins
  for (int i = 0; i < 4; i ++) {
    //turn on/off the appropriate control pins according to what channel we are trying to read
    digitalWrite(controlPin1[i], muxChannel1[channel1][i]);
  }
  //read the value of the pin
  int val1 = analogRead(SIG_pin1);
  //return the value
  return val1;
}

//ANALOG IN MULTIPLEXER 2________________________________
int analogInsPrev2[16]; //array to hold previously read analog values - set all to zero for now
int tempAnalogInMux2 = 0; //array to hold previously read analog values
int tempAnalogInMap2 = 0;
int controlPin2[] = {s0, s1, s2, s3}; //set contol pins in array
//control array
int muxChannel2[16][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0},
{1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
{0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}};
//function to read mux
int readMux2(int channel2) {
  //loop through the four control pins
  for (int i = 0; i < 4; i ++) {
    //turn on/off the appropriate control pins according to what channel we are trying to read
    digitalWrite(controlPin2[i], muxChannel2[channel2][i]);
  }
  //read the value of the pin
  int val2 = analogRead(SIG_pin2);
  //return the value
  return val2;
}

//ANALOG IN MULTIPLEXER 3________________________________
int analogInsPrev3[16]; //array to hold previously read analog values - set all to zero for now
int tempAnalogInMux3 = 0; //array to hold previously read analog values
int tempAnalogInMap3 = 0;
int controlPin3[] = {s0, s1, s2, s3}; //set contol pins in array
//control array
int muxChannel3[16][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0}, 
{1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1}, 
{0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}};
//function to read mux
int readMux3(int channel3) {
  //loop through the four control pins
  for (int i = 0; i < 4; i ++) {
    //turn on/off the appropriate control pins according to what channel we are trying to read
    digitalWrite(controlPin3[i], muxChannel3[channel3][i]);
  }
  //read the value of the pin
  int val3 = analogRead(SIG_pin3);
  //return the value
  return val3;
}

//ANALOG IN MULTIPLEXER 4________________________________
int analogInsPrev4[16]; //array to hold previously read analog values - set all to zero for now
int tempAnalogInMux4 = 0; //array to hold previously read analog values
int tempAnalogInMap4 = 0;
int controlPin4[] = {s0, s1, s2, s3}; //set contol pins in array
//control array
int muxChannel4[16][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0},
{1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
{0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}};
//function to read mux
int readMux4(int channel4) {
  //loop through the four control pins
  for (int i = 0; i < 4; i ++) {
    //turn on/off the appropriate control pins according to what channel we are trying to read
    digitalWrite(controlPin4[i], muxChannel4[channel4][i]);
  }
  //read the value of the pin
  int val4 = analogRead(SIG_pin4);
  //return the value
  return val4;
}


//DIGITAL IN MULTIPLEXER 5 FOR PUSH BUTTONS___________________
long timeHit[32]; //16*2 = 32
int shiftChange;
bool buttonState[16]; //array with stored state: if the button was last turned on or off
bool tempDigitalRead5 = 0;//array to hold previously read analog values
int controlPin5[] = {s0, s1, s2, s3}; //set contol pins in array
//control array
int muxChannel5[16][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0}, 
{1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1}, 
{0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}};
//function to read mux
int readMux5(int channel5) {
  //loop through the four control pins
  for (int i = 0; i < 4; i ++) {
    //turn on/off the appropriate control pins according to what channel we are trying to read
    digitalWrite(controlPin5[i], muxChannel5[channel5][i]);
  }
  //read the value of the pin
  int val5 = digitalRead(SIG_pin5);
  //return the value
  return val5;
}









//ANALOG IN ON BOARD_______________________________________________

//  If using on board analog pins
int analogPinsTeensy[10] = {14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
int analogInsPrevTeensy[10]; //array to hold previously read analog values
int tempAnalogInTeensy = 0;
int tempAnalogInMapTeensy = 0;


// ======================================================================================
// SETUP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup() {

    //AREF_______________________________________________
    // Voltage reference for analog input, MUST BE BEFORE analogRead! 
  analogReference(EXTERNAL);


    //DISPLAY LAUNCH_____________________________________
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);



  //DEBUG_______________________________________________
  if (enableDebug == 1) {
    Serial.begin(9600);//open serail port @ debug speed
    Serial.flush();
    Serial.println();
    Serial.println("___DEBUG ENABLED___");
    Serial.println();
  } else {
    Serial.begin(31250);//open serail port @ midi speed
  }

  //CHECK FOR CONFLICTING INPUTS_______________________________________________
  if (enableDebug) {
    Serial.println("~~~~~~~~~~~~~~ CHECKING CONFLICTS ~~~~~~~~~~~~~~");

    //pushbutton vs encoder
    //LED vs encoder
    //pushbutton vs LED
    //teensy analog vs pushbutton
    //teensy analog vs LED
    //teensy analog vs encoder

    //pushbutton vs encoder
    for (int p = 0; p < 16; p++) { //loop through all pushbuttons
      for (int e = 0; e < 3; e++) { //loop through all encoders
        if ( (encoderPins[e][0] == p && toReadDigitalMux5[p] == 1) || (encoderPins[e][1] == p && toReadDigitalMux5[p] == 1) ) {
          Serial.println("ERROR - pin [" + (String)p + "] enabled in both pushbutton and encoder");
        }
      }
    }
    //LED vs encoder
    for (int d = 0; d < 16; d++) { //loop through all LEDs
      for (int e = 0; e < 3; e++) { //loop through all encoders
        if ( (encoderPins[e][0] == d && toDisplayLED[d] == 1) || (encoderPins[e][1] == d && toDisplayLED[d] == 1) ) {
          Serial.println("ERROR - pin [" + (String)d + "] enabled in both encoder and LED");
        }
      }
    }
    //pushbutton vs LED
    for (int i = 0; i < 16; i++) { //loop through all pins
      if (toReadDigitalMux5[i] == 1 && toDisplayLED[i] == 1) {
        Serial.println("ERROR - pin [" + (String)i + "] enabled in both pushbutton and LED");
      }
    } 
  }

  if (enableDebug) {
    Serial.println("~~~~~~~~~~~~~~ PIN CONFIG ~~~~~~~~~~~~~~");
  }

  //SHIFT - pin config _______________________________________________
  //we need enable the shift pin as an INPUT as well as turn on the pullup resistor
  if (shiftPin != 0) {
    pinMode(shiftPin, INPUT_PULLUP); //shift button
    if (enableDebug) {
      Serial.println("SHIFT button is enabled on pin [" + (String)shiftPin + "]");
    }
  }

  //ENCODER - pin config _______________________________________________
  for (int i = 0; i < 3; i++) {
    if (encoderPins[i][0] != 99 && encoderPins[i][1] != 99) {
      encoders[i] = new Encoder(encoderPins[i][0], encoderPins[i][1]);
      toReadEncoder[i] = true;
      if (enableDebug) {
        Serial.println("Encoder on pins [" + (String)encoderPins[i][0] + "," + (String)encoderPins[i][1] + "] is enabled");
      }
    }
    else {
      toReadEncoder[i] = false;
    }
  }

  //ANALOG IN - pin config _______________________________________________
  //MULTIPLEXER 1_______________________________________________
  for (int i = 0; i < 16; i++) {
    if (toReadAnalogMux1[i] == 1 && enableDebug == 1) {
      Serial.println("Analog in on multiplexer pin [" + (String)i + "] is enabled");
    }
  }
  //MULTIPLEXER 2_______________________________________________
  for (int i = 0; i < 16; i++) {
    if (toReadAnalogMux2[i] == 1 && enableDebug == 1) {
      Serial.println("Analog in on multiplexer pin [" + (String)i + "] is enabled");
    }
  }
  //MULTIPLEXER 3_______________________________________________
  for (int i = 0; i < 16; i++) {
    if (toReadAnalogMux3[i] == 1 && enableDebug == 1) {
      Serial.println("Analog in on multiplexer pin [" + (String)i + "] is enabled");
    }
  }
  //MULTIPLEXER 4_______________________________________________
  for (int i = 0; i < 16; i++) {
    if (toReadAnalogMux4[i] == 1 && enableDebug == 1) {
      Serial.println("Analog in on multiplexer pin [" + (String)i + "] is enabled");
    }
  }

  //MULTIPLEXER 5 - pin config _______________________________________________
  //we need enable each pushbutton pin as an INPUT as well as turn on the pullup resistor
  for (int i = 0; i < 16; i++) {
    if (toReadDigitalMux5[i] == 1 && enableDebug == 1) {
      pinMode(i, INPUT_PULLUP); //pushbutton pullup
      if (enableDebug) {
        Serial.println("Pushbutton on pin [" + (String)i + "] is enabled with pushbutton mode [" + (String)pushbuttonMode[i] + "]");
      }
    }
  }

   
   //SHIFT REGISTER - pin config _______________________________________________
   //Initialize array for how many used shift registers
  registerState = new byte[numOfRegisters];
  for (size_t i = 0; i < numOfRegisters; i++) {
    registerState[i] = 0;
  }
  

  //TEENSY ONBOARD - pin config _______________________________________________

  for (int i = 0; i < 10; i++) {
    if (toReadAnalogTeensy[i] == 1 && enableDebug) {
      int p = i + 14;
      Serial.println("Analog in on teensy pin [" + (String)p + "] is enabled");
    }
  }
  
  //MULTIPLEXER SETUP _______________________________________________

  //set multiplexers reading pin
  pinMode(SIG_pin1, INPUT);
  pinMode(SIG_pin2, INPUT);
  pinMode(SIG_pin3, INPUT);
  pinMode(SIG_pin4, INPUT);
  pinMode(SIG_pin5, INPUT); 

  //set inhibit pin to output and turn off
  pinMode(INH_pin, OUTPUT);
  digitalWrite(INH_pin, LOW); 
  
  //set our control pins to output
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  
  //turn all control pins off (for now)
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  //SHIFT REGISTER SETUP _______________________________________________

  //set shift register pins to output 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);


  //DEBUG
  if (enableDebug) {
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("PROGRAM WILL START LOOPING IN THREE SECONDS...");
    delay(3000); //wait three seconds
  }
}



// ======================================================================================
// LOOPS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {


  //SHIFT loop _______________________________________________
  if (shiftPin != 0) {
    if (readMux5(shiftPin) == LOW) { //check if shift button was engaged
      shiftChange = 16;  //if enganged, the offset is 16
    } else {
      shiftChange = 0;
    }
  }


  //ANALOG IN MUX loop 1_______________________________________________
  for (int i = 0; i < 16; i++) { //loop through 16 mux channels
    if (toReadAnalogMux1[i] == 1) { //we read this mux channel analog input
      tempAnalogInMux1 = readMux1(i); //ready valued using readMux function
      if (abs(analogInsPrev1[i] - tempAnalogInMux1) > analogThreshold) { //ensure value changed more than our threshold
        tempAnalogInMap1 = map(tempAnalogInMux1, 0, 1023, 0, 127); //remap value between 0 and 127
        midiSend('a', tempAnalogInMap1, i); //send message
        analogInsPrev1[i] = tempAnalogInMux1; //update current value
      }
    }
  }

  //ANALOG IN MUX loop 2_______________________________________________
  for (int i = 0; i < 16; i++) { //loop through 16 mux channels
    if (toReadAnalogMux2[i] == 1) { //we read this mux channel analog input
      tempAnalogInMux2 = readMux2(i); //ready valued using readMux function
      if (abs(analogInsPrev2[i] - tempAnalogInMux2) > analogThreshold) { //ensure value changed more than our threshold
        tempAnalogInMap2 = map(tempAnalogInMux2, 0, 1023, 0, 127); //remap value between 0 and 127
        midiSend('a', tempAnalogInMap2, i+16); //send message
        analogInsPrev2[i] = tempAnalogInMux2; //update current value
      }
    }
  }

  //ANALOG IN MUX loop 3_______________________________________________
  for (int i = 0; i < 16; i++) { //loop through 16 mux channels
    if (toReadAnalogMux3[i] == 1) { //we read this mux channel analog input
      tempAnalogInMux3 = readMux3(i); //ready valued using readMux function
      if (abs(analogInsPrev3[i] - tempAnalogInMux3) > analogThreshold) { //ensure value changed more than our threshold
        tempAnalogInMap3 = map(tempAnalogInMux3, 0, 1023, 0, 127); //remap value between 0 and 127
        midiSend('a', tempAnalogInMap3, i+32); //send message
        analogInsPrev3[i] = tempAnalogInMux3; //update current value
      }
    }
  }

  //ANALOG IN MUX loop 4_______________________________________________
  for (int i = 0; i < 16; i++) { //loop through 16 mux channels
    if (toReadAnalogMux4[i] == 1) { //we read this mux channel analog input
      tempAnalogInMux4 = readMux4(i); //ready valued using readMux function
      if (abs(analogInsPrev4[i] - tempAnalogInMux4) > analogThreshold) { //ensure value changed more than our threshold
        tempAnalogInMap4 = map(tempAnalogInMux4, 0, 1023, 0, 127); //remap value between 0 and 127
        midiSend('a', tempAnalogInMap4, i+48); //send message
        analogInsPrev4[i] = tempAnalogInMux4; //update current value
      }
    }
  }

  //DIGITAL IN MUX 5 BUTTONS_______________________________________________ 
  for (int i = 0; i < 16; i++) { //loop through 16 mux channels
    if (i != shiftPin) { //ensure this is not the shift pin
      int j = i + shiftChange; //add the shift change (+16)
      if (toReadDigitalMux5[i] == 1) { //we read the choosen mux channel input
        tempDigitalRead5 = readMux5(i);
        if (pushbuttonMode[j] == 1 && tempDigitalRead5 != buttonState[j]) { //___NORMAL MODE (1)
          delay(2); //just a delay for noise to ensure push button was actually hit
          if (readMux5(i) == tempDigitalRead5) { //check if pushbutton is still the same
            if (tempDigitalRead5 == LOW) { //button pressed, turn note on
              midiSend('p', 1, j); //call note on/off function
            } else { //button released
              midiSend('p', 0, j);
            }
            buttonState[j] = tempDigitalRead5; //update the state (on or off)
          }
        } else { //___ALL OTHER MODES (2,3,4)
          if (readMux5(i) == LOW && (millis() - timeHit[j]) > pbBounce) { //check bounce time
            if (pushbuttonMode[j] == 2) { //mode 2 - only note on
              midiSend('p', 1, j);
            } else if (pushbuttonMode[j] == 3) { //mode 3 - only note off
              midiSend('p', 0, j);
            } else { //mode 4 - toggle
              if (buttonState[j] == 1) { //on->off
                midiSend('p', 0, j);
                buttonState[j] = 0;
              } else { //off->on
                midiSend('p', 1, j);
                buttonState[j] = 1;
              }
            }
            timeHit[j] = millis();
          }
        }
      }
    }
  }

 
 // //DIGITAL OUT MUX 6 LEDS_______________________________________________
//  for (int i = 0; i < 16; i++) {  //loop through 16 mux channels
//    if (buttonState[i] == 0) {    //we read the buttonState to send signal to LED when button is pressed
//    writeMux6(i);   //turn on the LED, going thru multiplexer 6
//  }
//  }







  //ANALOG IN TEENSY loop _______________________________________________
  for (int i = 0; i < 10; i++) { //loop through the 10 analog teensy channels
    if (toReadAnalogTeensy[i] == 1) { //we read this analog input
      tempAnalogInTeensy = analogRead(analogPinsTeensy[i]);
      if (abs(analogInsPrevTeensy[i] - tempAnalogInTeensy) > analogThreshold) { //ensure value changed more than our threshold
        tempAnalogInMapTeensy = map(tempAnalogInTeensy, 0, 1023, 0, 127); //remap value between 0 and 127
        midiSend('a', tempAnalogInMapTeensy, i + 16); //send message
        analogInsPrevTeensy[i] = tempAnalogInTeensy; //update current value
      }
    }
  }

//ENCODER loop _______________________________________________
  for (int i = 0; i < 3; i++) { //loop through all encoders
    if (toReadEncoder[i] == true) { //check if we should read this encoder
      tempEncPosition = encoders[i]->read(); //get encoder position
      if (shiftChange == 26) { //shift button is engaged
        i = i + 3; //encoders 6-11
      } else {
        //encoders 0-5
      }
      if (tempEncPosition > encPosition[i]) { //this position is greater than the last
        midiSend('e', 1, i); //send message
        encPosition[i] = tempEncPosition; //update position
      } else if (tempEncPosition < encPosition[i]) { //this position is less than the last
        midiSend('e', 0, i); //send message
        encPosition[i] = tempEncPosition; //update position
      } else {
        //do nothing
      }
    }
  }

  
  //----------------------------------------------------------------------------
  //              LOOP  -  DISPLAY
  //----------------------------------------------------------------------------

  //DISPLAY____________________________________
 {
  //Display text
  display.setCursor(0, 0);
  display.print("CC number ~ "); // Change to CC number?
  display.println(global_pin);  //Put CCnumber here

  display.setCursor(0, 12);
  display.print("says hi at");

  display.setCursor(0, 24);
  display.print("MIDI channel ~ ");
  display.println(channelNumber); // Put midichannel here

  display.setCursor(0, 36);
  display.print("n tickling");
  display.setCursor(0, 48);
  display.print("VALUE number ~");
  display.setCursor(91, 48);
  display.println(global_val); // to vary 0 to end result, ie 1024/?  //Put CC value here

  //Display parameter bar
  display.fillRect(0, 63, global_val, 1, 1); // 2=V from left; 61=H from top; val/8= lenght of bar ie,
  //val/16 is half screen with full value; 3= thickness of slide bar; 1= ? colour or brightness
  display.display();
  display.clearDisplay();  // Clear display must be used to clear text etc
 }
}    // LOOP END









// ........................................................................................................
// COMMUNICATION FUNCTIONS ________________________________________________________________________________
// http://www.pjrc.com/teensy/td_midi.html

//debug out
void serialDebugOut(String cType, int cNum, String cVal) {
  Serial.print(cType);
  Serial.print(" ");
  Serial.print(cNum);
  Serial.print(": ");
  Serial.println(cVal);
}


//function to send MIDI
void midiSend(char type, int val, int pin) {
  String clickState;
  switch (type) {

    case 'p': //--------------- PUSHBUTTON
      if (enableDebug) {
        if (val == 1) {
          clickState = "click on";
        } else {
          clickState = "click off";
        }
        serialDebugOut("Pushbutton", pin, clickState);
      }
      else {
        if (val == 1) {
          usbMIDI.sendNoteOn(pin, 127, channelNumber); //!!!
        } else {
          usbMIDI.sendNoteOff(pin, 127, channelNumber); //!!!
        }
      }
      break;

    case 'e': //--------------- ENCODER
      if (enableDebug) {
        if (val == 1) {
          clickState = "forward";
        } else {
          clickState = "reverse";
        }
        serialDebugOut("Encoder", pin, clickState);
      }
      else {
        if (val == 1) {
          //fuzzy
          //usbMIDI.sendNoteOn(pin+26,127,channelNumber); //!!! //we have 26 used digitals, thus must add 26
          //ableton
          //usbMIDI.sendControlChange(pin+23,68,channelNumber); //!!! //we have 23 used analogs, thus must add 23
          //other
          //usbMIDI.sendControlChange(pin+23,65,channelNumber); //!!! //we have 23 used analogs, thus must add 23

        }
        else {
          //fuzzy
          //usbMIDI.sendNoteOff(pin+26,127,channelNumber); //!!! //we have 26 used digitals, thus must add 26
          //ableton
          //usbMIDI.sendControlChange(pin+23,60,channelNumber); //!!! //we have 23 used analogs, thus must add 23
          //other
          //usbMIDI.sendControlChange(pin+23,63,channelNumber); //!!! //we have 23 used analogs, thus must add 23
        }
      }
      break;

    case 'a': //--------------- ANALOG
      if (enableDebug) {
        if (pin > 15) {
          serialDebugOut("Analog Teensy", pin, val);
        } else {
          serialDebugOut("Analog mux", pin, val);
        }
      } else {
        usbMIDI.sendControlChange(pin, val, channelNumber); //!!!
        global_val=val;
        global_pin=pin;
      }
      break;


  }
}

