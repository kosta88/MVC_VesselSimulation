/*
 *  Created on: June 19, 2018
 *      Author: kosta
 */

#include "Model.h"

/***************************************************************************************/

Model* Model::instance = nullptr;

/***************************************************************************************/
Model::Model(): time(0)
{
	factory = make_shared<VesselFactory>();
	add_port(make_shared<Port>("Nagoya",Point(50,5),1000000,1000));
}

/***************************************************************************************/
Model& Model::getInstance()
{
	if(instance == nullptr)
		instance = new Model();
	return (*instance);
}

/***************************************************************************************/

Model::~Model()
{
	allPorts.clear();
	allVessels.clear();
}

/***************************************************************************************/

vesselType Model::stringToShipType(string type) const
{
	if(type.compare("Freighter") == 0) return freighter;
	if(type.compare("Cruiser") == 0)   return cruiser;
	return patrol;
}

/***************************************************************************************/

Point Model::stringToPoint(string pos)
{
	stringstream ss(pos);
	char c;
	double first,second;
	ss>>c>>first>>c>>second;
	return Point(first,second);
}

/***************************************************************************************/

void Model::add_Vessel(shared_ptr<Vessel> vessel_x)
{
	allVessels.insert( pair< string, shared_ptr< Vessel> > (vessel_x->getName()  ,  vessel_x) );
}

/***************************************************************************************/

void Model::add_port(shared_ptr<Port> port)
{
	allPorts.insert( pair< string, shared_ptr< Port > > (port->getName() , port));
}

/***************************************************************************************/

shared_ptr<Port> Model::getPort(string port)
{
	auto tmp=allPorts.find(port);
	if(tmp==allPorts.end())
		return nullptr;

	return tmp->second;
}

/***************************************************************************************/

shared_ptr<Vessel> Model::getVessel(string ship_name)
{
	auto tmp=allVessels.find(ship_name);
	if(tmp==allVessels.end())
		return nullptr;

	return tmp->second;;
}

/***************************************************************************************/

string Model::inScale(int dirY,int dirX,int scale) const
{
	int i,j;

	for(auto vessel_it : allVessels){
		i=vessel_it.second->getLocation().x;
		j=vessel_it.second->getLocation().y;

		if(i<(dirX+scale) && i>=dirX && j<(dirY+scale) &&
				j>=dirY && vessel_it.second->getState() != Docked)

			return vessel_it.first;
	}
	for(auto port_iter : allPorts)
	{
		i=port_iter.second->getLocation().x;
		j=port_iter.second->getLocation().y;

		if(i<(dirX+scale)&& i>=dirX && j<(dirY+scale) && j>=dirY)
			return port_iter.first;
	}
	return "";
}

/***************************************************************************************/

void Model::addPortToPath(shared_ptr<Patrol_boat> vessel)
{
	double dis = DBL_MAX;
	double min = DBL_MAX;

	shared_ptr<Port> port = nullptr;
	string closest="";

	Cartesian_vector cv;
	Polar_vector pv;

	for(auto i : getPortList())
	{
		dis=distance(vessel->getLocation(),i.second->getLocation());
		if(dis < min)
		{
			if(vessel->alreadyVisited(i.second->getName()))
				continue;
			min =dis;
			closest= i.second->getName();
		}
	}

	if(min == DBL_MAX)
	{
		vessel->setDestination(vessel->getPort());
		port = getPort(vessel->getPort());
		cv.delta_x = port->getLocation().x-vessel->getLocation().x;
		cv.delta_y = port->getLocation().y-vessel->getLocation().y;
		pv = cv;
		vessel->getData().setCourseDirection(to_degrees(pv.theta));
		vessel->setState(MovingPort);
		vessel->getData().setSpeed(vessel->getSpeed()); //original speed set
		return;
	}

	vessel->addToSet(closest);
	port = getPort(closest);
	cv.delta_x = port->getLocation().x-vessel->getLocation().x;
	cv.delta_y = port->getLocation().y-vessel->getLocation().y;
	pv = cv;


	vessel->getData().setCourseDirection(to_degrees(pv.theta));
	vessel->setDestination(closest);
	vessel->setState(MovingPort);

	vessel->getData().setSpeed(vessel->getSpeed());
}

/***************************************************************************************/

void Model::addTaskToFreighter(string freighter,string task,string dest,string amount)
{
	shared_ptr<Freighter> f = dynamic_pointer_cast<Freighter>(getVessel(freighter));
	f->addTask(task,dest,stoi(amount));
}

/***************************************************************************************/

bool Model::inPortRange(shared_ptr<Vessel> sh)
{
	shared_ptr<Port> port=getPort(sh->getDestination());
	if(distance(port->getLocation(),sh->getLocation()) < 0.1)
		return true;

	return false;
}

/***************************************************************************************/

void Model::portTimeUpdate()
{
	string name="";

	for(auto i : allPorts)
	{
		i.second->increaseFuelSupply();
		if(!i.second->queueIsEmpty())
			name=i.second->getRefulingQHead();
		else
			continue;

		shared_ptr<Vessel> tmp = getVessel(name);
		auto tank = tmp->getFuelTank();

		if(tmp->getVesselType()==freighter)
		{
			auto diff=MaxFreighterTank-tank;
			if(diff > i.second->getFuelSupply())
				continue;

			tmp->setFuelTank(MaxFreighterTank);
			i.second->setFuelSupply(i.second->getFuelSupply()-diff);
			i.second->popFromRefulingQ();
		}
		else if(tmp->getVesselType() == patrol)
		{
			auto diff= MaxPatrolTank- tank;
			if(diff > i.second->getFuelSupply())
				continue;
			tmp->setFuelTank(MaxPatrolTank);
			i.second->setFuelSupply(i.second->getFuelSupply()-diff);
			i.second->popFromRefulingQ();
		}

	}
}

/***************************************************************************************/

void Model::VesselTimeUpdate()
{
	shared_ptr<Freighter> tmpFreighter = nullptr;
	shared_ptr<Cruiser> tmpCruiser = nullptr;
	shared_ptr<Patrol_boat> tmpPB = nullptr;
	shared_ptr<Port> tmpPort = nullptr;

	double dist=0;

	for(auto i : allVessels){
		switch(i.second->getVesselType())
		{
		case freighter: tmpFreighter = dynamic_pointer_cast<Freighter>(i.second);
		switch(tmpFreighter->getState()){
		case MovingCourse: tmpFreighter->advance();
		break;
		case MovingPort:
			tmpPort=getPort(tmpFreighter->getDestination());
			if(inPortRange(tmpFreighter)){
				tmpFreighter->setState(Docked);
				tmpFreighter->setLocation(tmpPort->getLocation());
				tmpFreighter->getData().setSpeed(0);
				tmpFreighter->getData().setCourseDirection(0);
				break;
			}
			dist = distance(tmpFreighter->getLocation(),tmpPort->getLocation());
			if(dist < tmpFreighter->getData().getSpeed())
				tmpFreighter->getData().setSpeed(dist); //decrease speed for docking
			tmpFreighter->advance();
			break;
		case MovingPosition: dist = distance(tmpFreighter->getLocation(),tmpFreighter->getDestinationPoint());
		if(dist < tmpFreighter->getData().getSpeed()){
			tmpFreighter->getData().setSpeed(dist);
			tmpFreighter->setState(Stopped);
		}
		tmpFreighter->advance();
		break;
		case Docked:
			tmpFreighter->execMission();
			break;

		default:
			break;
		}
		break;

		case cruiser:
			tmpCruiser = dynamic_pointer_cast<Cruiser>(getVessel(i.second->getName()));
			switch(tmpCruiser->getState()){
			case MovingCourse:
				tmpCruiser->update();
				break;
			case MovingPosition:
				tmpCruiser->update();
				break;
			default:
				break;
			}
			break;

			case patrol:
				tmpPB = dynamic_pointer_cast<Patrol_boat>(getVessel(i.second->getName()));
				switch(tmpPB->getState()){
				case MovingPort:
					tmpPort=getPort(tmpPB->getDestination()); //new start port
					tmpPB->addToSet(tmpPort->getName());
					if(inPortRange(tmpPB)){
						tmpPB->setState(Docked);
						tmpPB->getData().setCourseDirection(0);
						tmpPB->getData().setSpeed(0);
						tmpPB->setDockedPeriod(0);
						tmpPB->setLocation(tmpPort->getLocation());
					}
					else{
						dist = distance(tmpPB->getLocation(),tmpPort->getLocation());
						if(dist < tmpPB->getData().getSpeed())
							tmpPB->getData().setSpeed(dist);
						tmpPB->advance();
					}
					break;
				case Docked:
					tmpPort = getPort(tmpPB->getDestination());
					if(tmpPB->getDestination().compare(tmpPB->getPort())== 0 && tmpPB->getArrived())
					{
						tmpPB->clearSet();
						break;
					}
					if(tmpPB->getDockedPeriod() == 0)
					{
						if(tmpPort->queueIsEmpty())
							tmpPort->addToRefulingQ(tmpPB->getName());
						tmpPB->setDockedPeriod(1);
					}
					else if(tmpPB->getDockedPeriod() == 1)
					{
						tmpPB->setDockedPeriod(2);
					}
					else if(tmpPB->getDockedPeriod() == 2)
					{
						addPortToPath(tmpPB);
						if(tmpPB->getDestination().compare(tmpPB->getPort()) == 0)
							tmpPB->setArrived(true);
					}
					break;
				default:
					break;
				}
				break;
		}
	}
}

/***************************************************************************************/

void Model::update(vector<string>& vect)
{
	time++;
	vector<string> stringS;
	shared_ptr<Freighter> tmpFreighter = nullptr;
	shared_ptr<Cruiser> tmpCruiser = nullptr;
	shared_ptr<Port> tmpPort=nullptr;
	shared_ptr<Vessel> vessel=nullptr;

	for(auto i : vect){
		stringS=splitString(i);
		cmd command=commandFromString(stringS[1]);

		vessel=getVessel(stringS[0]);
		switch(command)
		{
		case LOAD_AT:
			tmpFreighter= dynamic_pointer_cast<Freighter>(vessel);
			tmpFreighter->addTask(stringS[2],"load",-1);
			tmpFreighter->setOrderByPort(tmpFreighter->getDestination());
			break;
		case UNLOAD_AT:
			tmpFreighter=dynamic_pointer_cast<Freighter>(vessel);
			tmpFreighter->addTask(stringS[2],"unload",stoi(vect[3]));
			tmpFreighter->setOrderByPort(tmpFreighter->getDestination());
			break;
		case ATTACK:
			tmpCruiser= dynamic_pointer_cast<Cruiser>(vessel);
			tmpCruiser->attack(getVessel(stringS[2]));
			break;
		case REFUEL:
			if( vessel->getVesselType() == freighter && vessel->getState()==Docked )
			{
				tmpPort->addToRefulingQ(vessel->getName());
				tmpPort=getPort(vessel->getDestination());
			}
			break;
		default:
			break;
		}
	}
	vect.clear();

	VesselTimeUpdate();

	portTimeUpdate();
}


/***************************************************************************************/

void Model::status()
{
	for(auto i : allPorts)
	{
		cout<<"Port "<<i.second->getName()<<" at position ";
		i.second->getLocation().print();
		cout<<", Fuel available: "<<fixed<<setprecision(1)<<i.second->getFuelSupply()<<" kl"<<endl;
	}

	shared_ptr<Freighter> tmpFreighter = nullptr;
	shared_ptr<Cruiser> tmpCruiser = nullptr;
	shared_ptr<Patrol_boat> tmpPB = nullptr;

	for(auto i : allVessels)
	{
		vesselType type = i.second->getVesselType();

		switch(type){
		case freighter: tmpFreighter = dynamic_pointer_cast<Freighter>(i.second);
		tmpFreighter->printStatus();
		break;
		case cruiser:  tmpCruiser = dynamic_pointer_cast<Cruiser>(i.second);
		tmpCruiser->printStatus();
		break;
		case patrol: tmpPB = dynamic_pointer_cast<Patrol_boat>(i.second);
		tmpPB->printStatus();
		break;
		}
	}
}

/***************************************************************************************/

vector<string> Model::splitString(string in)
{
	stringstream ss(in);

	istream_iterator<string> begin(ss);
	istream_iterator<string> end;

	vector<string> splitStrings(begin, end);
	return splitStrings;
}

/***************************************************************************************/

void Model::commandInputCheck(string line) noexcept(false)
{
	auto vect = splitString(line);
	bool firstArg = false;
	shared_ptr<Vessel> ship = nullptr;

	if(vect.size() == 0 || vect.size() > MAX_ARGUMENTS_NUM)
		throw AllExceptions::InvalidInputException();

	cmd command= commandFromString(vect[0]);

	if(command==GO||command==DEFAULT||command== STATUS||command==STOP||command==EXIT||command==SHOW)
	{
		if(vect.size() > 1)
			throw AllExceptions::InvalidInputException();

		firstArg = true;
	}
	else if( command==PAN||command==SIZE || command==ZOOM )
	{
		if(vect.size() != 2)
			throw AllExceptions::InvalidInputException();
		if(command==SIZE && (stoi(vect[1]) > MAX_SIZE || stoi(vect[1]) < MIN_SIZE))
			throw AllExceptions::InvalidInputException();
		if(command==ZOOM && stoi(vect[1]) < 1)
			throw AllExceptions::InvalidInputException();
		firstArg = true;
	}
	else if(command==CREATE)
	{
		if(vect.size() < MAX_ARGUMENTS_NUM-1 || vect[1].length() > MaxNameLength)
			throw AllExceptions::InvalidInputException();

		else if(inList(vect[1]) )
			throw AllExceptions::VesselAlreadyExistException();
		else
		firstArg = true;
	}

	if(firstArg) return;
	if(vect.size() == 1)
		throw AllExceptions::InvalidInputException();

	command = commandFromString(vect[1]);

	if(command==DESTINATION || command==UNLOAD_AT)
	{
		if(vect.size() != 4)
			throw AllExceptions::InvalidInputException();
		if(!inList(vect[0]))
			throw AllExceptions::NoVesselException();
		if(!inPortList(vect[2]) )
			throw AllExceptions::NoPortException();
		if(command==DESTINATION && !speedCheck(stoi(vect[3]),vect[0]))
			throw AllExceptions::InvalidInputException();
		return;
	}
	else if(command==LOAD_AT || command==DOCK_AT)
	{
		if(vect.size() != 3)
			throw AllExceptions::InvalidInputException();
		if(!inList(vect[0]))
			throw AllExceptions::NoVesselException();
		if(!inPortList(vect[2]))
			throw AllExceptions::NoPortException();
		return;
	}
	else if(command==COURSE)
	{
		if(vect.size() != 4)
			throw AllExceptions::InvalidInputException();
		if(!speedCheck(stoi(vect[3]),vect[0]))
			throw AllExceptions::InvalidInputException();
		return;
	}
	else if(command==POSITION)
	{
		if(vect.size() != 5)
			throw AllExceptions::InvalidInputException();
		if(!inList(vect[0]))
			throw AllExceptions::NoVesselException();
		if(!speedCheck(stoi(vect[4]),vect[0]))
			throw AllExceptions::InvalidInputException();
		if(!pointCheck(vect[2]+vect[3]))
			throw AllExceptions::InvalidInputException();
		return;
	}
	else if( command==REFUEL||command==STOP)
	{
		if(vect.size() != 2)
			throw AllExceptions::InvalidInputException();
		if(!inList(vect[0]))
			throw AllExceptions::NoVesselException();
		return;
	}
	else if(command==ATTACK)
	{
		if(vect.size() != 3)
			throw AllExceptions::InvalidInputException();

		if(!inList(vect[0]) || !inList(vect[2]))
			throw AllExceptions::NoVesselException();
	}
	else if(command == ERROR)
		throw AllExceptions::InvalidInputException(); //no command found
							}

/***************************************************************************************/

bool Model::inList(string ship) const
{
	if(allVessels.find(ship) == allVessels.end())
		return false;

	return true;
}

/***************************************************************************************/

bool Model::inPortList(string port) const
{
	if(allPorts.find(port) == allPorts.end())
		return false;

	return true;
}

/***************************************************************************************/

bool Model::pointCheck(string pT) const
{
	if(pT[0] != '(' && pT.at(pT.length()-1) != ')')
		return false;

	int commas=0;
	int dots=0;

	for(unsigned i=0;i<pT.length();i++){
		if(isalpha(pT[i]))
			return false;
		if(pT[i] == '.')
			dots++;
		if(pT[i]== ',')
			commas++;
	}

	if(dots != 2 || commas != 1)
		return false;

	return true;
}

/***************************************************************************************/

bool Model::speedCheck(int speed,string name)
{
	if(speed < 0)
		return false;
	shared_ptr<Vessel> tmpV = getVessel(name);

	if(tmpV->getVesselType() == freighter && speed > MaxFreighterMph)
		return false;
	else if(tmpV->getVesselType() == patrol && speed > MaxPatrolMph)
		return false;
	else if(tmpV->getVesselType() == cruiser && speed > MaxCruiserMph)
		return false;


	return true;
}

/***************************************************************************************/

double Model::distance(Point source,Point target)
{
	return sqrt( pow(source.y-target.y,2) +pow(source.x-target.x,2));
}

/***************************************************************************************/

cmd Model::commandFromString(string val)
{
	if(val.compare("status")        ==   0) return STATUS;
	else if(val.compare("create") 	==   0) return CREATE;
	else if(val.compare("course") 	==   0) return COURSE;
	else if(val.compare("position") ==   0) return POSITION;
	else if(val.compare("default") 	==   0) return DEFAULT;
	else if(val.compare("unload_at")==   0) return UNLOAD_AT;
	else if(val.compare("dock_at")  ==   0) return DOCK_AT;
	else if(val.compare("attack")   ==   0) return ATTACK;
	else if(val.compare("refuel")   ==   0) return REFUEL;
	else if(val.compare("stop")     ==   0) return STOP;
	else if(val.compare("size")		==   0) return SIZE;
	else if(val.compare("zoom")     ==   0) return ZOOM;
	else if(val.compare("pan")	    ==   0) return PAN;
	else if(val.compare("show")     ==   0) return SHOW;
	else if(val.compare("destination")==   0) return DESTINATION;
	else if(val.compare("load_at") 	  ==   0) return LOAD_AT;
	else if(val.compare("exit") 	  ==   0) return EXIT;
	else if(val.compare("go") 		  ==   0) return GO;
	else
		return ERROR;
}
