/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#ifndef ABSTRACTOBJECT_H_
#define ABSTRACTOBJECT_H_

#include <iostream>

#include "Calculations.h"


using namespace std;

class AbstractObj {
protected:
	string name;
	Point loc;
public:
	AbstractObj(string N_ame,Point location) : name(N_ame),loc(location) { };
	virtual ~AbstractObj( ){ };

	/*************     GETTERS   ---   SETTERS     ****************/

	string getName() const {return name;}
	Point& getLocation() {return loc;}
};

#endif /* ABSTRACTOBJECT_H_ */
