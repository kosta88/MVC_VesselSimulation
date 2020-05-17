/*
 *  Created on: June 21, 2018
 *      Author: kosta
 */
#include "View.h"

/*****************************************************************/

View::View(int S_ize,double S_cale,Point init) :
start(init),size(S_ize),scale(S_cale)
{

}

/*****************************************************************/

View::View(const View& rhs)
{
	*this=rhs;
}

/*****************************************************************/

View::View(View&& Vm)
:start(Vm.start),size(Vm.size),scale(Vm.scale)
{

}

/*****************************************************************/

View& View:: operator=(const View& rhs)
{
	if(this == &rhs)
		return *this;
	size = rhs.size;
	scale = rhs.scale;
	start=rhs.start;
	return *this;
}

/*****************************************************************/

View& View::operator=(View&& mhs)
{
	swap(size,mhs.size);
	swap(scale,mhs.scale);
	swap(start,mhs.start);
	return *this;
}

/*****************************************************************/

void View::show()
{

	double dirY=start.y+scale*(size+3)-scale;
	double ConstDirY= dirY-3*scale;
	double dirX=start.x-3*scale;

	cout<<"Display size: "<<size<<", scale: "<<scale<<", origin: ("<<
			start.x<<","<<start.y<<")"<<endl;

	for(auto i = 0 ; i < size ; i++){
		if(i%3==0)
		{
			dirY -= 3*scale;
			cout<<setw(4)<<(int)dirY;
		}
		else cout<<setw(4)<<"";
		for(auto j = 0 ; j < size ; j++)
		{
			auto name=Model::getInstance().inScale(ConstDirY-i*scale,j*scale+start.x,scale);
			if(name.length()==0)
				cout<<setw(2)<<".";

			else cout<<name.substr(0,2);
		}
		cout<<endl;
	}
	for(auto i = 0 ; i < size ; i++){
		if(i%3 == 0)
		{
			dirX += scale*3;
			cout<<setw(6)<<(int)dirX;
		}
	}
	cout<<endl;
}

/*****************************************************************/

void View::zoom(double x)
{
	scale=x;
}

/*****************************************************************/

void View::resize(int x)
{
	size=x;
}

/*****************************************************************/

void View::setDefualt()
{
	size=25;
	scale=2;
	start.x=0;
	start.y=0;

}

/*****************************************************************/

void View::pan(double x,double y)
{
	start.x=x;
	start.y=y;
}

/*****************************************************************/
