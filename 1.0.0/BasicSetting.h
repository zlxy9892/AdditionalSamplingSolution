#ifndef _BASICSETTING_H_
#define _BASICSETTING_H_


#include <string>

using namespace std;

class BasicSetting
{
public:
	int Width;
	int Height;
	double Xmin;
	double Ymin;
	double Xmax;
	double Ymax;
	double CellSize;
	double NoDataValue;

public:
	BasicSetting(void);
	~BasicSetting(void);
};

#endif