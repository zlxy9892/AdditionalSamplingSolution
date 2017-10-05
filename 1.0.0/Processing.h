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


using namespace std;

class Processing
{
public:
	EnvDataset *EDS;
	vector<EnvUnit *> SampleEnvUnits;
	double unc_thred;
	double w1;	// 权重1：不可推测面积比例
	double w2;	// 权重2：1-w1

public:
	Processing(void);
	~Processing(void);
	Processing(EnvDataset *envDataset, vector<EnvUnit *> sampleEnvUnits);
	Processing(EnvDataset *envDataset);

	double CalcSimi_Single(double e1, double e2, double range, DataTypeEnum dataType);
	double CalcSimi(EnvUnit *e1, EnvUnit *e2);											// 计算两个环境单元之间的综合相似度
	double CalcUncertainty(vector<EnvUnit *> samples, EnvUnit *e);						// 计算某一点的推测不确定性
	int CalcCanPredictArea(double thred_unc);											// 计算可推测面积，即不确定性在阈值以内的栅格数量
	double CalcUncertainty_Sum();														// 计算推测不确定性总和
	void RefreshUncertainty(vector<EnvUnit *> samples);									// 更新每个待推测点的不确定性
	void RefreshUncertainty();															// 更新每个待推测点的不确定性
	void RefreshUncertainty_Parallel();													// 更新每个待推测点的不确定性
	double ObjectFunction();															// 计算目标函数
	double ObjectFunctionByNewSample(EnvUnit *newSample);								// 计算目标函数
	EnvUnit* FindBestNewSampleByObj();
	vector<EnvUnit*> FindBestNewSampleListByObj(int newSampleCount);
	void UpdateWeights();

	void ShowInfo(int iter=0);
	void ShowProcessInfo(string sampleFilename);
};

#endif 