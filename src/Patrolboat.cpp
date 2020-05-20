/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#include "Patrolboat.h"

/****************************************************************************************/

Patrol_boat::Patrol_boat(string name,Point loc,int power)
:Vessel(name,loc,900000,2000,power,patrol),startSpeed(0),arrived(false)
{

}

/****************************************************************************************/

Patrol_boat::~Patrol_boat()
{
	visited.clear();
}

/****************************************************************************************/

bool Patrol_boat::alreadyVisited(string name) const
{
	if(visited.find(name) != visited.end())
		return true;

	return false;
}

/****************************************************************************************/

void Patrol_boat::isUnderAttack()
{
	current_state=Stopped;
	visited.clear();
	data.setCourseDirection(0);
	data.setSpeed(0);
}

/****************************************************************************************/

void Patrol_boat::printStatus()
{
	cout<<"Patrol_boat "<<name<<" at ";
	loc.print();
	cout<<", fuel: "<<setprecision(1)<<fuelTank<<" kl, power: "<<Power;
	if(current_state==Stopped)
		cout<<", Stopped, ";

	else if(current_state == Dead)
		cout<<", Dead in the water ";

	else if( current_state==Docked)
		cout<<", docked at "<<destination;

	else cout<<", Moving to "<<destination;
	cout<<" on course "<<data.getCourseDirection()<<
			" deg, speed "<<data.getSpeed()<<" nm/hr"<<endl;

}

/****************************************************************************************/
