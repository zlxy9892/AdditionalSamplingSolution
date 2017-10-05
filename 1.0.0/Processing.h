// ������
#ifndef _PROCESSING_H_
#define _PROCESSING_H_


#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "DataTypeEnum.h"
#include "Location.h"
#include "EnvUnit.h"
#include "BasicSetting.h"
#include "Location.h"
#include "EnvDataset.h"
#include "Utility.h"
#include <omp.h>


using namespace std;

class Processing
{
public:
	EnvDataset *EDS;
	vector<EnvUnit *> SampleEnvUnits;
	double unc_thred;
	double w1;	// Ȩ��1�������Ʋ��������
	double w2;	// Ȩ��2��1-w1

public:
	Processing(void);
	~Processing(void);
	Processing(EnvDataset *envDataset, vector<EnvUnit *> sampleEnvUnits);
	Processing(EnvDataset *envDataset);

	double CalcSimi_Single(double e1, double e2, double range, DataTypeEnum dataType);
	double CalcSimi(EnvUnit *e1, EnvUnit *e2);											// ��������������Ԫ֮����ۺ����ƶ�
	double CalcUncertainty(vector<EnvUnit *> samples, EnvUnit *e);						// ����ĳһ����Ʋⲻȷ����
	int CalcCanPredictArea(double thred_unc);											// ������Ʋ����������ȷ��������ֵ���ڵ�դ������
	double CalcUncertainty_Sum();														// �����Ʋⲻȷ�����ܺ�
	void RefreshUncertainty(vector<EnvUnit *> samples);									// ����ÿ�����Ʋ��Ĳ�ȷ����
	void RefreshUncertainty();															// ����ÿ�����Ʋ��Ĳ�ȷ����
	void RefreshUncertainty_Parallel();													// ����ÿ�����Ʋ��Ĳ�ȷ����
	double ObjectFunction();															// ����Ŀ�꺯��
	double ObjectFunctionByNewSample(EnvUnit *newSample);								// ����Ŀ�꺯��
	EnvUnit* FindBestNewSampleByObj();
	vector<EnvUnit*> FindBestNewSampleListByObj(int newSampleCount);
	void UpdateWeights();

	void ShowInfo(int iter=0);
	void ShowProcessInfo(string sampleFilename);
};

#endif 