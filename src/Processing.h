// 处理方法
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
	double w1;			// 权重1：不可推测面积比例
	double w2;			// 权重2：1-w1
	double p_factor;	// 指数因子
	double imporve_factor;	// 激进率

public:
	Processing(void);
	~Processing(void);
	Processing(EnvDataset *envDataset, vector<EnvUnit *> sampleEnvUnits);
	Processing(EnvDataset *envDataset);

	double CalcSimi_Single(double e1, double e2, double range, DataTypeEnum dataType);
	double CalcSimi(EnvUnit *e1, EnvUnit *e2);											// 计算两个环境单元之间的综合相似度
	double CalcUncertainty(vector<EnvUnit *> samples, EnvUnit *e);						// 计算某一点的推测不确定性
	int CalcCanPredictArea(double thred_unc);											// 计算可推测面积，即不确定性在阈值以内的栅格数量
	int CalcCanPredictAreaByUncTmp();													// 根据 unc_tmp 计算可推测面积，即不确定性在阈值以内的栅格数量
	double CalcCanPredictAreaProportion(double thred_unc);								// 计算可推测面积占总面积的比例
	double CalcUncertainty_Sum();														// 计算推测不确定性总和
	double CalcUncertaintyTmp_Sum();													// 计算临时推测不确定性总和
	void RefreshUncertainty(vector<EnvUnit *> samples);									// 更新每个待推测点的不确定性
	void RefreshUncertainty();															// 更新每个待推测点的不确定性
	void RefreshUncertainty_Parallel();													// 更新每个待推测点的不确定性
	void RefreshUncertaintyTmpByNewSample(EnvUnit* newSample);							// 更新每个待推测点的不确定性（基于一个新增样点，速率快一些）
	void RefreshIsCanPredict();															// 更新每个点是否可被推测（每个点的推测不确定性是否低于最低不确定性阈值 unc_thred_min）
	double ObjectFunction();															// 计算目标函数
	double ObjectFunction(double thred_unc);											// 计算目标函数（在某一不确定性约束下的结果值）
	double ObjectFunctionByNewSample(EnvUnit *newSample);								// 计算目标函数
	EnvUnit* FindBestNewSampleByObj(int step = 1);
	vector<EnvUnit*> FindBestNewSampleListByObj(int newSampleCount, int step = 1);
	void UpdateUncertaintyThred();
	void UpdateWeights();
	EnvUnit* GetMostSimiSample(vector<EnvUnit *> samples, EnvUnit *e);

	vector<EnvUnit *> ConstraintKmeansClustering(vector<EnvUnit *> envUnits, vector<EnvUnit *> existedSamples, int newSampleCount);

	void PredictClass(vector<EnvUnit *> predEnvUnits);
	double CalcPredictClassAccuracy();

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