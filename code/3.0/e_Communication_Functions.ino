


  //Function to send MIDI
  //http://www.pjrc.com/teensy/td_midi.html
  
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
        delay(500);

    }
  } 



  void noteOn(byte channel, byte note, byte velocity) {
    
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, note);
        digitalWrite(latchPin, HIGH);  
  }

  void noteOff(byte channel, byte note, byte velocity) {
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 0x00);
        digitalWrite(latchPin, HIGH);
  }

