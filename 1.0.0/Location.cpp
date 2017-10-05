#include "Location.h"


Location::Location(void)
{
}


Location::~Location(void)
{
}

Location::Location(int row, int col, double x, double y)
{
	this->Row = row;
	this->Col = col;
	this->X = x;
	this->Y = y;
}
