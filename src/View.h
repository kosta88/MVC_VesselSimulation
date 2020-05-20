/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#ifndef VIEW_H_
#define VIEW_H_

#include "Model.h"
#include <iomanip>

class View {
private:
	Point start;
	int size;
	double scale;
public:
	View(int,double,Point);
	~View() {};

	View(const View&);
	View(View&&);
	View& operator=(const View&);
	View& operator=(View&&);

	void zoom(double x);
	void resize(int x);
	void show();
	void setDefualt();
	void pan(double x,double y);
};

#endif /* VIEW_H_ */
