//Digital Glitch Mixer v1.2
//by Po8aster
//Based on the library Arduino-X9C by Phil Bowles (https://github.com/philbowles/Arduino-X9C)

//Adds in the ability to control the sensitivity via a potentiometer

#include <X9C.h>

#define INC 2   // D1 Mini D4 - pulled up in H/W (10k) ->  chip pin 1
#define UD 15   // D1 Mini D8                          ->  chip pin 2
#define CS 16   // D1 Mini D0 - pulled up in H/W (10k) ->  2nd chip pin 7

int val = 0; //value based on incoming audio
int absVal = 0; //absolute value of Val
int bias = 511; //set bias amount (around 512)
int del = 100; // delay time between check ins (recc at least 100)


int potStatic = 0; // pot value when incoming signal is below threshold (50 = midpoint; 0 = lowpoint)

int audioPin = A0; // pin for audio signal coming in
int potVal = 0; //value to send to pot (0-99)
//int potValLast = 0; //tracks the last value to reduce spam when value doesn't change 

int sensPin = A1; // pin for sensitivity control
int sens = 0; // value read from sensitivity control
int threshold = 25; // threshold value, anything under this number keeps the pot at its static value (0-512)
int thresholdMin = 25; // minimum value of the threshold, 25 feels pretty good
int thresholdMax = 450; //maximum value of the threshold, bout 350

X9C pot;         // create a pot controller

void setup() {
  Serial.begin(115200);
  pot.begin(CS,INC,UD);
}

void loop() {

//read audio signal level and do some math do get a useful number 
val = (analogRead(audioPin)); //read value from audio pin
absVal = abs(val - bias);
//Serial.println (absVal); //print the value

//set the threshold based on sensitivity control
sens = analogRead(sensPin);
threshold = map(sens, 0, 1023, thresholdMin, thresholdMax);

//set the pot value if above threshold, otherwise stays at static value
if (absVal >= threshold){
  potVal = map(absVal, threshold, 512, 0, 99); //maps the pot value from 0-99 based on difference between threshold and max (512)
}
else{
  potVal = potStatic;
}

pot.setPot(potVal,false); //update the pot

//if (potVal != potValLast){ //if the pot value has changed, print some info
Serial.print(sens); //print the sensitivity
Serial.print(" / ");
Serial.print(threshold); //print the threshold
Serial.print(" / ");
Serial.println (potVal);  //print the pot value
//}
//else{
//}
//potValLast = potVal; //update value tracking
delay(del); //wait a bit before looping
}
