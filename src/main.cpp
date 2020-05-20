/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#include "Controller.h"

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		cerr<<"Invalid number of arguments"<<endl;
		return -1;
	}


	shared_ptr<Controller> controller = make_shared<Controller>(string(argv[1]));

	controller->run();

	return 0;
}

