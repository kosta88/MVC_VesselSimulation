/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#ifndef FREIGHTER_H_
#define FREIGHTER_H_
#include <vector>
#include <map>
#include "Vessel.h"
typedef enum {none=0,load,unload } FreighterInstructions;

class Freighter : public Vessel {
private:
	map <string, vector< pair<string,int> >  > missions;
	int containerNum;
	int capacity;
	FreighterInstructions toDo;

public:
	Freighter(string name="",Point loc=Point(0,0),int pow=0,int cap=0);
	~Freighter();

	void isUnderAttack();
	void execMission();
	void addTask(string,string,int);
	void printStatus();
	void clearTasks(){missions.clear();}

	/*************     GETTERS   ---   SETTERS     ****************/
	void setOrderByPort(string);
	void setContainerNum(int Num){ containerNum=Num;}
	int getContainerNum() { return containerNum; }
	FreighterInstructions getTODO() { return toDo; }
	void setTODO(FreighterInstructions newJob) { toDo = newJob; }

};

#endif /* FREIGHTER_H_ */
