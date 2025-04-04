//Digital Glitch Mixer v2.1
//by Po8aster
//Based on the library Arduino-X9C by Phil Bowles (https://github.com/philbowles/Arduino-X9C)

//Adds in a second digital pot (102) for patchbay control

#include <X9C.h>

#define INC 2   // D1 Mini D4 - pulled up in H/W (10k) ->  chip pin 1
#define UD 15   // D1 Mini D8                          ->  chip pin 2
#define CS 16   // D1 Mini D0 - pulled up in H/W (10k) ->  chip pin 7

#define INCB 6   // pulled up in H/W (10k) ->  chip pin 6
#define UDB 4   //                         ->  chip pin 4
#define CSB 8   // pulled up in H/W (10k) ->  chip pin 8

int val = 0; //value based on incoming audio
int absVal = 0; //absolute value of Val
int bias = 511; //set bias amount (around 512)
int del = 250; // delay time between check ins (recc at least 100)


int potStatic = 0; // pot value when incoming signal is below threshold (50 = midpoint; 0 = lowpoint)
int potStatic2 = 0; // pot value when incoming signal is below threshold (50 = midpoint; 0 = lowpoint)

int audioPin = A0; // pin for audio signal coming in
int potVal = 0; //value to send to pot (0-99)
int potVal2 = 0; //value to send to second pot (0-99)


int sensPin = A1; // pin for sensitivity control
int sens = 0; // value read from sensitivity control
int threshold = 25; // threshold value, anything under this number keeps the pot at its static value (0-512)
int thresholdMin = 25; // minimum value of the threshold, 25 feels pretty good
int thresholdMax = 450; //maximum value of the threshold, bout 350
int sensPin2 = A2; // pin for second sensitivity control
int sens2 = 0; // value read from second sensitivity control
int threshold2 = 25; // second threshold value, anything under this number keeps the pot at its static value (0-512)
int thresholdMin2 = 25; // minimum value of the threshold, 25 feels pretty good
int thresholdMax2 = 450; //maximum value of the threshold, bout 350

X9C pot;         // create a pot controller
X9C potB;       // second pot

void setup() {
  Serial.begin(115200);
  pot.begin(CS,INC,UD);
  potB.begin(CSB,INCB,UDB);
}

void loop() {

//read audio signal level and do some math do get a useful number 
val = (analogRead(audioPin)); //read value from audio pin
absVal = abs(val - bias);
//Serial.println (absVal); //print the value

//set the first threshold based on sensitivity control
sens = analogRead(sensPin);
threshold = map(sens, 0, 1023, thresholdMin, thresholdMax);

//set the pot value if above threshold, otherwise stays at static value
if (absVal >= threshold){
  potVal = map(absVal, threshold, 512, 0, 99); //maps the pot value from 0-99 based on difference between threshold and max (512)
}
else{
  potVal = potStatic;
}

//set the second threshold based on sensitivity control
sens2 = analogRead(sensPin2);
threshold2 = map(sens2, 0, 1023, thresholdMin2, thresholdMax2);

//set the pot value if above threshold, otherwise stays at static value
if (absVal >= threshold2){
  potVal2 = map(absVal, threshold2, 512, 0, 99); //maps the pot value from 0-99 based on difference between threshold and max (512)
}
else{
  potVal2 = potStatic2;
}

pot.setPot(potVal,false); //update the pot
potB.setPot(potVal2,false); //update the second pot

//if (potVal != potValLast){ //if the pot value has changed, print some info
Serial.print(potVal); //print the sensitivity
Serial.print(" / ");
Serial.println(potVal2); //print the threshold
//Serial.print(" / ");
//Serial.println (potVal);  //print the pot value
//}
//else{
//}
//potValLast = potVal; //update value tracking
delay(del); //wait a bit before looping
}
