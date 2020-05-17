/*
 *  Created on: June 22, 2018
 *      Author: kosta
 */
#include "Calculations.h"

#include <ctgmath>
#include <cmath>

const double pi = 2. * atan2(1., 0.);

/**********************************************************************/
double to_degrees(double theta_r)
{
	return theta_r * 180.0 / pi;
}

/**********************************************************************/
double to_radians(double theta_d)
{
	return theta_d * pi / 180.0;
}

/**********************************************************************/

void Cartesian_vector::operator=(const Polar_vector& pv)
{
	delta_x = pv.r * cos(pv.theta);
	delta_y = pv.r * sin(pv.theta);
}

/**********************************************************************/

Cartesian_vector::Cartesian_vector(const Polar_vector& pv)
{
	delta_x = pv.r * cos(pv.theta);
	delta_y = pv.r * sin(pv.theta);
}


/**********************************************************************/

Polar_vector::Polar_vector(const Cartesian_vector& cv) {
	r = sqrt((cv.delta_x * cv.delta_x) + (cv.delta_y * cv.delta_y));

	theta = atan2(cv.delta_y, cv.delta_x);
	if (theta < 0.)
		theta = 2. * pi + theta;
}

/**********************************************************************/

void Polar_vector::operator=(const Cartesian_vector& cv)
{
	r = sqrt((cv.delta_x * cv.delta_x) + (cv.delta_y * cv.delta_y));

	theta = atan2(cv.delta_y, cv.delta_x);
	if (theta < 0.)
		theta = 2. * pi + theta;
}

/**********************************************************************/

void Point::print() const
{
	cout << setprecision(2) << "(" << x << ", " << y << ")";
}

/**********************************************************************/

bool Point::operator==(const Point & rhs)
{
	return x == rhs.x && y == rhs.y;
}

