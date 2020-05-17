/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#ifndef VESSELFACTORY_H_
#define VESSELFACTORY_H_

#include "AbstractFactory.h"
#include "Cruiser.h"
#include "Freighter.h"
#include "Patrolboat.h"

class VesselFactory: public AbstractFactory {
public:
	VesselFactory(){ };
	virtual ~VesselFactory(){};

	shared_ptr<Vessel> makeShip(string Name,vesselType VT,Point pos,int pow,int arg){
		switch(VT)
		{
		case freighter:
			return make_shared<Freighter>(Name,pos,pow,arg);
		case cruiser:
			return make_shared<Cruiser>(Name,pos,pow,arg);
		case patrol:
			return make_shared<Patrol_boat>(Name,pos,pow);
		default:
			return nullptr;
		}
	}
};

#endif /* VESSELFACTORY_H_ */
