/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#ifndef PATROLBOAT_H_
#define PATROLBOAT_H_

#include <set>
#include "float.h"
#include "Vessel.h"


class Patrol_boat: public Vessel {
private:
	string startPort;
	double startSpeed;
	bool arrived;
	set<string> visited;

public:
	Patrol_boat(string name="",Point loc=Point(0,0),int power=0);
	~Patrol_boat();
	void printStatus();
	void isUnderAttack();


	bool alreadyVisited(string) const;

	void addToSet(string p_name) {visited.insert(p_name); }
	void clearSet() { visited.clear(); }

	/*************     GETTERS   ---   SETTERS     ****************/
	string getPort() const { return startPort; }
	void setPort(string home) { startPort= home; }
	double getSpeed() const { return startSpeed; }
	void setSpeed(double speed) { startSpeed = speed; }
	bool getArrived() const { return arrived; }
	void setArrived(bool flag) { arrived = flag; }
};
#endif /* PATROLBOAT_H_ */
