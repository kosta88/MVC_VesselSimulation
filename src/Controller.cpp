/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#include "Controller.h"
/*********************************************************************************/
//default d'tor
Controller::~Controller()
{

}

/*********************************************************************************/

Controller::Controller(string input_file)
: model(Model::getInstance()),viewer(25,2,Point(0,0))
{
	try{
		inputPortFile(input_file);
	}
	catch (AllExceptions::InvalidInputFileException& ex)
	{
		exit(1);
	}
}

/*********************************************************************************/

Controller::Controller(const Controller& rhs) : model(Model::getInstance()),viewer(rhs.viewer)
{

}

/*********************************************************************************/

Controller::Controller(Controller&& mhs) : model(mhs.model),viewer(mhs.viewer),commands(mhs.commands)
{

}

/*********************************************************************************/

Controller& Controller::operator=(const Controller& rhs){
	if(this == &rhs)
		return *this;
	viewer = rhs.viewer;
	commands = rhs.commands;
	return *this;
}

/*********************************************************************************/

Controller& Controller::operator=(Controller&& mhs)
{
	swap(viewer,mhs.viewer);
	swap(commands,mhs.commands);
	return *this;
}

/*********************************************************************************/

void Controller::inputPortFile(string input_file) noexcept(false)
{
	ifstream file(input_file);
	if(!file.is_open())
		throw AllExceptions::InvalidInputFileException();
	string str = "";
	vector<string> vect;
	Point p;
	shared_ptr<Port> port = nullptr;

	while(getline(file,str)){
		try {
			inputFileCheck(str); //check line
		} catch (AllExceptions::InvalidInputFileException& e)
		{
			exit(1);
		}
		vect = model.splitString(str); //split string to vector
		p = model.stringToPoint(vect[1]+vect[2]);
		port = make_shared<Port>(vect[0],p,stoi(vect[3]),stoi(vect[4])); //new port
		model.add_port(port); //add to list
	}
}

/*********************************************************************************/

void Controller::inputFileCheck(string in) noexcept(false)
		{
	auto vect = model.splitString(in);
	if(vect.size() != INPUT_FILE_ARGUMENTS_NUM)
		throw AllExceptions::InvalidInputFileException();

	for(auto j=3;j<INPUT_FILE_ARGUMENTS_NUM;j++)
		for(unsigned i=0;i<vect[j].length();i++)
			if(!isdigit(vect[j].at(i)))
				throw AllExceptions::InvalidInputFileException();
}

/*********************************************************************************/

void Controller::run(){
	string input;
	while(true)
	{
		cout<<"Time "<<model.getTime()<<": Enter command: ";
		getline(cin,input);

		switch(command(input))
		{
		case UNIMMEDIATE:
			commands.push_back(input);
			break;
		case IMMEDIATE:
			break;
		case GO:
			model.update(commands);
			break;
		case EXIT:
			return;
		default:
			break;
		}
	}
}

/*********************************************************************************/

cmd Controller::command(string line)
{

	try{
		model.commandInputCheck(line);
	}
	catch (AllExceptions::InvalidInputException& ex)
	{
		return ERROR;
	}
	catch (AllExceptions::NoVesselException& ex)
	{
		return ERROR;
	}
	catch (AllExceptions::NoPortException& ex)
	{
		return ERROR;
	}
	catch (AllExceptions::VesselAlreadyExistException& ex)
	{
		return ERROR;
	}
	auto vect = model.splitString(line);

	bool flag=true;
	vesselType type=freighter;
	shared_ptr<Vessel> tmpVessel = nullptr;
	shared_ptr<Patrol_boat> tmpPB = nullptr;
	shared_ptr<Freighter> tmpFreighter = nullptr;
	shared_ptr<Port> tmpPort=nullptr;

	Point p;
	Cartesian_vector cv;
	Polar_vector pv;
	int arg;
	cmd command = model.commandFromString(vect[0]);

	switch(command)
	{
	case GO:
		return GO;
	case DEFAULT:
		viewer.setDefualt();
		break;
	case SIZE:
		viewer.resize(stoi(vect[1]));
		break;
	case ZOOM:
		viewer.zoom(stod(vect[1]));
		break;
	case PAN:
		viewer.pan(stod(vect[1]),stod(vect[2]));
		break;
	case SHOW:
		viewer.show();
		break;
	case STATUS:
		model.status();
		break;
	case CREATE:
		type = model.stringToShipType(vect[2]);
		vect.size() == 7 ? arg=stoi(vect[6]) : arg=-1;
		tmpVessel = model.getFactory()->makeShip(vect[1],type,model.stringToPoint(vect[3]+vect[4]),stoi(vect[5]),arg);
		model.add_Vessel(tmpVessel);
		break;
	case EXIT:
		return EXIT;
	default:
		command = model.commandFromString(vect[1]);
		flag=false;
		break;
	}

	if(flag) return IMMEDIATE;
	tmpVessel=model.getVessel(vect[0]);



	switch(command)
	{
	case POSITION:
		p = model.stringToPoint(vect[2]+vect[3]);
		tmpVessel->setDestinationPoint(p);
		cv.delta_x = tmpVessel->getDestinationPoint().x-tmpVessel->getLocation().x;
		cv.delta_y = tmpVessel->getDestinationPoint().y-tmpVessel->getLocation().y;
		pv = cv;
				tmpVessel->getData().setCourseDirection(to_degrees(pv.theta));
				tmpVessel->getData().setSpeed(stod(vect[4]));
				tmpVessel->setState(MovingPosition);
				tmpVessel->setDestination("");
				break;
	case COURSE:
		tmpVessel->getData().setCourseDirection(stod(vect[2]));
		tmpVessel->getData().setSpeed(stod(vect[3]));
		tmpVessel->setState(MovingCourse);
		tmpVessel->setDestination("");
		break;
	case DESTINATION:
		tmpPort= model.getPort(vect[2]);
		if(tmpVessel->getVesselType() == patrol){
			tmpPB = dynamic_pointer_cast<Patrol_boat>(tmpVessel);
			tmpPB->setPort(tmpPort->getName());
			tmpPB->setSpeed(stod(vect[3]));
			tmpPB->setArrived(false);
		}

		p = tmpPort->getLocation();
		cv.delta_x = p.x-tmpVessel->getLocation().x;
		cv.delta_y = p.y-tmpVessel->getLocation().y;
		pv = cv;

		tmpVessel->getData().setCourseDirection(to_degrees(pv.theta));
		tmpVessel->getData().setSpeed(stod(vect[3]));
		tmpVessel->setDestination(vect[2]);
		if(tmpVessel->getVesselType() == freighter){
			tmpFreighter = dynamic_pointer_cast<Freighter>(tmpVessel);
			tmpFreighter->setOrderByPort(vect[2]);
		}
		tmpVessel->setState(MovingPort);
		break;
	case STOP:
		tmpVessel->stopShipping();
		if(tmpVessel->getVesselType()==freighter)
			dynamic_pointer_cast<Freighter>(tmpVessel)->clearTasks();
		break;
	default:
		flag=false;
		break;
	}
	if(flag)
		return IMMEDIATE;


	return UNIMMEDIATE;
}

/*********************************************************************************/


class SingletonDtor
{
public:

	~SingletonDtor()
	{
		delete Model::instance;
	}
} sd;

