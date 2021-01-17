/*  Example of a simple button keyboard , no adsr, looped note

		Mozzi documentation/API
		https://sensorium.github.io/Mozzi/doc/html/index.html

		Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Tim Barrass 2013-14, CC by-nc-sa.
*/


#include <MozziGuts.h>
#include <Oscil.h>
#include <Smooth.h>
#include <AutoMap.h>
#include <ADSR.h>
#include <mozzi_midi.h>
#include <tables/sin2048_int8.h> // sine table for oscillator
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

//midi reference values
//c4 60
//d4 62
//e4 64
//f4 65
//g4 67
//a4 69
//b4 71
//c5 72

int pin_4;
int pin_5;
int pin_6;
int pin_7;
int pin_8;

byte gain = 255;
int pitch=60;//D4

// FUNCTIONS

void readPins() 
{
  pin_4 = digitalRead(4);
  pin_5 = digitalRead(5);
  pin_6 = digitalRead(6);
  pin_7 = digitalRead(7);
  pin_8 = digitalRead(8);

  if (pin_4 == LOW or pin_5 == LOW or pin_6 == LOW or pin_7 == LOW or pin_8 == LOW) 
  {  
  digitalWrite(13,HIGH);
  }
  else
  {    
  digitalWrite(13,LOW); 
  }

  if (pin_4 == LOW)//D4
  {
  pitch=62;
  }
  if (pin_5 == LOW)//E3
  {
  pitch=64;
  }
  if (pin_6 == LOW)//F3
  {
  pitch=65;
  }
  if (pin_6 == LOW)//G3
  {
  pitch=67;
  }
  if (pin_7 == LOW)//a3
  {
  pitch=69;
  }
  if (pin_8 == LOW)//b3
  {
  pitch=71;
  }
}


void setup() 
{
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  startMozzi();
  Serial.begin(115200); 
}

void updateControl() 
{
  readPins(); 
  aSin.setFreq(mtof(float(pitch)));
}

int updateAudio() 
{
  return (aSin.next()* gain)>>8;
}

void loop() 
{
  audioHook();
}