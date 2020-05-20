/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#ifndef CRUISER_H_
#define CRUISER_H_

#include "Freighter.h"
#include "Vessel.h"

class Cruiser: public Vessel {
private:
	double range;

public:
	Cruiser(string name="",Point loc=Point(0,0),int pow = 0,double atkRange = 10);
	~Cruiser();

	void isUnderAttack(){ };

	void attack(shared_ptr<Vessel> toBatacked);
    void update();
    void printStatus();

	/*************     GETTERS   ---   SETTERS     ****************/
	double getAttackRange(){return range;}

};

#endif /* CRUISER_H_ */
