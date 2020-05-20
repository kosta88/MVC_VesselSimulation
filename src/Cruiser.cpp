/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#include "Cruiser.h"
/************************************************************************************/
Cruiser::Cruiser(string name,Point loc,int pow,double atkRange )
:Vessel(name,loc,-1,-1,pow,cruiser),range(atkRange)
{

}

/************************************************************************************/

Cruiser::~Cruiser()
{

}

/************************************************************************************/

void Cruiser::attack(shared_ptr<Vessel>  attacked)
{
	if(attacked->getVesselType()==cruiser)
		return;

	attacked->isUnderAttack();

	if(Power > attacked->getPower())
	{
		Power++;
		attacked->setPower(attacked->getPower()-1);
		if(attacked->getVesselType()==freighter)
		{
			shared_ptr<Freighter> temp = dynamic_pointer_cast<Freighter>(attacked);
			temp->setContainerNum(0);
		}
	}
	else {
		Power--;
		attacked->setPower(attacked->getPower()+1);
	}
}

/************************************************************************************/

void Cruiser::printStatus()
{
	cout<<"Cruiser "<<name<<" at ";
	loc.print();
	cout<<", power: "<<Power<<", moving on course "<<data.getCourseDirection()
			<<" deg, speed "<<data.getSpeed()<<" nm/hr"<<endl;
}

/************************************************************************************/

void Cruiser::update(){
	Polar_vector pv;
	pv.r = data.getSpeed();
	pv.theta = to_radians(data.getCourseDirection());
	Cartesian_vector cv(pv);

	loc.x+=cv.delta_x;
	loc.y+=cv.delta_y;
}

/************************************************************************************/

