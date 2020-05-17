/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#ifndef VESSEL_H_
#define VESSEL_H_

#include "Port.h"
#include "AbstractObject.h"
#include "TrackingInfo.h"
#include <cmath>
#include <memory>

#define MaxFreighterMph 40
#define MaxCruiserMph 75
#define MaxPatrolMph 15
#define MaxFreighterTank 500000
#define MaxPatrolTank 900000

typedef enum{Stopped=0,Docked,Dead,MovingCourse,MovingPort ,MovingPosition} vesselState;
typedef enum{freighter=0,patrol,cruiser} vesselType;

class Vessel: public AbstractObj {
protected:
	int Power;
	int dockedPeriod;
	double fuelTank;
	double fuelConsumption;

	vesselState current_state;
	string destination;
	vesselType type;
	Point destination_point;   //next location
	TrackingInfo data;

public:
	Vessel(string in_name="",Point loc=Point(0,0),double tank=0,double fuelIntake=0,
			int Power = 0,vesselType VT=cruiser);

	virtual ~Vessel() {};
	void advance();
	void stopShipping();

	virtual void isUnderAttack() = 0;

	/*************     GETTERS   ---   SETTERS     ****************/

	double getFuelTank() const {return fuelTank;}
	void setFuelTank(double tank){ fuelTank=tank;}
	void setPower(int p){Power=p;}
	int getPower() const {return Power;}

	void setDestination(string dest) { destination = dest; }
	string getDestination() const {return destination;}
	vesselType getVesselType() const {return type;}

	void setState(vesselState new_state) { current_state = new_state; }
	vesselState getState() const { return current_state; }
	void setLocation(Point pos) { this->loc = pos; }

	void setDockedPeriod(int val) { dockedPeriod = val; }
	int getDockedPeriod() const { return dockedPeriod; }
	TrackingInfo& getData() { return data; }

	void setDestinationPoint(Point p){destination_point=p;}
	Point getDestinationPoint() const {return destination_point;}
};

#endif /* VESSEL_H_ */
