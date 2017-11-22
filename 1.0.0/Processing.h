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
#include <math.h>


using namespace std;

class Processing
{
public:
	EnvDataset *EDS;
	vector<EnvUnit *> SampleEnvUnits;
	vector<EnvUnit *> ValidateSampleEnvUnits;
	double unc_thred_max;
	double unc_thred_min;
	double unc_thred;
	double w1;			// Ȩ��1�������Ʋ��������
	double w2;			// Ȩ��2��1-w1
	double p_factor;	// ָ������
	double imporve_factor;	// ������

public:
	Processing(void);
	~Processing(void);
	Processing(EnvDataset *envDataset, vector<EnvUnit *> sampleEnvUnits);
	Processing(EnvDataset *envDataset);

	double CalcSimi_Single(double e1, double e2, double range, DataTypeEnum dataType);
	double CalcSimi(EnvUnit *e1, EnvUnit *e2);											// ��������������Ԫ֮����ۺ����ƶ�
	double CalcUncertainty(vector<EnvUnit *> samples, EnvUnit *e);						// ����ĳһ����Ʋⲻȷ����
	int CalcCanPredictArea(double thred_unc);											// ������Ʋ����������ȷ��������ֵ���ڵ�դ������
	int CalcCanPredictAreaByUncTmp();													// ���� unc_tmp ������Ʋ����������ȷ��������ֵ���ڵ�դ������
	double CalcCanPredictAreaProportion(double thred_unc);								// ������Ʋ����ռ������ı���
	double CalcUncertainty_Sum();														// �����Ʋⲻȷ�����ܺ�
	double CalcUncertaintyTmp_Sum();													// ������ʱ�Ʋⲻȷ�����ܺ�
	void RefreshUncertainty(vector<EnvUnit *> samples);									// ����ÿ�����Ʋ��Ĳ�ȷ����
	void RefreshUncertainty();															// ����ÿ�����Ʋ��Ĳ�ȷ����
	void RefreshUncertainty_Parallel();													// ����ÿ�����Ʋ��Ĳ�ȷ����
	void RefreshUncertaintyTmpByNewSample(EnvUnit* newSample);							// ����ÿ�����Ʋ��Ĳ�ȷ���ԣ�����һ���������㣬���ʿ�һЩ��
	void RefreshIsCanPredict();															// ����ÿ�����Ƿ�ɱ��Ʋ⣨ÿ������Ʋⲻȷ�����Ƿ������Ͳ�ȷ������ֵ unc_thred_min��
	double ObjectFunction();															// ����Ŀ�꺯��
	double ObjectFunction(double thred_unc);											// ����Ŀ�꺯������ĳһ��ȷ����Լ���µĽ��ֵ��
	double ObjectFunctionByNewSample(EnvUnit *newSample);								// ����Ŀ�꺯��
	EnvUnit* FindBestNewSampleByObj();
	vector<EnvUnit*> FindBestNewSampleListByObj(int newSampleCount);
	void UpdateUncertaintyThred();
	void UpdateWeights();

	vector<EnvUnit *> ConstraintKmeansClustering(vector<EnvUnit *> envUnits, vector<EnvUnit *> existedSamples, int newSampleCount);

	void PredictClass(vector<EnvUnit *> predEnvUnits);
	double CalcPredictClassAccuracy();

	double MaxSimiSample( EnvUnit *e, vector<EnvUnit *> envUnits );

	// ----------- for test ------------ //
	void GetSampleListByDifferentPowerFactor(int newSampleCount, double pfactor_min, double pfactor_max, double pfactor_step);
	void GetSampleListByDifferentPowerFactorAndImproveFactor(int newSampleCount, double pfactor_min, double pfactor_max, double pfactor_step, double ifactor_min, double ifactor_max, double ifactor_step);
	void ShowSampleListInfoByDifferentPowerFactorAndImproveFactor(int newSampleCount, string dir, string prefix, double pfactor_min, double pfactor_max, double pfactor_step, double ifactor_min, double ifactor_max, double ifactor_step);
	void GetSampleListByDifferentUncThredMaxAndImproveFactor(int newSampleCount, double uncmax_min, double uncmax_max, double uncmax_step, double ifactor_min, double ifactor_max, double ifactor_step);
	void ShowSampleListInfoByDifferentUncThredMaxAndImproveFactor(int newSampleCount, string dir, string prefix, double uncmax_min, double uncmax_max, double uncmax_step, double ifactor_min, double ifactor_max, double ifactor_step);
	// ----------- for test ------------ //

	// ----------- show info ------------ //
	void ShowInfo(int iter=0);
	void ShowProcessInfo(string sampleFilename);
	void ShowParameters();
	// ----------- show info ------------ //
};

#endif 