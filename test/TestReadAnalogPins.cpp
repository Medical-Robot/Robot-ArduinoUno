#include <StandardCplusplus.h>
#include <Arduino.h>


/*
  White offset
  S1: 75
  S2: 55
  S3: 56
  S4: 74
  S5: 82
*/


void setup(){
    Serial.begin(9600);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
}


void loop(){
    Serial.print(analogRead(A0));
    Serial.print('\t');
    Serial.print(analogRead(A1));
    Serial.print('\t');
    Serial.print(analogRead(A2));
    Serial.print('\t');
    Serial.print(analogRead(A3));
    Serial.print('\t');
    Serial.print(analogRead(A4));
    Serial.print('\n');
}