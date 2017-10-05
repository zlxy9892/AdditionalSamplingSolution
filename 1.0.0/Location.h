#ifndef _LOCATION_H_
#define _LOCATION_H_


class Location
{
public:
	int Row;	// 栅格所在位置的行号
	int Col;	// 栅格所在位置的列号
	double X;	// 栅格所在位置的x坐标值，该栅格的左上角的值
	double Y;	// 栅格所在位置的y坐标值，该栅格的左上角的值

public:
	Location(void);
	~Location(void);

	Location(int row, int col, double x, double y);
};

#endif