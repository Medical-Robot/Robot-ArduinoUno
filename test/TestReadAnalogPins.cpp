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

/*
  background laborator offset
  S1: 225
  S2: 91
  S3: 101
  S4: 203
  S5: 197
*/


/*
  colored line value:
  S1: 581
  S2: 572
  S3: 557
  S4: 586
  S5: 588
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