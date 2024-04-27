#pragma once
#include <string.h>
#include "PurePursuitGeometry.h"
#include "cubic.c"
#include "polyfit.h"

class LineSensors
{
public:
	LineSensors(size_t NumberOfSensors_) {
		this->NumberOfSensors = NumberOfSensors_;
		this->BackgroundColorOnlyCalibrationAvarages = new float[NumberOfSensors_];
		this->LineColorOlyCalibrationAvarages = new float[NumberOfSensors_];
		this->LineColorSensorsPercentage = new float[NumberOfSensors_];
		this->sensorXposition = new float[NumberOfSensors_];
		for (size_t i = 0; i < NumberOfSensors_; i++)
		{
			this->sensorXposition[i] = i;
		}
	}

	void SetBackgroundColorOnlyCalibrationAvarages(float* sensorsReadings) {
		memcpy(this->BackgroundColorOnlyCalibrationAvarages, sensorsReadings, sizeof(float) * NumberOfSensors);
	}
	void SetLineColorOlyCalibrationAvarages(float* sensorsReadings) {
		memcpy(this->LineColorOlyCalibrationAvarages, sensorsReadings, sizeof(float) * NumberOfSensors);
	}

	void* calibratedReadingsPercentualLineColor(float* sensorsReadings) {
		for (size_t i = 0; i < this->NumberOfSensors; i++) {
			sensorsReadings[i] = (sensorsReadings[i] - this->BackgroundColorOnlyCalibrationAvarages[i]) / (this->LineColorOlyCalibrationAvarages[i] - this->BackgroundColorOnlyCalibrationAvarages[i]);
		}
	}

	// returns an interval between -1 and 1 where 0 is the middle sensor, -1 is the left sensor and 1 is the right sensor
	Point2D ReadSensors2(float* sensorsReadings) {
		float tempAverage;
		float middleSensorDistance = (((float)this->NumberOfSensors - 1) / 2.0f);
		int MaxValueSensorIndex;
		ParabolaABC parabola;
		Point2D middleLine = { -2.0f, -1.0f };

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

	Point2D ReadSensors(float* sensorsReadings) {
		float tempAverage;
		float middleSensorDistance = (((float)this->NumberOfSensors - 1) / 2.0f);
		int MaxValueSensorIndex;
		int rVal, n_roots;
		float polyfit_result[5];
		float polyder_result[4];
		float polyfit_max_roots[3];
		ParabolaABC parabola;
		Point2D middleLine = { -2.0f, -1.0f };
		Point2D color_max = { -2.0f, -1.0f }, color_min = { -2.0f, -1.0f }, temp_point2d = { -2.0f, -1.0f };
		int isSetColor_max = 0, isSetColor_min = 0;

		if (this->NumberOfSensors <= 0) {
			return middleLine;
		}
		for (size_t i = 0; i < this->NumberOfSensors; i++) {
			this->LineColorSensorsPercentage[i] = (sensorsReadings[i] - this->BackgroundColorOnlyCalibrationAvarages[i]) / (this->LineColorOlyCalibrationAvarages[i] - this->BackgroundColorOnlyCalibrationAvarages[i]);
		}
		rVal = polyfit(5, this->sensorXposition, this->LineColorSensorsPercentage, 5, polyfit_result);
		polyder(polyfit_result, 5, polyder_result);
		n_roots = solve_cubic(polyder_result[0], polyder_result[1], polyder_result[2], polyder_result[3], polyfit_max_roots);

		for (size_t i = 0; i < n_roots; i++)
		{
			temp_point2d = polyval(polyfit_result, 4, polyfit_max_roots[i]);
			if (isSetColor_max)
			{
				if (color_max.y < temp_point2d.y) {
					color_max = temp_point2d;
				}
			}
			else {
				color_max = temp_point2d;
				isSetColor_max = 1;
			}

			if (isSetColor_min)
			{
				if (color_min.y > temp_point2d.y) {
					color_min = temp_point2d;
				}
			}
			else {
				color_min = temp_point2d;
				isSetColor_min = 1;
			}
		}

		middleLine = color_max;

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
	float* sensorXposition = nullptr;


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

