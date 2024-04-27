#include "LineSensors.h"
#include "PurePursuitGeometry.h"
#include "polyfit.h"
#include <stdio.h>
#include <complex.h>
#include "Polynomial.h"
#include "cubic.c"


int main() {
	int rVal;
    Point2D point1 = {0.0f, 0.45f};
    Point2D point2 = {1.0f, 0.6f};
    Point2D point3 = {2.0f, 0.75f};
	Point2D middleLine;
    ParabolaABC parabola;
	Polynomial polynomial;
	


    parabola = points2parabola_3(point1, point2, point3);
	middleLine = parabolaVertex(parabola);

	float arr[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	float x_arr[5] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f };
	float polynom_result[10];
	float polynom_der[10];
	Point2D res;
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
	arr[2] = 0.75f;
	arr[3] = 0.1f;
	arr[4] = 0.1f;
	rVal = polyfit(5, x_arr, arr, 5, polynom_result);
	for (size_t i = 0; i < 5; i++)
	{
		printf("%f ", polynom_result[i]);
	}printf("\n");

	float real[6] = {};
	float immaginary[6] = {};
	int n_roots;

	polyder(polynom_result, 5, polynom_der);
	for (size_t i = 0; i < 4; i++)
	{
		printf("%f ", polynom_der[i]);
	}
	printf("\n");

	//std::reverse(polynom_der, &(polynom_der[3]));
	//polynomial.SetCoefficients(polynom_der, 3);
	//polynomial.FindRoots(real, immaginary, &n_roots);

	//for (size_t i = 0; i < n_roots; i++)
	//{
	//	printf("%f ", real[i]);
	//}
	//printf("\n");

	n_roots = solve_cubic(polynom_der[0], polynom_der[1], polynom_der[2], polynom_der[3], real);
	for (size_t i = 0; i < n_roots; i++)
	{
		printf("%f ", real[i]);
	}
	printf("\n");

	for (size_t i = 0; i < n_roots; i++)
	{
		middleLine = polyval(polynom_result, 4, real[i]);
		printf("(%f; %f) ", middleLine.x, middleLine.y);
	}
	printf("\n");
	
	
	res = sensors.ReadSensors(arr);

	return 0;
}


