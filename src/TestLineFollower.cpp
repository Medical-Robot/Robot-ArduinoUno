#include <StandardCplusplus.h>
#include <Arduino.h>
#include <LineSensors.h>
#define ENABLE_ARDUINO 1

#define MOTORS_LEFT_IN1_PIN1 6
#define MOTORS_LEFT_IN2_PIN2 5

#define MOTORS_RIGHT_IN3_PIN1 9
#define MOTORS_RIGHT_IN4_PIN2 3

#include <SteeringController.h>


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

#define TOTAL_LINE_SENSORS 5
#define LINE_SENSOR_1_PIN A0
#define LINE_SENSOR_2_PIN A1
#define LINE_SENSOR_3_PIN A2
#define LINE_SENSOR_4_PIN A3
#define LINE_SENSOR_5_PIN A4

const float PID_Kp = 1.0f;

LineSensors lineSensors(TOTAL_LINE_SENSORS);
float sensorsReadings[TOTAL_LINE_SENSORS];
Point2D linePosition;

SteeringController steeringController(255.0f, 0.0f, -255.0f);

float BackgroundColorOnlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
  225.0f, 91.0f, 101.0f, 203.0f, 197.0f
};

float LineColorOlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
  581.0f, 572.0f, 557.0f, 586.0f, 588.0f
};

void readLineSensors(float* sensorsReadings_){
  sensorsReadings_[0] = analogRead(LINE_SENSOR_1_PIN);
  sensorsReadings_[1] = analogRead(LINE_SENSOR_2_PIN);
  sensorsReadings_[2] = analogRead(LINE_SENSOR_3_PIN);
  sensorsReadings_[3] = analogRead(LINE_SENSOR_4_PIN);
  sensorsReadings_[4] = analogRead(LINE_SENSOR_5_PIN);
}

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    delay(100);
  }

  pinMode(MOTORS_LEFT_IN1_PIN1, OUTPUT);
  pinMode(MOTORS_LEFT_IN2_PIN2, OUTPUT);
  pinMode(MOTORS_RIGHT_IN3_PIN1, OUTPUT);
  pinMode(MOTORS_RIGHT_IN4_PIN2, OUTPUT);

  analogWrite(MOTORS_LEFT_IN1_PIN1, LOW);
  analogWrite(MOTORS_LEFT_IN2_PIN2, LOW);
  analogWrite(MOTORS_RIGHT_IN3_PIN1, LOW);
  analogWrite(MOTORS_RIGHT_IN4_PIN2, LOW);

  pinMode(LINE_SENSOR_1_PIN, INPUT);
  pinMode(LINE_SENSOR_2_PIN, INPUT);
  pinMode(LINE_SENSOR_3_PIN, INPUT);
  pinMode(LINE_SENSOR_4_PIN, INPUT);
  pinMode(LINE_SENSOR_5_PIN, INPUT);

  lineSensors.SetBackgroundColorOnlyCalibrationAvarages(BackgroundColorOnlyCalibrationAvarages);
  lineSensors.SetLineColorOlyCalibrationAvarages(LineColorOlyCalibrationAvarages);
  
}

float speed = 0.5f;
float right_track_speed_cercentage = 1.0f;
float left_track_speed_cercentage = 1.0f;

void loop()
{
  readLineSensors(sensorsReadings);
  linePosition = lineSensors.ReadSensors(sensorsReadings);
  Serial.print("Posx:" + String(linePosition.x));
  Serial.print('\t');
  Serial.print("Posy:" + String(linePosition.x));
  Serial.print('\t');


  if (linePosition.x < 0.0f) {
    left_track_speed_cercentage = PID_Kp * (((linePosition.x) * speed) / speed);
    right_track_speed_cercentage = PID_Kp * (((1.0f - linePosition.x) * speed) / speed);
  }
  else if (linePosition.x > 0.0f) {
    right_track_speed_cercentage = PID_Kp * (((-linePosition.x) * speed) / speed);
    left_track_speed_cercentage = PID_Kp * (((1.0f + linePosition.x) * speed) / speed);
  }
  else{
    right_track_speed_cercentage = PID_Kp * (1.0f);
    left_track_speed_cercentage = PID_Kp * (1.0f);
  }
  
/*
  right_track_speed_cercentage = PID_Kp * (((1.0 - linePosition.x) * speed) / speed);
  left_track_speed_cercentage = PID_Kp * (((1.0f + linePosition.x) * speed) / speed);

*/
  left_track_speed_cercentage = MIN(left_track_speed_cercentage, 1.0f);
  left_track_speed_cercentage = MAX(left_track_speed_cercentage, -0.9f);
  right_track_speed_cercentage = MIN(right_track_speed_cercentage, 1.0f);
  right_track_speed_cercentage = MAX(right_track_speed_cercentage, -0.90f);


  Serial.print("left_track:" + String(left_track_speed_cercentage));
  Serial.print('\t');
  Serial.print("right_track:" + String(right_track_speed_cercentage));
  Serial.println();
  //steeringController.write(1.0f, 1.0f, 1.0f);
  steeringController.write(speed, left_track_speed_cercentage, right_track_speed_cercentage);
  //moveForward(MOTORS_RIGHT_IN3_PIN1, MOTORS_RIGHT_IN4_PIN2, 128);
}