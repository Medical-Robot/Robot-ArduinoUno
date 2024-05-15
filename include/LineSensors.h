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
		this->sensorPins = new int[NumberOfSensors_];
		this->LineSensorsRawValues = new float[NumberOfSensors_];
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
	void processSensors2(float* sensorsReadings) {
		float tempAverage;
		float middleSensorDistance = (((float)this->NumberOfSensors - 1) / 2.0f);
		int MaxValueSensorIndex, MinValueSensorIndex;
		ParabolaABC parabola;
		Point2D middleLine = { -2.0f, -1.0f }, min_value_temp;

		if (this->NumberOfSensors <= 0) {
			//return middleLine;
			return;
		}
		for (size_t i = 0; i < this->NumberOfSensors; i++) {
			this->LineColorSensorsPercentage[i] = (sensorsReadings[i] - this->BackgroundColorOnlyCalibrationAvarages[i]) / (this->LineColorOlyCalibrationAvarages[i] - this->BackgroundColorOnlyCalibrationAvarages[i]);
		}
		MaxValueSensorIndex = this->MaxValueIndexFloatArray(this->LineColorSensorsPercentage, this->NumberOfSensors);
		MinValueSensorIndex = this->MinValueIndexFloatArray(this->LineColorSensorsPercentage, this->NumberOfSensors);

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
				if (this->LineColorSensorsPercentage[MaxValueSensorIndex - 2] < this->LineColorSensorsPercentage[MaxValueSensorIndex - 1])
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
				if (this->LineColorSensorsPercentage[MaxValueSensorIndex + 2] < this->LineColorSensorsPercentage[MaxValueSensorIndex + 1])
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

		if (this->NumberOfSensors >= 3)
		{
			if (MinValueSensorIndex > 0 && MinValueSensorIndex < (this->NumberOfSensors - 1)) {
				parabola = points2parabola_3({ (float)(MinValueSensorIndex - 1), this->LineColorSensorsPercentage[MinValueSensorIndex - 1] },
					{ (float)(MinValueSensorIndex), this->LineColorSensorsPercentage[MinValueSensorIndex] },
					{ (float)(MinValueSensorIndex + 1), this->LineColorSensorsPercentage[MinValueSensorIndex + 1] });
				min_value_temp = parabolaVertex(parabola);
			}
			else if (MinValueSensorIndex > 0 && MinValueSensorIndex >= (this->NumberOfSensors - 1))
			{
				if (this->LineColorSensorsPercentage[MinValueSensorIndex - 2] > this->LineColorSensorsPercentage[MinValueSensorIndex - 1])
				{
					parabola = points2parabola_3({ (float)(MinValueSensorIndex - 1), this->LineColorSensorsPercentage[MinValueSensorIndex - 1] },
						{ (float)(MinValueSensorIndex), this->LineColorSensorsPercentage[MinValueSensorIndex] },
						{ (float)(MinValueSensorIndex - 2), this->LineColorSensorsPercentage[MinValueSensorIndex - 2] });
					min_value_temp = parabolaVertex(parabola);
					min_value_temp.x = fminf((float)(this->NumberOfSensors - 1), min_value_temp.x);
				}
				else
				{
					min_value_temp.y = this->LineColorSensorsPercentage[MinValueSensorIndex];
					min_value_temp.x = MinValueSensorIndex;
				}
			}
			else if (MinValueSensorIndex == 0)
			{
				if (this->LineColorSensorsPercentage[MinValueSensorIndex + 2] > this->LineColorSensorsPercentage[MinValueSensorIndex + 1])
				{
					parabola = points2parabola_3({ (float)(MinValueSensorIndex + 1), this->LineColorSensorsPercentage[MinValueSensorIndex + 1] },
						{ (float)(MinValueSensorIndex), this->LineColorSensorsPercentage[MinValueSensorIndex] },
						{ (float)(MinValueSensorIndex + 2), this->LineColorSensorsPercentage[MinValueSensorIndex + 2] });
					min_value_temp = parabolaVertex(parabola);
					min_value_temp.x = fminf(0.0f, min_value_temp.x);
				}
				else
				{
					min_value_temp.y = this->LineColorSensorsPercentage[MinValueSensorIndex];
					min_value_temp.x = MinValueSensorIndex;
				}
			}
		}

		middleLine.x = middleLine.x - middleSensorDistance;
		middleLine.x = middleLine.x / middleSensorDistance;
		max_value = middleLine;

		min_value_temp.x = min_value_temp.x - middleSensorDistance;
		min_value_temp.x = min_value_temp.x / middleSensorDistance;
		min_value = min_value_temp;
		//return middleLine;
	}

	void processSensors(float* sensorsReadings) {
		float tempAverage;
		float middleSensorDistance = (((float)this->NumberOfSensors - 1) / 2.0f);
		int MaxValueSensorIndex;
		int rVal, n_roots;
		float polyfit_result[5];
		float polyder_result[4];
		float polyfit_max_roots[3];
		ParabolaABC parabola;
		Point2D middleLine = { -2.0f, -1.0f };
		Point2D color_max, color_min, temp_point2d;
		int isSetColor_max = 0, isSetColor_min = 0;
		float* temp_LineColorSensorsPercentage = new float[this->NumberOfSensors+2];
		float* temp_sensorXposition = new float[this->NumberOfSensors+2];

		if (this->NumberOfSensors <= 0) {
			//return middleLine;
			return;
		}
		for (size_t i = 0; i < this->NumberOfSensors; i++) {
			this->LineColorSensorsPercentage[i] = (sensorsReadings[i] - this->BackgroundColorOnlyCalibrationAvarages[i]) / (this->LineColorOlyCalibrationAvarages[i] - this->BackgroundColorOnlyCalibrationAvarages[i]);
		}
		/*
		memcpy(&(temp_LineColorSensorsPercentage[1]), this->LineColorSensorsPercentage, this->NumberOfSensors * sizeof(float));
		temp_LineColorSensorsPercentage[0] = 0.0f;
		temp_LineColorSensorsPercentage[this->NumberOfSensors + 1] = 0.0f;

		memcpy(&(temp_sensorXposition[1]), this->sensorXposition, this->NumberOfSensors * sizeof(float));
		temp_sensorXposition[0] = -1.0f;
		temp_sensorXposition[this->NumberOfSensors + 1] = (float)(this->NumberOfSensors);
		*/
		rVal = polyfit(this->NumberOfSensors, sensorXposition, LineColorSensorsPercentage, 5, polyfit_result);
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

		max_value = color_max;
		max_value.x = max_value.x - middleSensorDistance;
		max_value.x = max_value.x / middleSensorDistance;

		min_value = color_min;
		min_value.x = min_value.x - middleSensorDistance;
		min_value.x = min_value.x / middleSensorDistance;

		middleLine.x = middleLine.x - middleSensorDistance;
		middleLine.x = middleLine.x / middleSensorDistance;

		//return middleLine;
	}
	~LineSensors() {
		delete this->BackgroundColorOnlyCalibrationAvarages;
		delete this->LineColorOlyCalibrationAvarages;
		delete this->LineColorSensorsPercentage;
		delete this->sensorPins;
		delete this->LineSensorsRawValues;
	}
	Point2D getMaxValue() {
		return max_value;
	}

	Point2D getMinValue() {
		return min_value;
	}

	void setPins(int *pinArray, size_t nPins){
		this->sensorPins = new int[nPins];
		memcpy(sensorPins, pinArray, sizeof(int) * nPins);
	}

	void readSensors(float* sensorsReadings){
		for (size_t i = 0; i < NumberOfSensors; i++) {
			sensorsReadings[i] = analogRead(this->sensorPins[i]);
		}
		
	}

	void read(){
		this->readSensors(this->LineSensorsRawValues);
		this->processSensors2(this->LineSensorsRawValues);
	}

private:
	size_t NumberOfSensors;
	float* LineSensorsRawValues = nullptr;
	float* BackgroundColorOnlyCalibrationAvarages = nullptr;
	float* LineColorOlyCalibrationAvarages = nullptr;
	float* LineColorSensorsPercentage = nullptr;
	float* sensorXposition = nullptr;
	int* sensorPins = nullptr;
	Point2D max_value;
	Point2D min_value;


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

	int MinValueIndexFloatArray(float* arr, size_t arraySize) {
		float maxValue;
		size_t index = 0;
		maxValue = arr[0];
		for (int i = 1; i < arraySize; i++)
		{
			if (arr[i] < maxValue)
			{
				maxValue = arr[i];
				index = i;
			}
		}
		return index;
	}
};

