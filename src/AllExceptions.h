/*
 * AllExceptions.h
 *
 *  Created on: Apr 27, 2018
 *      Author: kosta
 */

#ifndef ALLEXCEPTIONS_H_
#define ALLEXCEPTIONS_H_

class AllExceptions {
public:

	class InvalidInputFileException
	{
	public:
		InvalidInputFileException()
	{
			cerr<<"Invalid input file exception"<<endl;
	}

	};
	/************************************************************************/


	class NoPortException
{
	public:
		NoPortException()
	{
			cerr<<"ERR No such port exists"<<endl;
	}

};

	/************************************************************************/
	class NoVesselException
{
	public:
		NoVesselException()
	{
			cerr<<endl<<"ERR No such Vessel "<<endl;

	}

};

/************************************************************************/
class VesselAlreadyExistException
{
public:
	VesselAlreadyExistException()
	{
		cerr<<"ERR Vessel already exist"<<endl;
	}

};

/************************************************************************/
class InvalidInputException
{
public:
	InvalidInputException()
	{
		cerr<<"ERR Invalid input"<<endl;

	}

};

/************************************************************************/

class NotEnoughArgumentsException
{
public:
	NotEnoughArgumentsException()
	{
		cerr<<"ERROR Not enough Arguments for requested command"<<endl;
	}
};
/************************************************************************/
};

#endif /* ALLEXCEPTIONS_H_ */
