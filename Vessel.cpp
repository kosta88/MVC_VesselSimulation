/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#include "Vessel.h"

/************************************************************************************/

Vessel::Vessel(string name,Point loc,double tank,double fuelIntake,
		int Pow,vesselType VT)
		:AbstractObj(name,loc),Power(Pow),dockedPeriod(0),fuelTank(tank),
		 fuelConsumption(fuelIntake),
		 current_state(Stopped),destination(""),type(VT),
		 data(TrackingInfo())
		{

		}

/************************************************************************************/

void Vessel::advance()
{
	double speed=data.getSpeed();

	if(fuelTank - fuelConsumption < 0)
	{
		speed *= (fuelTank/fuelConsumption);
		current_state=Dead;
		fuelTank=0;
		data.setCourseDirection(0);
		data.setSpeed(0);
	}
	else
		fuelTank -= fuelConsumption;

	Polar_vector Pvec;
	Pvec.r = speed;
	Pvec.theta = to_radians(data.getCourseDirection());
	Cartesian_vector Cvec(Pvec);

	loc.x+=Cvec.delta_x;
	loc.y+=Cvec.delta_y;
}

/************************************************************************************/

void Vessel::stopShipping()
{
	current_state=Stopped;
	data.setCourseDirection(0);
	data.setSpeed(0);

	destination="";
	destination_point=Point(0,0);


}


/************************************************************************************/

