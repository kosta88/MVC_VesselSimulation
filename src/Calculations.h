/*
 *  Created on: June 22, 2018
 *      Author: kosta
 */
#ifndef CALCULATIONS_H_
#define CALCULATIONS_H_

#include <iomanip>
#include <iostream>

using namespace std;

double to_radians(double theta_d);
double to_degrees(double theta_r);


typedef struct Point {
	double x;
	double y;
	Point(double x, double y): x(x), y(y)	{	};
	Point() {   x = 0.0;		y = 0.0;	}
	void print() const;
	bool operator==(const Point& rhs);
} Point;



typedef struct Cartesian_vector {
	Cartesian_vector(const struct Polar_vector & pv);
	Cartesian_vector()	{	delta_x = 0.0;	delta_y = 0.0;	}
	void operator=(const struct Polar_vector & pv);
	double delta_x;
	double delta_y;
} Cartesian_vector;



typedef struct Polar_vector {
	Polar_vector(const Cartesian_vector & cv);
	Polar_vector(){		r = 0.0;	theta = 0.0;	};
	void operator=(const Cartesian_vector & cv);
	double r;
	double theta;
} Polar_vector;



#endif /* CALCULATIONS_H_ */
