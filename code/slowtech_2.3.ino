// This project is based on the Teensy Monster 1.0: https://github.com/FuzzyWobble/TeensyMonster
// License: http://opensource.org/licenses/MIT
// Edit: Imbecillen / Sebastian Galo 2018

// Use name.c as tab in the sketch to show custom midi name.
// Complete code for the project with 128x64px oled display, 16 led-buttons and 64 potentiometers.
// Updated slowtech_2.0 to work with the soldered PCB layout. 
// Updated I/O on Teensy and names on the digital VS analog multiplexers for better structure.

// Merged with branches:
// 2.1 potentiometers
// 2.2 buttons toggle release
// 2.2 buttons leds

// ======================================================================================
// LIBRARIES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <SPI.h>  // SPI library
#include <Wire.h> // I2C library
#include <Adafruit_GFX.h> // Display graphics library
#include <Adafruit_SSD1306.h> // Display library

#define  OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);






// ======================================================================================
// SETTINGS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//....................................................
//CHANNEL_____________________________________________
int channelNumber = 1; //each controller should have a unique channel number between 1 and 10



//POTENTIOMETER THRESHOLD
//you can increase this if you have some jitter problems on your analog devices
//it is best to solve your jitter problems according to the Instructable
int analogThreshold = 5;



//....................................................
//PIN+VAL TO DISPLAY__________________________________
int global_val, global_pin;



//...................................................
//DEBOUNCE BUTTONS___________________________________
//debounce is a measurement of the time in which a pushbutton is unresponsive after it is pressed
//this is important to prevent unwanted double clicks
uint32_t pbBounce = 150; //150 millisecond debounce duration - you may want to change this value depending on the mechanics of your pushbuttons



//.........................................................
//MULTIPLEXER READING_________________________________________
//CD4067BE - http://www.ti.com/lit/ds/symlink/cd4067b.pdf
//'1' for multiplexer I/O you want to send signal, else enter '0'

//ANALOG MULTIPLEXER 1_______________________________________________
int toReadAnalogMux1[16] = {
  //IC pin number are written below
  1, 1, 1, 1, //0-3
  1, 1, 1, 1, //4-7
  1, 1, 1, 1, //8-11
  1, 1, 1, 1 //12-15
};



//DIGITAL MULTIPLEXER 1, DIGITAL FOR PUSHBUTTONS______________
int toReadDigitalMux1[16] = {
  //IC pin number are written below
  1, 1, 1, 1, //0-3
  1, 1, 1, 1, //4-7
  1, 1, 1, 1, //8-11
  1, 1, 1, 1 //12-15
};

//PUSHBUTTON MODE______________________________________
//there are a few different modes in which you may wish for your pushbutton to behave
//'1' - standard mode, when pushbutton is engaged note is turned on, when pushbutton is released, note is turned off
//'2' - on mode, note is only turned on with each click
//'3' - off mode, note is only turned off with each click
//'4' - toggle mode, note is switched between on and off with each click
int pushbuttonMode1[16] = {
  //Pin number are written below
  4, 4, 4, 4, //0-3
  4, 4, 4, 4, //4-7
  4, 4, 4, 4, //8-11
  4, 4, 4, 4 //12-15 (NOTE: comma after last row/number for shift function to continue down under)
};



//LED__________________________________________________
int toDisplayLed[16] = {
  //IC pin number are written below
  //'1' for shift register pin you want to send signal to, else enter '0'
  1, 1, 1, 1, //0-3
  1, 1, 1, 1, //4-7
  1, 1, 1, 1, //8-11
  1, 1, 1, 1 //12-15
};


// ======================================================================================
// VARIABLES AND FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// TEENSY LED - blinks when alive yo! -
const int led = 13;  // Pin 13 has the LED on Teensy LC and 3.0
int ledState = LOW;             // ledState used to set the LED
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
// will store last time LED was updated
unsigned long previousMillis = 0;
// Constants won't change
// interval at which to blink (milliseconds)
const long interval = 800;



//MULTIPLEXER CONTROL SIGNAL PINS_______________________
int s0 = 1; //control pin A
int s1 = 2; //control pin B
int s2 = 3; //control pin C
int s3 = 4; //control pin D



//MULTUPLEXER INHIBITOR PINS_____________________________
int inhibitAnalogMux1 = 5;
int inhibitDigitalMux1 = 9;



//MULTIPLEXER INPUT PINS ________________________________
//analog in, signal pin for buttons
int analogSignalMux1 = 15; //analog mux read pin for mux1

//analog in, signal pin for buttons
int digitalSignalMux1 = 0; //digital mux read pin for muxButtons


//SHIFT REGISTER FOR LED OUTPUT PINS_____________________
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
  int val1 = analogRead(analogSignalMux1);
  //return the value
  return val1;
}



//DIGITAL IN MULTIPLEXER 1________________________________
unsigned long timeHit[16]; //16*2 = 32 for use with shift button code
boolean buttonState[16]; //array with stored state
boolean toggleState[16]; //array with stored state
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
    //turn on/off the appropriate control pins according to what channel we are trying to read
    digitalWrite(digitalControlPin1[i], digitalMuxChannel1[digitalChannel1][i]);
  }
  //read the value of the pin
  int val = digitalRead(digitalSignalMux1);
  //return the value
  return val;
}






// ======================================================================================
// SETUP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup() {

  Serial.begin(9600);



  //AREF_______________________________________________
  // Voltage reference for analog input, MUST BE BEFORE analogRead!
  analogReference(EXTERNAL);



  //SPI PINS__________________________________________________________
  SPI.setMOSI(11);  // DS, MOSI (data out from Teensy, into 595)
  SPI.setSCK(14);   // SH_CP, SCK (Shift register clock pin) Set to alternative pin 14 instead of 13!!
  SPI.begin();



  //DISPLAY LAUNCH_____________________________________
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);



  //MULTIPLEXER SETUP _________________________________
  //set multiplexers reading pin
  pinMode(analogSignalMux1, INPUT);
  pinMode(digitalSignalMux1, INPUT_PULLUP);


  //set inhibit pin to output and turn low
  pinMode(inhibitDigitalMux1, OUTPUT);
  digitalWrite(inhibitDigitalMux1, LOW);

  pinMode(inhibitAnalogMux1, OUTPUT);
  digitalWrite(inhibitAnalogMux1, LOW);

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



  //SHIFT REGISTER SETUP___________________________________
  //set shift register pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);



  // BLINK
  // initialize the digital pin as an output for Teensy LED
  pinMode(led, OUTPUT);

} // SETUP END






// ======================================================================================
// LOOPS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {


  // TEENSY LED BLINKS WHEN PROCESSING SIGNAL
  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(led, ledState);
  }



  //ANALOG MULTIPLEXER 1_______________________________________________
  for (int i = 0; i < 16; i++) { //loop through 16 mux channels
    if (toReadAnalogMux1[i] == 1) { //we read this mux channel analog input
      tempAnalogInMux1 = analogReadMux1(i); //ready valued using readMux function
      if (abs(analogInsPrev1[i] - tempAnalogInMux1) > analogThreshold) { //ensure value changed more than our threshold
        tempAnalogInMap1 = map(tempAnalogInMux1, 0, 1023, 0, 127); //remap value between 0 and 127
        midiSend('a', tempAnalogInMap1, i); //send message
        analogInsPrev1[i] = tempAnalogInMux1; //update current value
      }
    }
  }


  //DIGITAL MULTIPLEXER 1 ________________________________
for (int i = 0; i < 16; i++)   //loop through 16 mux channels
  {
    if (toReadDigitalMux1[i] == 1)   //we read the choosen mux channel input
    {
      tempDigitalRead1 = digitalReadMux1(i);


      // Look for a state change and debounce
      if (tempDigitalRead1 != buttonState[i]  && millis() - timeHit[i] > 20UL)
      {
        buttonState[i] = tempDigitalRead1; //update the state (LOW=PRESSED, HIGH=RELEASED)
        timeHit[i] = millis();


        if (pushbuttonMode1[i] == 1)   //___NORMAL MODE (1)
        {
          midiSend('p', tempDigitalRead1 == LOW, i); // note on/off when button is PRESSED/RELEASED
        }
        else if (pushbuttonMode1[i] == 2)   //mode 2 - only note on
        {
          if (tempDigitalRead1 == LOW)  // Send note on when button is PRESSED (Active LOW)
            midiSend('p', 1, i);
        }
        else if (pushbuttonMode1[i] == 3)     //mode 3 - only note off
        {
          if (tempDigitalRead1 == LOW)  // Send note off when button is PRESSED (Active LOW)
            midiSend('p', 0, i);
        }
        else     //mode 4 - toggle
        {
          if (tempDigitalRead1 == HIGH)   // Toggle on button RELEASE  (Active LOW)
          { 
            toggleState[i] = !toggleState[i];
            midiSend('p', toggleState[i], i);
            delay (50);
              Serial.print("pin   = ");
              Serial.println(i);
              Serial.print("state = ");
              Serial.println(toggleState[i]);
              Serial.println("");

          }
        }
      }
    }
  }



  // //DIGITAL OUT SHIFT REGISTER FOR LEDS_______________________
  for (int i = 0; i < 16; i++) { //loop buttons
    //if (toDisplayLed[i] == 1) { //we display the choosen LED
    int data = pgm_read_byte (&buttonState[i]); //load the button state from the array, make unsigned int from boolean
    if (digitalReadMux1(i) == tempDigitalRead1) { //check if pushbutton is still the same
      //ground latchPin and hold low for as long as you are transmitting
      digitalWrite(latchPin, 0);
      //send information
      shiftOut(dataPin, clockPin, LSBFIRST, data);
      //Serial.println("Data - ");
      //Serial.println(data);

      byte j; // Loop shift register
      for (j = 0; j < 16; j++) {
        digitalWrite(clockPin, LOW);
        if (tempDigitalRead1 == LOW) { //if button pressed > change led on/off
          if (buttonState[i] == 1) {
            digitalWrite(dataPin, (data >> j & 1));

            // Serial.print("Pins: ");
            // Serial.println(i);
            // Serial.println("");
            // Serial.print("Data: ");
            // Serial.println(data);
          }
          digitalWrite(clockPin, HIGH);
        }
        //return the latch pin to high to signal chip that it's no longer need to listen for information
        digitalWrite(latchPin, 1);
      }
    }
    // }
  }



  // Problemet ligger troligen i konvertering av bytes och bits från knapptryckningarna som är booleans? Osäker på detta.
  // Koden skriver bara 1/0 per loop till hela shift register, istället för att skicka 8st eller 16st 1/0 samtidigt vid shiftout.
  // Detta gör att alla leds tänds/släcks vid en knapptryckning om jag förstått det rätt.

  // Endast knapp 16 ger utslag på leds.
  // Ska for looparna vara 8 eller 16?
  // Målsättning är att det ska gå att justera antal shift register likt antal knappar senare. Se länken:
  // https://www.instructables.com/id/Arduino-16-LEDs-using-two-74HC595-shift-registers-/

  // Lösning:
  // Behöver göra byte eller int paket av tempDigitalRead5 (buttonstate?) för att kunna skicka alla på en gång.
  // Klarar shiftout mer än 8 bits åt gången eller får man skicka som fler paket?
  // Har jag förstått funktionen rätt skjuts extra bits över till nästa SR automatiskt.
  //
  // Serial monnitor resultat:
  // tempDigitalRead5 ger 1 eller 0 på alla pins samtidigt.
  // buttonState ger 1 eller 0 på motsvarande knapp.
  // data ger ?



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





// ======================================================================================
// COMMUNICATION FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// http://www.pjrc.com/teensy/td_midi.html



//function to send MIDI
void midiSend(char type, int val, int pin) {
  String clickState;
  switch (type) {

    case 'p': //--------------- PUSHBUTTON
      if (val == 1) {
        usbMIDI.sendNoteOn(pin, 127, channelNumber); //!!!
      } else {
        usbMIDI.sendNoteOff(pin, 127, channelNumber); //!!!
      }
      break;

    case 'a': //--------------- POTENTIOMETERS
      usbMIDI.sendControlChange(pin, val, channelNumber); //!!!
      global_val = val;
      global_pin = pin;
      break;


  }
} // VOID END


