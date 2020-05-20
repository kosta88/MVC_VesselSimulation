/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#ifndef ABSTRACTFACTORY_H_
#define ABSTRACTFACTORY_H_

#include "Vessel.h"

class AbstractFactory {
public:

	virtual shared_ptr<Vessel> makeShip(string,vesselType,Point,int,int) = 0;
	virtual ~AbstractFactory(){}

};

#endif /* ABSTRACTFACTORY_H_ */
