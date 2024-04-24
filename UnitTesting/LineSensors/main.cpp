#include "LineSensors.h"
#include "PurePursuitGeometry.h"

int main() {
    Point2D point1 = {0.0f, 0.45f};
    Point2D point2 = {1.0f, 0.6f};
    Point2D point3 = {2.0f, 0.75f};
	Point2D middleLine;
    ParabolaABC parabola;
    parabola = points2parabola_3(point1, point2, point3);
	middleLine = parabolaVertex(parabola);

	float arr[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	float res;
	LineSensors sensors(5);
	sensors.SetBackgroundColorOnlyCalibrationAvarages(arr);
	arr[0] = 1.0f;
	arr[1] = 1.0f;
	arr[2] = 1.0f;
	arr[3] = 1.0f;
	arr[4] = 1.0f;
	sensors.SetLineColorOlyCalibrationAvarages(arr);

	arr[0] = 1.0f;
	arr[1] = 0.0f;
	arr[2] = 0.0f;
	arr[3] = 0.0f;
	arr[4] = 0.0f;
	res = sensors.ReadSensors(arr);


	arr[0] = 0.0f;
	arr[1] = 1.0f;
	arr[2] = 0.0f;
	arr[3] = 0.0f;
	arr[4] = 0.0f;
	res = sensors.ReadSensors(arr);

	arr[0] = 0.6f;
	arr[1] = 0.75f;
	arr[2] = 0.35f;
	arr[3] = 0.35f;
	arr[4] = 0.35f;
	res = sensors.ReadSensors(arr);

	return 0;
}


