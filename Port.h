/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#ifndef PORT_H_
#define PORT_H_
#include "AbstractObject.h"

#include <queue>

class Port : public AbstractObj{
private:
	double fuelSupply;
	double ProductionRate;
	queue<string> RefulingQ;

public:
	Port(string N_ame="",Point loc=Point(0,0),int fuel=0,int rate=0)
		:AbstractObj(N_ame,loc),fuelSupply(fuel),ProductionRate(rate) {}

	virtual ~Port() { RefulingQ = queue<string>{}; }

	/*************     GETTERS   ---   SETTERS     ****************/

    void addToRefulingQ(string ship) { RefulingQ.push(ship); }
    string getRefulingQHead() const { return RefulingQ.front(); }
       void popFromRefulingQ() { RefulingQ.pop(); }
    bool queueIsEmpty() const { return RefulingQ.empty(); }
    void increaseFuelSupply() { fuelSupply += ProductionRate; }
    double getFuelSupply() const { return fuelSupply; }
    void setFuelSupply(double supply){fuelSupply =supply;}

};
#endif /* PORT_H_ */
