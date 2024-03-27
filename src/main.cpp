#include <Arduino.h>

#define MOTORS_LEFT_IN1_PIN1 9
#define MOTORS_LEFT_IN2_PIN2 3
#define MOTORS_RIGHT_IN3_PIN1 6
#define MOTORS_RIGHT_IN4_PIN2 5


void setup() {
  // put your setup code here, to run once:
  pinMode(MOTORS_LEFT_IN1_PIN1, OUTPUT);
  pinMode(MOTORS_LEFT_IN2_PIN2, OUTPUT);
  pinMode(MOTORS_RIGHT_IN3_PIN1,   OUTPUT);
  pinMode(MOTORS_RIGHT_IN4_PIN2, OUTPUT);

}

void moveBackwards(int pin1, int pin2, int val){
  analogWrite(pin1, val);
  analogWrite(pin2, LOW);

}

void moveForward(int pin1, int pin2, int val){
  analogWrite(pin1, LOW);
  analogWrite(pin2, val);


}

void rotateMoveRight360(int speed){
  moveForward(MOTORS_LEFT_IN1_PIN1, MOTORS_LEFT_IN2_PIN2, speed);
  moveBackwards(MOTORS_RIGHT_IN3_PIN1, MOTORS_RIGHT_IN4_PIN2, speed);

}

void rotateMoveLeft360(int speed){
  moveBackwards(MOTORS_LEFT_IN1_PIN1, MOTORS_LEFT_IN2_PIN2, speed);
  moveForward(MOTORS_RIGHT_IN3_PIN1, MOTORS_RIGHT_IN4_PIN2, speed);

}

void stopMove(int pin1, int pin2, int pin3, int pin4){
  analogWrite(pin1, LOW);
  analogWrite(pin2, LOW);
  analogWrite(pin3, LOW);
  analogWrite(pin4, LOW);


}

void loop() {
  float startTime = 0.0f;
  //moveForward(MOTORS_LEFT_IN1_PIN1, MOTORS_LEFT_IN2_PIN2);
  //moveForward(MOTORS_RIGHT_IN3_PIN1, MOTORS_RIGHT_IN4_PIN2);

  //delay(5000);

  //moveBackwards(MOTORS_LEFT_IN1_PIN1, MOTORS_LEFT_IN2_PIN2);
  //moveBackwards(MOTORS_RIGHT_IN3_PIN1, MOTORS_RIGHT_IN4_PIN2);

  //delay(5000);
  /*
startTime = (float) millis();
while (((float)millis() - startTime) < 5000) {
  rotateMoveRight360(255);
}

startTime = (float) millis();
while (((float)millis() - startTime) < 5000) {
  rotateMoveRight360(128);
}
*/
startTime = (float) millis();
while (((float)millis() - startTime) < 5000) {
  rotateMoveRight360(64);
}


  //Controlling speed (0   = off and 255 = max speed):     
  //analogWrite(MOTORS_LEFT_IN1_PIN1,   HIGH);
  //analogWrite(MOTORS_LEFT_IN2_PIN2, LOW);

  //analogWrite(MOTORS_RIGHT_IN3_PIN1, HIGH);
  //analogWrite(MOTORS_RIGHT_IN4_PIN2, LOW);
  //delay(3000);

  //analogWrite(MOTORS_LEFT_IN1_PIN1,   LOW);
  //analogWrite(MOTORS_LEFT_IN2_PIN2, HIGH);

  //analogWrite(MOTORS_RIGHT_IN3_PIN1, LOW);
  //analogWrite(MOTORS_RIGHT_IN4_PIN2, HIGH);
  //delay(3000);

}
