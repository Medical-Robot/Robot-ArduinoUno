#include <Arduino.h>
#include <LineSensors.h>

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

LineSensors lineSensors(TOTAL_LINE_SENSORS);
float sensorsReadings[TOTAL_LINE_SENSORS];
float linePosition;

float BackgroundColorOnlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
  75.0f, 55.0f, 56.0f, 74.0f, 82.0f
};

float LineColorOlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
  500.0f, 500.0f, 500.0f, 500.0f, 500.0f
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
  pinMode(LINE_SENSOR_1_PIN, INPUT);
  pinMode(LINE_SENSOR_2_PIN, INPUT);
  pinMode(LINE_SENSOR_3_PIN, INPUT);
  pinMode(LINE_SENSOR_4_PIN, INPUT);
  pinMode(LINE_SENSOR_5_PIN, INPUT);

  lineSensors.SetBackgroundColorOnlyCalibrationAvarages(BackgroundColorOnlyCalibrationAvarages);
  lineSensors.SetLineColorOlyCalibrationAvarages(LineColorOlyCalibrationAvarages);

  Serial.begin(9600);
  while (!Serial) {
    delay(100);
  }
  
}

void loop()
{
  readLineSensors(sensorsReadings);
  linePosition = lineSensors.ReadSensors(sensorsReadings);
  Serial.println(linePosition);
}