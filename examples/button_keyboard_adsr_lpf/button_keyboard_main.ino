/*  Example of a simple button keyboard with adsr and LowPass Filter

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
#include <tables/sin2048_int8.h> 
#include <tables/cos2048_int8.h> // for filter modulation
#include <LowPassFilter.h>
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aVib(SIN2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kFilterMod(COS2048_DATA);
//ADSR CONTROLL
ADSR <CONTROL_RATE, AUDIO_RATE> envelope;
//Analog Pins
const char INPUT_PIN0 = A0; 
const char INPUT_PIN1 = A1; 
const char INPUT_PIN2 = A2;
const char INPUT_PIN4 = A4;  
//
const char DIGITAL_PIN0 = A0; 
const char DIGITAL_PIN1 = A1; 
const char DIGITAL_PIN2 = A2;
const char DIGITAL_PIN3 = A5;  
//MIDI VALUES FOR REFERENCE
//c4 60
//d4 62
//e4 64
//f4 65
//g4 67
//a4 69
//b4 71
//c5 72
//digitial Pins
int pin_4;
int pin_5;
int pin_6;
int pin_7;
int pin_8;

///Standart Pitch
int pitch=60;//C4
//
//Analog Read
byte lpf_cuttoff;
//vibrato
byte vib_frenq;
const byte vib_intensity =100;
//LP-Filter
LowPassFilter lpf;


void readAnalogPins()
{
    
  int knob_value0 = mozziAnalogRead(INPUT_PIN0);
  int knob_value4 = mozziAnalogRead(INPUT_PIN4);

  lpf_cuttoff = map(knob_value0, 0, 1023, 0, 255);
  vib_frenq = map(knob_value4, 0, 1023, 0, 25);
}


void readDigitalPins() 
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
  envelope.noteOn();
  }  
  if (pin_5 == LOW)//E4
  {
  pitch=64;
  envelope.noteOn();
  }
  if (pin_6 == LOW)//F4
  {
  pitch=65;
  envelope.noteOn();
  }
  if (pin_6 == LOW)//G4
  {
  pitch=67;
  envelope.noteOn();
  }
  if (pin_7 == LOW)//A4
  {
  pitch=69;
  envelope.noteOn();
  }
  if (pin_8 == LOW)//B4
  {
  pitch=71;
  envelope.noteOn();
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
 
  envelope.setTimes(0,200,100,20); // every keypress is seen as an oneshot.
  Serial.begin(115200); 
}

void updateControl() 
{
  readDigitalPins(); 
  readAnalogPins();
  aSin.setFreq(mtof(float(pitch)));
  envelope.setADLevels(255,62);  
  aVib.setFreq((float)vib_frenq);
  envelope.update();
  lpf.setCutoffFreq(lpf_cuttoff);
  
}

int updateAudio() 
{
Q15n16 vibrato = (Q15n16) vib_intensity * aVib.next();
char afterVibrato=(int) (envelope.next() * aSin.phMod(vibrato))>>8;
char afterFilter=lpf.next(afterVibrato);
return (int) afterFilter;
//serial processing
//ADSR-VIBRATO-FILTER->OUTPUT
}

void loop() 
{
  audioHook();
}