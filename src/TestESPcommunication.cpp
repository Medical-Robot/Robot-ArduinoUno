#include <StandardCplusplus.h>
#include <Arduino.h>
#include <Wire.h>
#include "CommandParser.h"

#define SLAVE_ADDRESS 9

void receiveEvent(int bytes)
{
  while (Wire.available())
  {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

void setup()
{
  Wire.begin(SLAVE_ADDRESS);
  //Wire.onReceive(receiveEvent);
  Serial.begin(9600);
      while (!Serial){
      delay(100);
    }
  
  Serial.println("Arduino Started");
  delay(100);
}

void loop(){
    
}