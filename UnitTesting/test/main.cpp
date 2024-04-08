/*
* Copyright 2023 Constantin Dumitru Petre RĂDULICEA
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#include <stdio.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define abs(x)  (((x)<0)?-(x):(x))



#define MOTORS_LEFT_IN1_PIN1 9
#define MOTORS_LEFT_IN2_PIN2 3
#define MOTORS_RIGHT_IN3_PIN1 6
#define MOTORS_RIGHT_IN4_PIN2 5


#define ENABLE_ARDUINO 0
#define BaseSteeringController_ENABLE_PURE_VIRTUAL_FUNCTIONS 1


#if ENABLE_ARDUINO == 1
	void moveBackward(int pin1, int pin2, int val) {
		analogWrite(pin1, val);
		analogWrite(pin2, LOW);
	}

	void moveForward(int pin1, int pin2, int val) {
		analogWrite(pin1, LOW);
		analogWrite(pin2, val);
	}
#endif // ENABLE_ARDUINO == 1





class BaseSteeringController
{
public:
	BaseSteeringController(float max_forward_raw_value, float stand_still_speed_raw_value, float max_backward_raw_value){
		this->MaxForwardSpeedRawValue = max_forward_raw_value;
		this->MaxBackwardSpeedRawValue = max_backward_raw_value;
		this->StandStillSpeedRawValue = stand_still_speed_raw_value;
		this->LeftTrackSpeed = 0.0f;
		this->RightTrackSpeed = 0.0f;
		LeftTrackIsGoingForward = false;
		LeftTrackIsGoingBackward = false;
		LeftTrackIsStandingStill = false;
		RightTrackIsGoingForward = false;
		RightTrackIsGoingBackward = false;
		RightTrackIsStandingStill = false;
	}
	~BaseSteeringController(){}

#if BaseSteeringController_ENABLE_PURE_VIRTUAL_FUNCTIONS == 1
	virtual void writeLeftTrackMotor() = 0;
	virtual void writeRightTrackMotor() = 0;
#endif // BaseSteeringController_ENABLE_PURE_VIRTUAL_FUNCTIONS == 1

	void setMaxForwardSpeedRawValue(float speed) {
		this->MaxForwardSpeedRawValue = speed;
	}

	float getMaxForwardSpeedRawValue() {
		return this->MaxForwardSpeedRawValue;
	}

	void setMaxBackwardSpeedRawValue(float speed) {
		this->MaxBackwardSpeedRawValue = speed;
	}

	float getMaxBackwardSpeedRawValue() {
		return this->MaxBackwardSpeedRawValue;
	}

	void setStandStillSpeedRawValue(float speed) {
		this->StandStillSpeedRawValue = speed;
	}

	float getStandStillSpeedRawValue() {
		return this->StandStillSpeedRawValue;
	}

	float getLeftTrackSpeed() {
		return this->LeftTrackSpeed;
	}

	float getRightTrackSpeed() {
		return this->RightTrackSpeed;
	}

	void calculateSteering(float speed_percentage, float left_track_percentage, float right_track_percentage) {
		float temp_LeftTrackSpeed, temp_RightTrackSpeed;
		float forward_speed_span, backward_speed_span;

		LeftTrackIsGoingForward = false;
		LeftTrackIsGoingBackward = false;
		LeftTrackIsStandingStill = false;
		RightTrackIsGoingForward = false;
		RightTrackIsGoingBackward = false;
		RightTrackIsStandingStill = false;


		temp_LeftTrackSpeed = left_track_percentage * speed_percentage;
		temp_RightTrackSpeed = right_track_percentage * speed_percentage;

		forward_speed_span = MaxForwardSpeedRawValue - StandStillSpeedRawValue;
		backward_speed_span = StandStillSpeedRawValue - MaxBackwardSpeedRawValue;


		if (temp_LeftTrackSpeed == 0.0f) {
			temp_LeftTrackSpeed = StandStillSpeedRawValue;
			LeftTrackIsStandingStill = true;
		}
		else if (temp_LeftTrackSpeed > 0.0f) {
			temp_LeftTrackSpeed = StandStillSpeedRawValue + (abs(temp_LeftTrackSpeed) * forward_speed_span);
			LeftTrackIsGoingForward = true;
		}
		else {
			temp_LeftTrackSpeed = StandStillSpeedRawValue - (abs(temp_LeftTrackSpeed) * backward_speed_span);
			LeftTrackIsGoingBackward = true;
		}

		if (temp_RightTrackSpeed == 0.0f) {
			temp_RightTrackSpeed = StandStillSpeedRawValue;
			RightTrackIsStandingStill = true;
		}
		else if (temp_RightTrackSpeed > 0.0f) {
			temp_RightTrackSpeed = StandStillSpeedRawValue + (abs(temp_RightTrackSpeed) * forward_speed_span);
			RightTrackIsGoingForward = true;
		}
		else {
			temp_RightTrackSpeed = StandStillSpeedRawValue - (abs(temp_RightTrackSpeed) * backward_speed_span);
			RightTrackIsGoingBackward = true;
		}

		LeftTrackSpeed = temp_LeftTrackSpeed;
		RightTrackSpeed = temp_RightTrackSpeed;
	}

	void write(float speed_percentage, float left_track_percentage, float right_track_percentage) {
		this->calculateSteering(speed_percentage, left_track_percentage, right_track_percentage);

		#if BaseSteeringController_ENABLE_PURE_VIRTUAL_FUNCTIONS == 1
			this->writeLeftTrackMotor();
			this->writeRightTrackMotor();
		#endif // BaseSteeringController_ENABLE_PURE_VIRTUAL_FUNCTIONS == 1
	}

protected:
	float MaxForwardSpeedRawValue;
	float StandStillSpeedRawValue;
	float MaxBackwardSpeedRawValue;
	float LeftTrackSpeed;
	float RightTrackSpeed;

	float LeftTrackIsGoingForward;
	float LeftTrackIsGoingBackward;
	float LeftTrackIsStandingStill;

	float RightTrackIsGoingForward;
	float RightTrackIsGoingBackward;
	float RightTrackIsStandingStill;
};



class SteeringController : public BaseSteeringController
{
public:
	SteeringController(float max_forward_raw_value, float stand_still_speed_raw_value, float max_backward_raw_value) : BaseSteeringController(max_forward_raw_value, stand_still_speed_raw_value, max_backward_raw_value){};
	
	virtual void writeLeftTrackMotor() {
		if (this->LeftTrackIsStandingStill == true) {
			#if ENABLE_ARDUINO == 1
				moveForward(MOTORS_LEFT_IN1_PIN1, MOTORS_LEFT_IN2_PIN2, (int)(this->LeftTrackSpeed));
			#endif // ENABLE_ARDUINO == 1
		}
		else if (this->LeftTrackIsGoingForward == true) {
			#if ENABLE_ARDUINO == 1
				moveForward(MOTORS_LEFT_IN1_PIN1, MOTORS_LEFT_IN2_PIN2, (int)(this->LeftTrackSpeed));
			#endif // ENABLE_ARDUINO == 1
		}
		else if (this->LeftTrackIsGoingBackward == true) {
			#if ENABLE_ARDUINO == 1
				moveBackward(MOTORS_LEFT_IN1_PIN1, MOTORS_LEFT_IN2_PIN2, (int)(this->LeftTrackSpeed));
			#endif // ENABLE_ARDUINO == 1
		}
	}

	virtual void writeRightTrackMotor() {
		if (this->RightTrackIsStandingStill == true) {
			#if ENABLE_ARDUINO == 1
				moveForward(MOTORS_RIGHT_IN3_PIN1, MOTORS_RIGHT_IN4_PIN2, (int)(this->RightTrackSpeed));
			#endif // ENABLE_ARDUINO == 1
		}
		else if (this->RightTrackIsGoingForward == true) {
			#if ENABLE_ARDUINO == 1
				moveForward(MOTORS_RIGHT_IN3_PIN1, MOTORS_RIGHT_IN4_PIN2, (int)(this->RightTrackSpeed));
			#endif // ENABLE_ARDUINO == 1
		}
		else if (this->RightTrackIsGoingBackward == true) {
			#if ENABLE_ARDUINO == 1
				moveBackward(MOTORS_RIGHT_IN3_PIN1, MOTORS_RIGHT_IN4_PIN2, (int)(this->RightTrackSpeed));
			#endif // ENABLE_ARDUINO == 1
		}
	}

private:
};


class LineFollower
{
public:
	LineFollower(SteeringController &steeringWeel) : SteeringWeel(steeringWeel){
		setLineSensorPins(-1, -1, -1, -1, -1);
	}
	void setLineSensorPins(int LineSensorLeft2Pin, int LineSensorLeft1Pin, int LineSensorMiddlePin, int LineSensorRight1Pin, int LineSensorRight2Pin) {
		this->LineSensorLeft2Pin = LineSensorLeft2Pin;
		this->LineSensorLeft1Pin = LineSensorLeft1Pin;
		this->LineSensorMiddlePin = LineSensorMiddlePin;
		this->LineSensorRight1Pin = LineSensorRight1Pin;
		this->LineSensorRight2Pin = LineSensorRight2Pin;
	}

	void readLineSensors() {
		#if ENABLE_ARDUINO == 1
			LineSensorMiddleRawValue = analogRead(LineSensorMiddlePin);
			LineSensorLeft1RawValue = analogRead(LineSensorLeft1Pin);
			LineSensorLeft2RawValue = analogRead(LineSensorLeft2Pin);
			LineSensorRight1RawValue = analogRead(LineSensorRight1Pin);
			LineSensorRight2RawValue = analogRead(LineSensorRight2Pin);
		#endif
	}

	void logic() {
		this->readLineSensors();
	}
	
	~LineFollower(){}

private:
	int LineSensorMiddlePin;
	int LineSensorLeft1Pin;
	int LineSensorLeft2Pin;
	int LineSensorRight1Pin;
	int LineSensorRight2Pin;
	float LineSensorMiddleRawValue;
	float LineSensorLeft1RawValue;
	float LineSensorLeft2RawValue;
	float LineSensorRight1RawValue;
	float LineSensorRight2RawValue;
	SteeringController& SteeringWeel;
};


int main() {
	SteeringController steeringWeel(180, 90, 0);


	steeringWeel.write(0.1f, -0.1f, 1.0f);
	steeringWeel.write(1.0f, 0.0f, 0.0f);
	steeringWeel.write(0.1f, -0.1f, 1.0f);
	steeringWeel.write(0.1f, -0.1f, 1.0f);

	return 0;
}