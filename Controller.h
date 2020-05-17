/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define MaxNameLength 12
#define INPUT_FILE_ARGUMENTS_NUM 5

#include <fstream>
#include <algorithm>
#include "Model.h"
#include "View.h"

class Controller {
private:
	Model& model;
	View viewer;
	vector<string> commands;

public:

	Controller(string);
	Controller(const Controller&);
	Controller(Controller&&);
	Controller& operator=(const Controller&);
	Controller& operator=(Controller&&);

	~Controller();


	void run();

	cmd command(string);
	void inputPortFile(string)  noexcept(false);
	void inputFileCheck(string) noexcept(false);
	View& getViewer(){ return viewer; }

};
#endif /* CONTROLLER_H_ */
