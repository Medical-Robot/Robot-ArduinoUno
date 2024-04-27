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




#ifndef __PUREPURSUITGEOMETRY_H__
#define __PUREPURSUITGEOMETRY_H__

#include <math.h>
#include <string.h>

#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2

#define INCONSISTENT_ECUATION_SYSTEM 1
#define CONSISTENT_ECUATION_SYSTEM 2

typedef struct LineMQ {
	float m;
	float q;
}LineMQ;

typedef struct ParabolaABC {
	float A;
	float B;
	float C;
};

typedef struct LineABC {
	float Ax;
	float By;
	float C;
}LineABC;

typedef struct Point2D {
	float x;
	float y;
}Point2D;

typedef struct IntersectionPoints2D_2
{
	Point2D point1;
	Point2D point2;
	int numPoints;
}IntersectionPoints2D_2;

typedef struct IntersectionLines {
	Point2D point;
	int info; // 1: lines are parallel, 2: the lines are equal
}IntersectionLines;

typedef struct PurePersuitInfo {
	Point2D carPos;
	Point2D nextWayPoint;
	Point2D turnPoint;
	float distanceToWayPoints;
	float lookAheadDistance;
	float TrajectoryToWayPointAngle;
	float steeringAngle;
	float carLength;
	float turnRadius;
	float manouvreLength;
}PurePersuitInfo;


static LineABC xAxisABC() {
	LineABC line;
	line.Ax = 0.0f;
	line.By = 1.0f;
	line.C = 0.0f;
	return line;
}

static LineABC yAxisABC() {
	LineABC line;
	line.Ax = 1.0f;
	line.By = 0.0f;
	line.C = 0.0f;
	return line;
}

// polynomial_coefficients[0] = x^2
// polynomial_coefficients[1] = x
// polynomial_coefficients[3] = 1
// polynomial_degree = 2
static Point2D polyval(float* polynomial_coefficients, int polynomial_degree, float x) {
	float y = 0.0f;
	float x_power = 1.0f;
	Point2D result;

	for (int i = polynomial_degree; i >= 0; i--) {
		y += polynomial_coefficients[i] * x_power;
		x_power = x_power * x;
	}
	result.x = x;
	result.y = y;
	return result;
}


static int gaussianElimination3(float A[3][3 + 1], float x[3], int n) {
	int j, i, k;
	int pivot_row;
	float factor;
	float temp;
	float sum;

	// partial_pivot
	for (i = 0; i < n; i++) {
		pivot_row = i;
		for (j = i + 1; j < n; j++) {
			if (fabsf(A[j][i]) > fabsf(A[pivot_row][i])) {
				pivot_row = j;
			}
		}
		if (pivot_row != i) {
			for (j = i; j <= n; j++) {
				temp = A[i][j];
				A[i][j] = A[pivot_row][j];
				A[pivot_row][j] = temp;
			}
		}
		for (j = i + 1; j < n; j++) {
			factor = A[j][i] / A[i][i];
			for (k = i; k <= n; k++) {
				A[j][k] -= factor * A[i][k];
			}
		}
	}


	for (i = 0; i < n; i++)
	{
		sum = 0;
		for (j = 0; j < n; j++)
		{
			sum += A[i][j];
		}
		if ((sum == 0.0f) && (A[i][n] != 0.0f)) {
			memset(x, 0, sizeof(float) * n);
			return INCONSISTENT_ECUATION_SYSTEM;
		}
	}

	// back_substitute
	for (i = n - 1; i >= 0; i--) {
		sum = 0;
		for (j = i + 1; j < n; j++) {
			sum += A[i][j] * x[j];
		}
		x[i] = (A[i][n] - sum) / A[i][i];
	}
	return CONSISTENT_ECUATION_SYSTEM;
}
static int gaussianElimination2(float A[2][2 + 1], float x[2], int n) {
	int j, i, k;
	int pivot_row;
	float factor;
	float temp;
	float sum;

	// partial_pivot
	for (i = 0; i < n; i++) {
		pivot_row = i;
		for (j = i + 1; j < n; j++) {
			if (fabsf(A[j][i]) > fabsf(A[pivot_row][i])) {
				pivot_row = j;
			}
		}
		if (pivot_row != i) {
			for (j = i; j <= n; j++) {
				temp = A[i][j];
				A[i][j] = A[pivot_row][j];
				A[pivot_row][j] = temp;
			}
		}
		for (j = i + 1; j < n; j++) {
			factor = A[j][i] / A[i][i];
			for (k = i; k <= n; k++) {
				A[j][k] -= factor * A[i][k];
			}
		}
	}


	for (i = 0; i < n; i++)
	{
		sum = 0;
		for (j = 0; j < n; j++)
		{
			sum += A[i][j];
		}
		if ((sum == 0.0f) && (A[i][n] != 0.0f)) {
			memset(x, 0, sizeof(float) * n);
			return INCONSISTENT_ECUATION_SYSTEM;
		}
	}

	// back_substitute
	for (i = n - 1; i >= 0; i--) {
		sum = 0;
		for (j = i + 1; j < n; j++) {
			sum += A[i][j] * x[j];
		}
		x[i] = (A[i][n] - sum) / A[i][i];
	}
	return CONSISTENT_ECUATION_SYSTEM;
}


static ParabolaABC points2parabola_3(Point2D point1, Point2D point2, Point2D point3) {
	ParabolaABC resultParabola;
	int res;
	float A[3][3 + 1] = { {point1.x * point1.x, point1.x, 1.0f, point1.y},
						  {point2.x * point2.x, point2.x, 1.0f, point2.y},
						  {point3.x * point3.x, point3.x, 1.0f, point3.y}
	};
	float x[3];

	res = gaussianElimination3(A, x, 3);
	if (res != CONSISTENT_ECUATION_SYSTEM) {
		memset(&resultParabola, 0, sizeof(ParabolaABC));
		return resultParabola;
	}
	resultParabola.A = x[0];
	resultParabola.B = x[1];
	resultParabola.C = x[2];

	return resultParabola;
}

static int isValidParabola(ParabolaABC parabola) {
	if (parabola.A == parabola.B == parabola.C == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

static Point2D parabolaVertex(ParabolaABC parabola) {
	Point2D vertex;
	float delta;
	// (h, k) = (-b/2a, -D/4a)

	delta = (parabola.B * parabola.B) - 4.0f * (parabola.A * parabola.C);
	vertex.x = (-parabola.B) / (2.0f * parabola.A);
	vertex.y = (-delta) / (4.0f * parabola.A);
	return vertex;
}

static Point2D midPoint(Point2D point1, Point2D point2) {
	Point2D midpoint_;
	midpoint_.x = (point1.x + point2.x) / 2.0f;
	midpoint_.y = (point1.y + point2.y) / 2.0f;
	return midpoint_;
}

static LineMQ perpendicularToLinePassingThroughPointMQ(LineMQ line, Point2D point) {
	LineMQ perpendicularLine;

	perpendicularLine.m = -(1.0f / line.m);
	perpendicularLine.q = (point.x / line.m) + point.y;

	return perpendicularLine;
}


static float distanceBwParallelLinesABC(LineABC line1, LineABC line2) {
	float distance;
	distance = fabsf((-line2.C) - (-line1.C)) / sqrtf(1.0f + ((-line1.Ax) * (-line1.Ax)));
	return distance;
}

static Point2D mirrorImage(LineABC line, Point2D point)
{
	Point2D mirrorPoint_;
	float temp;

	temp = -2.0f * (line.Ax * point.x + line.By * point.y + line.C) / (line.Ax * line.Ax + line.By * line.By);
	mirrorPoint_.x = temp * line.Ax + point.x;
	mirrorPoint_.y = temp * line.By + point.y;

	return mirrorPoint_;
}

static LineABC normalizeLineABC2MQ(LineABC line) {
	if (line.By != 1.0f && line.By != 0.0f) {
		line.C = line.C / line.By;
		line.Ax = line.Ax / line.By;
		line.By = 1.0f;
		return line;
	}
	if (line.By == 0.0f && line.Ax != 0.0f)
	{
		line.C = line.C / line.Ax;
		line.Ax = 1.0f;
		return line;
	}
	return line;
}

static int arePerpenticularABC(LineABC line1, LineABC line2) {
	line2 = normalizeLineABC2MQ(line2);
	line1 = normalizeLineABC2MQ(line1);

	if (((-line1.Ax) * (-line2.Ax)) == -1.0f) {
		return 1;
	}
	else {
		return 0;
	}
}

static int areParallel(LineABC line1, LineABC line2) {
	line2 = normalizeLineABC2MQ(line2);
	line1 = normalizeLineABC2MQ(line1);
	if (line1.Ax == line2.Ax) {
		return 1;
	}
	else {
		return 0;
	}
}
/*
 * side:
 *		1: new line on right or bottom side
 *	   0: new line on left or upper side
*/
static LineABC parallelLineAtDistance(LineABC line, float distance, int side) {
	LineABC parallelLine;
	float abs_q1_minus_q2, newQ;

	newQ = 0.0f;

	line = normalizeLineABC2MQ(line);

	parallelLine = line;
	abs_q1_minus_q2 = distance * sqrtf(1.0f + ((-line.Ax) * (-line.Ax)));
	if (side > 0)
	{
		if (line.By == 0.0f)	//		|.
		{
			newQ = (-line.C) + distance;
		}
		else if ((-line.Ax) == 0.0f) //		-.
		{
			newQ = (-line.C) - distance;
		}
		else if ((-line.Ax) > 0.0f) //			/ .		
		{
			newQ = (-line.C) - abs_q1_minus_q2;
		}
		else if ((-line.Ax) < 0.0f) //		\ .		
		{
			newQ = (-line.C) + abs_q1_minus_q2;
		}
	}
	else
	{
		if (line.By == 0.0f)	//		|.
		{
			newQ = (-line.C) - distance;
		}
		else if ((-line.Ax) == 0.0f) //		-.
		{
			newQ = (-line.C) + distance;
		}
		else if ((-line.Ax) > 0.0f) //			/ .		
		{
			newQ = (-line.C) + abs_q1_minus_q2;
		}
		else if ((-line.Ax) < 0.0f) //		\ .		
		{
			newQ = (-line.C) - abs_q1_minus_q2;
		}
	}
	parallelLine.C = -newQ;
	return parallelLine;
}

static int isLineParallelToXaxis(LineABC line) {
	if (line.Ax == 0.0f && line.By != 0.0f)
	{
		return 1;
	}
	else {
		return 0;
	}
}

static int isLineParallelToYaxis(LineABC line) {
	if (line.By == 0.0f && line.Ax != 0.0f)
	{
		return 1;
	}
	else {
		return 0;
	}
}

static float angleBetweenLines(LineMQ line1, LineMQ line2) {
	float angle;
	angle = atanf(fabsf((line1.m - line2.m)) / (1.0f + (line1.m * line2.m)));
	return angle;
}

// https://www.math-only-math.com/equations-of-the-bisectors-of-the-angles-between-two-straight-lines.html
//acutangle is the bisector when the lines are parallel
static void bisectorsOfTwoLines(LineABC line1, LineABC line2, LineABC* acuteAngle, LineABC* ottuseAngle) {
	float a1, a2, b1, b2, c1, c2;
	float aa1, aa2, bb1, bb2, cc1, cc2;
	float leftDenominator, rightDenominator;
	float gg;

	a1 = line1.Ax;
	b1 = line1.By;
	c1 = line1.C;
	a2 = line2.Ax;
	b2 = line2.By;
	c2 = line2.C;

	if (c1 < 0.0f)
	{
		a1 = -a1;
		b1 = -b1;
		c1 = -c1;
	}
	if (c2 < 0.0f)
	{
		a2 = -a2;
		b2 = -b2;
		c2 = -c2;
	}

	leftDenominator = sqrtf((a1 * a1) + (b1 * b1));
	rightDenominator = sqrtf((a2 * a2) + (b2 * b2));

	// +
	aa1 = (rightDenominator * a1) - (leftDenominator * a2);
	bb1 = (rightDenominator * b1) - (leftDenominator * b2);
	cc1 = (rightDenominator * c1) - (leftDenominator * c2);

	// -
	aa2 = (rightDenominator * a1) + (leftDenominator * a2);
	bb2 = (rightDenominator * b1) + (leftDenominator * b2);
	cc2 = (rightDenominator * c1) + (leftDenominator * c2);

	gg = (a1 * a2) + (b1 * b2);

	if (gg >= 0.0f)
	{
		if (ottuseAngle)
		{
			ottuseAngle->Ax = aa1;
			ottuseAngle->By = bb1;
			ottuseAngle->C = cc1;
		}
		if (acuteAngle)
		{
			acuteAngle->Ax = aa2;
			acuteAngle->By = bb2;
			acuteAngle->C = cc2;
		}
	}
	else if (gg < 0.0f) {
		if (acuteAngle)
		{
			acuteAngle->Ax = aa1;
			acuteAngle->By = bb1;
			acuteAngle->C = cc1;
		}
		if (ottuseAngle)
		{
			ottuseAngle->Ax = aa2;
			ottuseAngle->By = bb2;
			ottuseAngle->C = cc2;
		}
	}
	if (ottuseAngle) {
		*ottuseAngle = normalizeLineABC2MQ(*ottuseAngle);
	}
	if (acuteAngle) {
		*acuteAngle = normalizeLineABC2MQ(*acuteAngle);
	}
}

static LineMQ points2line(Point2D point1, Point2D point2) {
	LineMQ line;
	line.m = (point1.y - point2.y) / (point1.x - point2.x);
	line.q = (line.m * (-point1.x)) + point1.y;
	return line;
}

static LineABC lineMQ2ABC(LineMQ line) {
	LineABC lineAbc;
	lineAbc.Ax = -line.m;
	lineAbc.By = 1.0f;
	lineAbc.C = -line.q;
	return lineAbc;
}

static LineMQ lineABC2MQ(LineABC line) {
	LineMQ lineMq;
	line = normalizeLineABC2MQ(line);
	lineMq.m = -line.Ax;
	lineMq.q = -line.C;
	return lineMq;
}

static LineABC perpendicularToLinePassingThroughPointABC(LineABC line, Point2D point) {
	LineABC perpendicularLine;
	LineMQ lineMq;

	line = normalizeLineABC2MQ(line);

	if (isLineParallelToYaxis(line)) {
		perpendicularLine = xAxisABC();
		perpendicularLine.C = -point.y;
	}
	else if (isLineParallelToXaxis(line)) {
		perpendicularLine = yAxisABC();
		perpendicularLine.C = -point.x;
	}
	else
	{
		lineMq = lineABC2MQ(line);
		lineMq = perpendicularToLinePassingThroughPointMQ(lineMq, point);
		perpendicularLine = lineMQ2ABC(lineMq);
	}


	return perpendicularLine;
}

static float angleBetweenLinesABC(LineABC line1, LineABC line2) {
	float angle;
	LineMQ line1Mq, line2Mq;

	if (isLineParallelToXaxis(line1) && isLineParallelToXaxis(line2)) {
		return 0.0f;	/* line1 // line2 // Y */
	}

	if (isLineParallelToYaxis(line1))
	{
		if (isLineParallelToXaxis(line2)) {
			return M_PI_2;
		}
		else if (isLineParallelToYaxis(line2)) {
			return 0.0f;	/* line1 // line2 // Y */
		}
		line1Mq = lineABC2MQ(line2);
		line2Mq.m = 0;
		line2Mq.q = 0;
		angle = M_PI_2 - angleBetweenLines(line1Mq, line2Mq);
	}
	else if (isLineParallelToYaxis(line2)) {
		if (isLineParallelToXaxis(line1)) {
			return M_PI_2;
		}
		else if (isLineParallelToYaxis(line1)) {
			return 0.0f;	/* line1 // line2 // Y */
		}
		line1Mq = lineABC2MQ(line1);
		line2Mq.m = 0;
		line2Mq.q = 0;
		angle = M_PI_2 - angleBetweenLines(line1Mq, line2Mq);
	}
	else {
		line1Mq = lineABC2MQ(line1);
		line2Mq = lineABC2MQ(line2);
		angle = angleBetweenLines(line1Mq, line2Mq);
	}

	return angle;
}

static LineABC points2lineABC(Point2D point1, Point2D point2) {
	LineMQ lineMq;
	LineABC lineAbc;
	if (point1.x == point2.x) { // perpendicular to y axis
		lineAbc = yAxisABC();
		lineAbc.C = -point1.x;
		return lineAbc;
	}
	lineMq = points2line(point1, point2);
	lineAbc = lineMQ2ABC(lineMq);
	return lineAbc;
}

static float euclidianDistance(Point2D point1, Point2D point2) {
	float distance;
	distance = sqrtf(((point2.x - point1.x) * (point2.x - point1.x)) + ((point2.y - point1.y) * (point2.y - point1.y)));
	return distance;
}

static float distance2line(Point2D point, LineMQ line) {
	float distance;
	distance = fabsf((line.m * point.x) + (-1.0f * point.y) + line.q) / sqrtf((line.m * line.m) + 1.0f);
	return distance;
}

static float distance2lineABC(Point2D point, LineABC lineAbc) {
	float distance;
	LineMQ line;
	Point2D point2Temp;
	if (lineAbc.By == 0.0f)
	{
		lineAbc = normalizeLineABC2MQ(lineAbc);
		point2Temp.y = point.y;
		point2Temp.x = (-lineAbc.C) / lineAbc.Ax;
		return euclidianDistance(point, point2Temp);
	}
	line = lineABC2MQ(lineAbc);
	distance = distance2line(point, line);
	return distance;
}

static IntersectionPoints2D_2 intersectionLineCircle(Point2D circleCenter, float circleRadius, LineMQ line) {
	IntersectionPoints2D_2 points;
	float a, b, c, delta;

	memset(&points, 0, sizeof(points));

	// ax^2 + bx + c = 0
	a = 1.0f + (line.m * line.m);
	b = (2.0f * line.m * line.q) + ((-2.0f * circleCenter.x) + ((-2.0f * circleCenter.y * line.m)));
	c = ((-2.0f) * circleCenter.y * line.q) + (line.q * line.q) + (circleCenter.x * circleCenter.x) + (circleCenter.y * circleCenter.y) - (circleRadius * circleRadius);

	delta = b * b + ((-4.0f) * a * c);

	if (delta < 0) {
		points.numPoints = 0;
	}
	else if (delta == 0) {
		points.numPoints = 1;
	}
	else {
		points.numPoints = 2;
	}

	points.point1.x = (-b + sqrtf(delta)) / (2.0f * a);
	points.point2.x = (-b - sqrtf(delta)) / (2.0f * a);
	points.point1.y = line.m * points.point1.x + line.q;
	points.point2.y = line.m * points.point2.x + line.q;

	return points;
}

static IntersectionPoints2D_2 intersectionLineCircleABC(Point2D circleCenter, float circleRadius, LineABC lineAbc) {
	IntersectionPoints2D_2 points;
	LineMQ line;
	float a, b, c, x_, delta;

	x_ = 0.0f;
	memset(&line, 0, sizeof(line));
	memset(&points, 0, sizeof(points));

	lineAbc = normalizeLineABC2MQ(lineAbc);

	if (!isLineParallelToYaxis(lineAbc))
	{
		line = lineABC2MQ(lineAbc);
		// ax^2 + bx + c = 0
		a = 1.0f + (line.m * line.m);
		b = (2.0f * line.m * line.q) + ((-2.0f * circleCenter.x) + ((-2.0f * circleCenter.y * line.m)));
		c = ((-2.0f) * circleCenter.y * line.q) + (line.q * line.q) + (circleCenter.x * circleCenter.x) + (circleCenter.y * circleCenter.y) - (circleRadius * circleRadius);
	}
	else {
		x_ = (-lineAbc.C) / lineAbc.Ax;
		a = 1.0f;
		b = -2.0f * circleCenter.y;
		c = (x_ * x_) + (-2.0f * circleCenter.x * x_) + (circleCenter.x * circleCenter.x) - (circleRadius * circleRadius);
	}


	delta = b * b + ((-4.0f) * a * c);

	if (delta < 0) {
		points.numPoints = 0;
		return points;
	}
	else if (delta == 0) {
		points.numPoints = 1;
	}
	else {
		points.numPoints = 2;
	}


	if (!isLineParallelToYaxis(lineAbc))
	{
		points.point1.x = (-b + sqrtf(delta)) / (2.0f * a);
		points.point2.x = (-b - sqrtf(delta)) / (2.0f * a);
		points.point1.y = line.m * points.point1.x + line.q;
		points.point2.y = line.m * points.point2.x + line.q;
	}
	else {
		points.point1.y = (-b + sqrtf(delta)) / (2.0f * a);
		points.point2.y = (-b - sqrtf(delta)) / (2.0f * a);
		points.point1.x = x_;
		points.point2.x = x_;
	}

	return points;
}

static IntersectionLines intersectionLinesABC(LineABC line1, LineABC line2) {
	IntersectionLines inters;

	memset(&inters, 0, sizeof(inters));

	if ((line1.Ax * line2.By - line2.Ax * line1.By) == 0.0f) {
		line2 = normalizeLineABC2MQ(line2);
		line1 = normalizeLineABC2MQ(line1);
		if (memcmp(&line1, &line2, sizeof(inters) == 0))
		{
			inters.info = 2;
		}
		else {
			inters.info = 1;
		}
		return inters;
	}

	inters.point.x = (line1.By * line2.C - line2.By * line1.C) / (line1.Ax * line2.By - line2.Ax * line1.By);
	inters.point.y = (line1.C * line2.Ax - line2.C * line1.Ax) / (line1.Ax * line2.By - line2.Ax * line1.By);

	return inters;
}

static float triangleAngleA(float AC, float CB, float BA) {
	float angle;
	angle = acosf(((AC * AC) + (BA * BA) - (CB * CB)) / (2.0f * AC * BA));
	return angle;
}

static float carTrajectoryAndWayPointAngle(Point2D carPos, Point2D nextWayPoint) {
	float lookAheadDistance, TrajectoryToWayPointAngle;
	Point2D temp;

	lookAheadDistance = euclidianDistance(carPos, nextWayPoint);
	temp = carPos;
	temp.y += lookAheadDistance;
	TrajectoryToWayPointAngle = triangleAngleA(lookAheadDistance, euclidianDistance(nextWayPoint, temp), lookAheadDistance);
	if (carPos.x < nextWayPoint.x) {
		TrajectoryToWayPointAngle = -TrajectoryToWayPointAngle;
	}
	return TrajectoryToWayPointAngle;
}

static float steeringWheelAngle(float TrajectoryToWayPointAngle, float carLength, float nextWayPointDistance) {
	float angle;
	angle = atanf((2.0f * carLength * sinf(TrajectoryToWayPointAngle)) / nextWayPointDistance);
	return angle;
}

static float turnRadius(float TrajectoryToWayPointAngle, float carLength, float nextWayPointDistance) {
	float angle;
	angle = (nextWayPointDistance / (2.0f * sinf(TrajectoryToWayPointAngle)));
	return angle;
}

static float purePursuitComputeSteeringWheelAngle(Point2D carPos, LineMQ wayPoints, float carLength, float lookAheadDistance) {
	float temp;
	IntersectionPoints2D_2 intersectionPoints;
	Point2D nextWayPoint;

	temp = distance2line(carPos, wayPoints);
	if (temp >= lookAheadDistance) {
		lookAheadDistance = temp + (temp * 0.25f);
	}

	intersectionPoints = intersectionLineCircle(carPos, lookAheadDistance, wayPoints);
	if (intersectionPoints.point1.y > intersectionPoints.point2.y) {
		nextWayPoint = intersectionPoints.point1;
	}
	else {
		nextWayPoint = intersectionPoints.point2;
	}

	temp = carTrajectoryAndWayPointAngle(carPos, nextWayPoint);
	return steeringWheelAngle(temp, carLength, lookAheadDistance);
}

static PurePersuitInfo purePursuitCompute(Point2D carPos, LineMQ wayPoints, float carLength, float lookAheadDistance) {
	float temp;
	PurePersuitInfo info;
	IntersectionPoints2D_2 intersectionPoints;
	Point2D nextWayPoint;

	temp = distance2line(carPos, wayPoints);
	info.distanceToWayPoints = temp;
	if (temp >= lookAheadDistance) {
		lookAheadDistance = temp + (temp * 0.25f);
	}

	intersectionPoints = intersectionLineCircle(carPos, lookAheadDistance, wayPoints);
	if (intersectionPoints.point1.y > intersectionPoints.point2.y) {
		nextWayPoint = intersectionPoints.point1;
	}
	else {
		nextWayPoint = intersectionPoints.point2;
	}
	info.TrajectoryToWayPointAngle = carTrajectoryAndWayPointAngle(carPos, nextWayPoint);
	info.steeringAngle = steeringWheelAngle(info.TrajectoryToWayPointAngle, carLength, lookAheadDistance);

	info.carPos = carPos;
	info.nextWayPoint = nextWayPoint;
	info.lookAheadDistance = lookAheadDistance;
	info.carLength = carLength;
	info.turnRadius = turnRadius(info.TrajectoryToWayPointAngle, carLength, lookAheadDistance);
	info.manouvreLength = fabsf(((2.0f * M_PI * info.turnRadius) * info.TrajectoryToWayPointAngle) / (2.0f * M_PI));
	info.turnPoint = carPos;
	if (info.TrajectoryToWayPointAngle < 0) {
		info.turnPoint.x += info.turnRadius;
	}
	else {
		info.turnPoint.x -= info.turnRadius;
	}

	return info;
}

static PurePersuitInfo purePursuitComputeABC(Point2D carPos, LineABC wayPoints, float carLength, float lookAheadDistance) {
	float temp;
	PurePersuitInfo info;
	IntersectionPoints2D_2 intersectionPoints;
	Point2D nextWayPoint;

	temp = distance2lineABC(carPos, wayPoints);
	info.distanceToWayPoints = temp;
	if (temp >= lookAheadDistance) {
		lookAheadDistance = temp + (temp * 0.1f);
	}

	intersectionPoints = intersectionLineCircleABC(carPos, lookAheadDistance, wayPoints);
	if (intersectionPoints.point1.y > intersectionPoints.point2.y) {
		nextWayPoint = intersectionPoints.point1;
	}
	else {
		nextWayPoint = intersectionPoints.point2;
	}
	info.TrajectoryToWayPointAngle = carTrajectoryAndWayPointAngle(carPos, nextWayPoint);
	info.steeringAngle = steeringWheelAngle(info.TrajectoryToWayPointAngle, carLength, lookAheadDistance);

	info.carPos = carPos;
	info.nextWayPoint = nextWayPoint;
	info.lookAheadDistance = lookAheadDistance;
	info.carLength = carLength;
	info.turnRadius = turnRadius(info.TrajectoryToWayPointAngle, carLength, lookAheadDistance);
	info.manouvreLength = fabsf(((2.0f * M_PI * info.turnRadius) * info.TrajectoryToWayPointAngle) / (2.0f * M_PI));
	info.turnPoint = carPos;
	if (info.TrajectoryToWayPointAngle < 0) {
		info.turnPoint.x += info.turnRadius;
	}
	else {
		info.turnPoint.x -= info.turnRadius;
	}

	return info;
}


#endif // !__PUREPURSUITGEOMETRY_H__
