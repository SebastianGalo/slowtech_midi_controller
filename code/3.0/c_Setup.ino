


void setup() {

  //SERIAL MONITOR________________________________________
  //Baud rate, what to use?
  Serial.begin(115200);



  //AREF_______________________________________________
  //voltage reference for analog input, MUST BE BEFORE analogRead in the loop!
  analogReference(EXTERNAL);



  //SHIFT REGISTER BUTTONS, OUTPUT PINS________________________________
  SPI.setMOSI(11);  // DS, MOSI (data out from Teensy, into 595)
  SPI.setSCK(14);   // SH_CP, SCK (Shift register clock pin) Set to alternative pin 14, instead of 13 which TEENSY LED uses
  SPI.begin();      



  //DISPLAY LAUNCH_____________________________________
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);



  //MULTIPLEXER READING SIGNAL, INPUT PINS________________________________
  //reading pins for signal from multiplexers
  pinMode(analogSignalMux1, INPUT);
  pinMode(digitalSignalMux1, INPUT); //INPUT_PULLUP used before external pullup resistors was installed



  //MULTIPLEXER CONTROL SIGNAL, OUTPUT PINS_______________________
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  //turn all control pins off (for now)
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);



   //MULTIPLEXER INHIBITOR, OUTPUT PINS_____________________________
  pinMode(inhibitAnalogMux1, OUTPUT);
  pinMode(inhibitDigitalMux1, OUTPUT);

  //set inhibit pin to output and turn low to keep them alive, if else set to HIGH (?)
  digitalWrite(inhibitAnalogMux1, LOW);
  digitalWrite(inhibitDigitalMux1, LOW);



  //SHIFT REGISTER FOR PUSHBUTTON LEDS, OUTPUT PINS_____________________
  //set shift register pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);


  //SET HANDLES FOR VOIDS, readUSB.MIDI
  usbMIDI.setHandleNoteOn(noteOn);
  usbMIDI.setHandleNoteOff(noteOff);



  // TEENSY LED________________________________________
  // initialize the digital pin as an output for Teensy LED
  pinMode(led, OUTPUT);


} // SETUP END

