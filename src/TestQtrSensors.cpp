#include <StandardCplusplus.h>
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


// This example is designed for use with six analog QTR sensors. These
// reflectance sensors should be connected to analog pins A0 to A5. The
// sensors' emitter control pin (CTRL or LEDON) can optionally be connected to
// digital pin 2, or you can leave it disconnected and remove the call to
// setEmitterPin().
//
// The setup phase of this example calibrates the sensors for ten seconds and
// turns on the Arduino's LED (usually on pin 13) while calibration is going
// on. During this phase, you should expose each reflectance sensor to the
// lightest and darkest readings they will encounter. For example, if you are
// making a line follower, you should slide the sensors across the line during
// the calibration phase so that each sensor can get a reading of how dark the
// line is and how light the ground is. Improper calibration will result in
// poor readings.
//
// The main loop of the example reads the calibrated sensor values and uses
// them to estimate the position of a line. You can test this by taping a piece
// of 3/4" black electrical tape to a piece of white paper and sliding the
// sensor across it. It prints the sensor values to the serial monitor as
// numbers from 0 (maximum reflectance) to 1000 (minimum reflectance) followed
// by the estimated location of the line as a number from 0 to 5000. 1000 means
// the line is directly under sensor 1, 2000 means directly under sensor 2,
// etc. 0 means the line is directly under sensor 0 or was last seen by sensor
// 0 before being lost. 5000 means the line is directly under sensor 5 or was
// last seen by sensor 5 before being lost.

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
  300.0f, 300.0f, 300.0f, 300.0f, 300.0f
};

float LineColorOlyCalibrationAvarages[TOTAL_LINE_SENSORS] = {
  70.0f, 70.0f, 70.0f, 70.0f, 70.0f
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