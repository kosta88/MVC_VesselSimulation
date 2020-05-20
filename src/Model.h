/*
 *  Created on: June 19, 2018
 *      Author: kosta
 */
#ifndef MODEL_H_
#define MODEL_H_

#define MaxNameLength 12
#define MAX_ARGUMENTS_NUM 7
#define MIN_SIZE 6
#define MAX_SIZE 30

#include "Port.h"
#include "AllExceptions.h"
#include "AbstractFactory.h"
#include "VesselFactory.h"
#include <sstream>
#include <map>
#include <limits>
#include <iterator>

typedef enum {GO=0,DEFAULT,SIZE,ZOOM,PAN,SHOW,STATUS,CREATE,COURSE,POSITION,DESTINATION,LOAD_AT,
	UNLOAD_AT,DOCK_AT,ATTACK,REFUEL,STOP,EXIT,IMMEDIATE,UNIMMEDIATE,ERROR} cmd;

	class Model {
	private:
		shared_ptr<AbstractFactory> factory;
		map<string,shared_ptr<Port>> allPorts;
		map<string,shared_ptr<Vessel>> allVessels;
		int time;

		static Model* instance;
		Model();
		friend class SingletonDtor;

	public:
		static Model& getInstance();
		Model(const Model&) = delete;
		Model(const Model&&) = delete;
		Model& operator=(const Model&) = delete;
		Model& operator=(const Model&&) = delete;
		~Model();


		map<string,shared_ptr<Port>>& getPortList() { return allPorts; }
		double distance(Point,Point);
		shared_ptr<AbstractFactory> getFactory() { return factory; }

		vesselType stringToShipType(string) const;
		Point stringToPoint(string);


		void update(vector<string>&);
		void add_Vessel(shared_ptr<Vessel>);
		void add_port(shared_ptr<Port>);
		void addTaskToFreighter(string,string,string,string);
		bool inPortRange(shared_ptr<Vessel>);
		bool inPortList(string name) const;
		bool pointCheck(string name) const;
		bool speedCheck(int,string);
		void portTimeUpdate();
		void status();
		void addPortToPath(shared_ptr<Patrol_boat>);
		void VesselTimeUpdate();
		void commandInputCheck(string line) noexcept(false);
		bool inList(string name) const;
		string inScale(int,int,int) const;

		/*************     GETTERS  --STRING ACTIONS-- SETTERS     ****************/
		shared_ptr<Vessel> getVessel(string);
		shared_ptr<Port> getPort(string);
		int getTime() const {return time;}
		cmd commandFromString(string);
		vector<string> splitString(string);
	};

#endif /* MODEL_H_ */
