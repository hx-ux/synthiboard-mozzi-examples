#include <Arduino.h>
/*
    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or
    check the README or http://sensorium.github.com/Mozzi/

		Mozzi documentation/API
		https://sensorium.github.io/Mozzi/doc/html/index.html

		Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Tim Barrass 2012, CC by-nc-sa.
*/

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/cos8192_int8.h>
#include <mozzi_midi.h>

#define CONTROL_RATE 128

// harmonics
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos1(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos2(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos3(COS8192_DATA);

///knobs
const char INPUT_PIN0 = A0; // set the input for the knob to analog pin 0
const char INPUT_PIN1 = A1; // set the input for the knob to analog pin 0
const char INPUT_PIN2 = A2; // set the input for the knob to analog pin 0
// audio volumes updated each control interrupt and reused in audio till next control
const int BUTTON_PIN = 4;  
char button_state;
int freq1=261;
//pitch selected by the knobs
byte volume0;
byte volume1;
byte volume2;

void setup()
{  
// set volume change frequencies
  startMozzi(CONTROL_RATE);
}

void updateControl()
{
  int knob_value0 = mozziAnalogRead(INPUT_PIN0);
  int knob_value1 = mozziAnalogRead(INPUT_PIN1);
  int knob_value2 = mozziAnalogRead(INPUT_PIN2);

  button_state = digitalRead(BUTTON_PIN);
  
   // C-Major Accord
  aCos1.setFreq(261);    //C4
  aCos2.setFreq(329);    //E4
  aCos3.setFreq(392);    //G4

  volume0 = map(knob_value0, 0, 1023, 0, 255);
  volume1 = map(knob_value1, 0, 1023, 0, 255);
  volume2 = map(knob_value2, 0, 1023, 0, 255);
 
}

int updateAudio()
{

long asig=(long)
  ((int)aCos1.next() * volume0) +
  ((int)aCos2.next() * volume1) +
  ((int)aCos3.next() * volume2) ;  
  asig >>=9;
  return(int) asig;
}

void loop()
{
  audioHook();
}