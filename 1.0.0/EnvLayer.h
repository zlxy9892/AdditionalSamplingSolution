// 环境数据图层
#ifndef _ENVLAYER_H_
#define _ENVLAYER_H_


#include <string>
#include <vector>
#include "gdal_priv.h"
#include "ogrsf_frmts.h" // for ogr
#include "gdal_alg.h"	 // for GDALPolygonize
#include "cpl_conv.h"	 // for CPLMalloc() 
#include "DataTypeEnum.h"

using namespace std;

class EnvLayer
{
public:
	int LayerId;
	string LayerName;
	vector<double> EnvData;
	//double *EnvData;
	DataTypeEnum DataType;
	GDALDataset *GdalEnvData;
	double Data_Max;
	double Data_Min;
	double Data_Range;
	double NoDataValue;
	int XSize;
	int YSize;

public:
	EnvLayer(void);
	~EnvLayer(void);

	EnvLayer(int layerId, string layerName, string filename, DataTypeEnum dataType);
	void CalcStat();

	void Writeout(string filename, string type, GDALDataset *srcDs);
};

#endif