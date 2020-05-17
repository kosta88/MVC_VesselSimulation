/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#include "Freighter.h"
/***********************************************************************************/

Freighter::Freighter(string name,Point loc,int pow,int cap)
:Vessel(name,loc,500000,1000,pow,freighter),containerNum(0),capacity(cap),toDo(none)
{

}

/***********************************************************************************/

Freighter::~Freighter()
{
	for(auto it : missions)
		it.second.clear();
	missions.clear();
}

/***********************************************************************************/

void Freighter::isUnderAttack()
{
	current_state=Stopped;
	missions.clear();
	data.setCourseDirection(0);
	data.setSpeed(0);
	toDo = none;
}

/***********************************************************************************/

void Freighter::execMission()
{
	auto it = missions.find(destination);
	if(it == missions.end())
		return;
	vector<pair<string,int>>& tasks = it->second;
	if(tasks.size() == 0)
		return;

	if(tasks[0].first.compare("load") == 0)
		containerNum = capacity;
	else if(tasks[0].first.compare("unload") == 0){
		if(containerNum< tasks[0].second)
		{
			containerNum= 0;
			cerr<<"Insufficient number of containers"<<endl;
		}
		else containerNum -= tasks[0].second;
	}

	tasks.erase(tasks.begin());
}

/***********************************************************************************/

void Freighter::printStatus()
{
	bool move = false;
	cout<<"Freighter "<<name<<" at ";
	loc.print();
	cout<<", fuel: "<<setprecision(1)<<fuelTank<<" kl, power: "<<Power<<", ";
	switch(current_state)
	{
	case Stopped:
		cout<<"Stopped ";
		break;
	case Dead:
		cout<<"Dead in the water ";
		break;
	case Docked:
		cout<<"Docked at "<<destination<<" ";
		break;
	case MovingCourse:
		cout<<"Moving";
		move = true;
		break;
	case MovingPort:
		cout<<"Moving to "<<destination<<" ";
		move = true;
		break;
	case MovingPosition:
		cout<<"Moving to ";
		destination_point.print();
		move = true;
		break;
	}
	cout<<"on course "<<data.getCourseDirection()<<" deg, speed "<<
			data.getSpeed()<<" nm/hr, Containers: "<<containerNum<<endl;
	if(!move) return;


	switch(toDo)
	{
	case load:
		cout<<"moving to loading destination"<<endl;
		break;
	case unload:
		cout<<"moving to unloading destination"<<endl;
		break;
	case none:
		cout<<"no cargo destinations"<<endl;
		break;

	}
	return;
}

/***********************************************************************************/

void Freighter::setOrderByPort(string port)
{
	auto it = missions.find(port);
	if(it == missions.end() || it->second.size() == 0)
	{
		toDo = none;
		return;
	}

	auto task = it->second[0];

	if(task.first.compare("unload") == 0)
		toDo = unload;

	else if(task.first.compare("load") == 0)
		toDo = load;
}

/***********************************************************************************/

void Freighter::addTask(string dest,string task,int amount)
{
	auto find = missions.find(dest);
	if(find == missions.end())
	{
		vector<pair<string,int>> vect;
		vect.push_back(pair<string,int>(task,amount));
		missions.insert(pair<string,vector<pair<string,int>>>(dest,vect));
	}
	else
		find->second.push_back(pair<string,int>(task,amount));
}

/***********************************************************************************/

