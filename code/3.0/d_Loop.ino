


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



  //READ MIDI CALLBACK_______________________________________________
  usbMIDI.read(); 



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



  //DIGITAL MULTIPLEXER 1_______________________________________________
  for (int i = 0; i < 16; i++) { //loop through 16 mux channels
    if (toReadDigitalMux1[i] == 1) { //we read the choosen mux channel input
      tempDigitalRead1 = digitalReadMux1(i);
      if (pushbuttonMode[i] == 1 && tempDigitalRead1 != buttonState[i]) { //___NORMAL MODE (1)
        delay(20); //just a delay for noise to ensure push button was actually hit
        if (digitalReadMux1(i) == tempDigitalRead1) { //check if pushbutton is still the same
          if (tempDigitalRead1 == LOW) { //button pressed, turn note on
            midiSend('p', 1, i); //call note on/off function
          } else { //button released
            midiSend('p', 0, i);
          }
          buttonState[i] = tempDigitalRead1; //update the state (on or off)
        }
      } else { //___ALL OTHER MODES (2,3,4)
        if (digitalReadMux1(i) == LOW && (millis() - timeHit[i]) > pbBounce) { //check bounce time
          if (pushbuttonMode[i] == 2) { //mode 2 - only note on
            midiSend('p', 1, i);
          } else if (pushbuttonMode[i] == 3) { //mode 3 - only note off
            midiSend('p', 0, i);
          } else { //mode 4 - toggle
            if (buttonState[i] == 1) { //on->off
              midiSend('p', 0, i);
              buttonState[i] = 0;
            } else { //off->on
              midiSend('p', 1, i);
              buttonState[i] = 1;
            }
          }
          timeHit[i] = millis();
        }
      }
    }
  }




  //DIGITAL OUT SHIFT REGISTER FOR LEDS_______________________
    for (byte i = numOfBytes; i > 0; i--) { //get bytes from array
      for (byte j = 8; j > 0; j--) {    //send information
        if (pushbuttonMode[i] == 2) {
          digitalWrite(dataPin, buttonState[((i - 1) * 8) + (j - 1)]);      // mode 1
          digitalWrite(clockPin, HIGH);  // clock the output into the 595
          digitalWrite(clockPin, LOW);

        } else {
          digitalWrite(dataPin, toggleState[((i - 1) * 8) + (j - 1)]);      // mode 4
          digitalWrite(clockPin, HIGH);  // clock the output into the 595
          digitalWrite(clockPin, LOW);
        }
        digitalWrite(latchPin, 1);
        digitalWrite(latchPin, 0);
      }
    }
  



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
