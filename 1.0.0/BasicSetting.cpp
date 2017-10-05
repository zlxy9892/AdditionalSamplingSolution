#include "BasicSetting.h"


BasicSetting::BasicSetting(void)
{
	this->Width = 0;
	this->Height = 0;
	this->Xmin = 0;
	this->Ymin = 0;
	this->Xmax = 0;
	this->Ymax = 0;
	this->CellSize = 10;
	this->NoDataValue = -9999;
}


BasicSetting::~BasicSetting(void)
{
}
