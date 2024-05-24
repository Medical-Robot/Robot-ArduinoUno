#include <Arduino.h>
#define ENABLE_ARDUINO 1

#define MOTORS_LEFT_IN1_PIN1 6
#define MOTORS_LEFT_IN2_PIN2 5

#define MOTORS_RIGHT_IN3_PIN1 9
#define MOTORS_RIGHT_IN4_PIN2 3

#include <SteeringController.h>

SteeringController steeringController(255.0f, 0.0f, -255.0f);

void setup()
{
  Serial.begin(9600);
  /*
  while (!Serial) {
    delay(100);
  }*/

  pinMode(MOTORS_LEFT_IN1_PIN1, OUTPUT);
  pinMode(MOTORS_LEFT_IN2_PIN2, OUTPUT);
  pinMode(MOTORS_RIGHT_IN3_PIN1, OUTPUT);
  pinMode(MOTORS_RIGHT_IN4_PIN2, OUTPUT);

  analogWrite(MOTORS_LEFT_IN1_PIN1, LOW);
  analogWrite(MOTORS_LEFT_IN2_PIN2, LOW);
  analogWrite(MOTORS_RIGHT_IN3_PIN1, LOW);
  analogWrite(MOTORS_RIGHT_IN4_PIN2, LOW);
}


void loop()
{
  steeringController.write(1.0f, -1.0f, 1.0f);
}