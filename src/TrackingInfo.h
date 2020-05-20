/*
 *  Created on: June 20, 2018
 *      Author: kosta
 */
#ifndef TRACKINGINFO_H_
#define TRACKINGINFO_H_
class TrackingInfo {

private:
	double courseOfDirection;
	double speed;

public:
	TrackingInfo():courseOfDirection(0), speed(0){}
	~TrackingInfo(){}

	/*************     GETTERS   ---   SETTERS     ****************/
	double getCourseDirection() const {return courseOfDirection;}
	void setCourseDirection(double newCourse) {courseOfDirection = newCourse; }

	double getSpeed() const {return speed;}
	void setSpeed(double s) { speed = s; }
};

#endif /* TRACKINGINFO_H_ */
