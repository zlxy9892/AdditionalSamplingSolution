#include "EnvLayer.h"

EnvLayer::EnvLayer(void)
{
}

EnvLayer::EnvLayer(int layerId, string layerName, string filename, DataTypeEnum dataType)
{
	this->LayerId = layerId;
	this->LayerName = layerName;
	this->DataType = dataType;
	this->GdalEnvData = (GDALDataset *)GDALOpen(filename.c_str(), GA_ReadOnly);
	this->NoDataValue = -9999;
	this->NoDataValue = this->GdalEnvData->GetRasterBand(1)->GetNoDataValue();
	double xSize = this->GdalEnvData->GetRasterXSize();
	double ySize = this->GdalEnvData->GetRasterYSize();
	this->XSize = xSize;
	this->YSize = ySize;
	float *pData = (float*)CPLMalloc(sizeof(float)*xSize*ySize);
	this->GdalEnvData->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, xSize, ySize, pData, xSize, ySize, GDALDataType::GDT_Float32, 0, 0);
	this->EnvData.reserve(xSize*ySize);
	for(int i = 0; i <xSize*ySize; i++)
	{
		this->EnvData.push_back(pData[i]);
	}
	CPLFree(pData);
	pData = NULL;
	this->CalcStat();
}

EnvLayer::~EnvLayer(void)
{
	GDALClose((GDALDatasetH)this->GdalEnvData);
}

void EnvLayer::CalcStat()
{
	if(this->EnvData.size() <= 0) { return; }
	int count = this->EnvData.size();
	double min = this->EnvData[0];
	double max = this->EnvData[0];
	for(int i = 0; i < count; i++)
	{
		if(this->EnvData[i] != this->NoDataValue)
		{
			min = this->EnvData[i];
			max = this->EnvData[i];
			break;
		}
	}
	for(int i = 0; i < count; i++)
	{
		if(this->EnvData[i] == this->NoDataValue)
		{
			continue;
		}
		double value = this->EnvData[i];
		if(value < min)
		{
			min = value;
		}
		if(value > max)
		{
			max = value;
		}
	}
	this->Data_Min = min;
	this->Data_Max = max;
	this->Data_Range = max-min;
}

void EnvLayer::Writeout(string filename, string type, GDALDataset *srcDs)
{
	GDALDriver *pDriver = GetGDALDriverManager()->GetDriverByName(type.c_str());
	GDALDataset *ds = pDriver->Create(filename.c_str(), this->XSize, this->YSize, 1, GDT_Float32, NULL);
	if (ds == NULL)
	{
		return;
	}
	double geoTransform[6];
	srcDs->GetGeoTransform(geoTransform);
	ds->SetGeoTransform(geoTransform);
	ds->SetProjection(srcDs->GetProjectionRef());
	ds->GetRasterBand(1)->SetNoDataValue(this->NoDataValue);
	int pixelCount = this->XSize * this->YSize;
	float *pData = new float[pixelCount];
	for (int i = 0; i < pixelCount; i++)
	{
		pData[i] = (float)this->EnvData[i];
	}
	ds->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, this->XSize, this->YSize, pData, this->XSize, this->YSize, GDT_Float32, 0, 0);
	ds->FlushCache();

	GDALClose((GDALDatasetH)ds);
}
