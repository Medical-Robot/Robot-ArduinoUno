#pragma once
#include <string.h>
#include "PurePursuitGeometry.h"

class LineSensors
{
public:
	LineSensors(size_t NumberOfSensors_) {
		this->NumberOfSensors = NumberOfSensors_;
		this->BackgroundColorOnlyCalibrationAvarages = new float[NumberOfSensors_];
		this->LineColorOlyCalibrationAvarages = new float[NumberOfSensors_];
		this->LineColorSensorsPercentage = new float[NumberOfSensors_];
	}

	void SetBackgroundColorOnlyCalibrationAvarages(float* sensorsReadings) {
		memcpy(this->BackgroundColorOnlyCalibrationAvarages, sensorsReadings, sizeof(float) * NumberOfSensors);
	}
	void SetLineColorOlyCalibrationAvarages(float* sensorsReadings) {
		memcpy(this->LineColorOlyCalibrationAvarages, sensorsReadings, sizeof(float) * NumberOfSensors);
	}
	
	// returns an interval between -1 and 1 where 0 is the middle sensor, -1 is the left sensor and 1 is the right sensor
	Point2D ReadSensors(float* sensorsReadings) {
		float tempAverage;
		float middleSensorDistance = (((float)this->NumberOfSensors-1) / 2.0f);
		int MaxValueSensorIndex;
		ParabolaABC parabola;
		Point2D middleLine = {-2.0f, -1.0f};

		if (this->NumberOfSensors <= 0) {
			return middleLine;
		}
		for (size_t i = 0; i < this->NumberOfSensors; i++) {
			this->LineColorSensorsPercentage[i] = (sensorsReadings[i] - this->BackgroundColorOnlyCalibrationAvarages[i]) / (this->LineColorOlyCalibrationAvarages[i] - this->BackgroundColorOnlyCalibrationAvarages[i]);
		}
		MaxValueSensorIndex = this->MaxValueIndexFloatArray(this->LineColorSensorsPercentage, this->NumberOfSensors);
		

		if (this->NumberOfSensors >= 3)
		{
			if (MaxValueSensorIndex > 0 && MaxValueSensorIndex < (this->NumberOfSensors - 1)) {
				parabola = points2parabola_3({ (float)(MaxValueSensorIndex - 1), this->LineColorSensorsPercentage[MaxValueSensorIndex - 1] },
					{ (float)(MaxValueSensorIndex), this->LineColorSensorsPercentage[MaxValueSensorIndex] },
					{ (float)(MaxValueSensorIndex + 1), this->LineColorSensorsPercentage[MaxValueSensorIndex + 1] });
				middleLine = parabolaVertex(parabola);
			}
			else if (MaxValueSensorIndex > 0 && MaxValueSensorIndex >= (this->NumberOfSensors - 1))
			{
				if (this->LineColorSensorsPercentage[MaxValueSensorIndex - 2] <= this->LineColorSensorsPercentage[MaxValueSensorIndex - 1])
				{
					parabola = points2parabola_3({ (float)(MaxValueSensorIndex - 1), this->LineColorSensorsPercentage[MaxValueSensorIndex - 1] },
						{ (float)(MaxValueSensorIndex), this->LineColorSensorsPercentage[MaxValueSensorIndex] },
						{ (float)(MaxValueSensorIndex - 2), this->LineColorSensorsPercentage[MaxValueSensorIndex - 2] });
					middleLine = parabolaVertex(parabola);
					middleLine.x = fminf((float)(this->NumberOfSensors - 1), middleLine.x);
				}
				else
				{
					middleLine.y = this->LineColorSensorsPercentage[MaxValueSensorIndex];
					middleLine.x = MaxValueSensorIndex;
				}
			}
			else if (MaxValueSensorIndex == 0)
			{
				if (this->LineColorSensorsPercentage[MaxValueSensorIndex + 2] <= this->LineColorSensorsPercentage[MaxValueSensorIndex + 1])
				{
					parabola = points2parabola_3({ (float)(MaxValueSensorIndex + 1), this->LineColorSensorsPercentage[MaxValueSensorIndex + 1] },
						{ (float)(MaxValueSensorIndex), this->LineColorSensorsPercentage[MaxValueSensorIndex] },
						{ (float)(MaxValueSensorIndex + 2), this->LineColorSensorsPercentage[MaxValueSensorIndex + 2] });
					middleLine = parabolaVertex(parabola);
					middleLine.x = fminf(0.0f, middleLine.x);
				}
				else
				{
					middleLine.y = this->LineColorSensorsPercentage[MaxValueSensorIndex];
					middleLine.x = MaxValueSensorIndex;
				}
			}
		}

		middleLine.x = middleLine.x - middleSensorDistance;
		middleLine.x = middleLine.x / middleSensorDistance;

		return middleLine;
	}
	~LineSensors() {
		delete this->BackgroundColorOnlyCalibrationAvarages;
		delete this->LineColorOlyCalibrationAvarages;
		delete this->LineColorSensorsPercentage;
	}
private:
	size_t NumberOfSensors;
	float* BackgroundColorOnlyCalibrationAvarages = nullptr;
	float* LineColorOlyCalibrationAvarages = nullptr;
	float* LineColorSensorsPercentage = nullptr;


	int MaxValueIndexFloatArray(float* arr, size_t arraySize) {
		float maxValue = 0.0f;
		size_t index = -1;
		for (int i = 0; i < arraySize; i++)
		{
			if (arr[i] > maxValue)
			{
				maxValue = arr[i];
				index = i;
			}
		}
		return index;
	}
};

