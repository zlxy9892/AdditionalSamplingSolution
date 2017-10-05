// 环境单元，包含多个环境因子的信息
#ifndef _ENVUNIT_H_
#define _ENVUNIT_H_


#include <vector>
#include "DataTypeEnum.h"
#include "Location.h"

using namespace std;

class EnvUnit
{
public:
	bool IsCal;							// 是否参与计算
	vector<double> EnvValues;			// 该点内所有环境因子值
	vector<DataTypeEnum> DataTypes;		// 该点内所有环境因子值的类型
	double SoilType;					// 土壤类型值
	double SoilVarible;					// 土壤属性值
	string SampleID;					// 若是样点，记录样点的ID号
	Location *Loc;						// 该点的位置信息
	double Uncertainty;					// 该点的不确定性值
	double MaxSimi;						// 该点与样点集最大的相似度值
	double CellSize;					// 该点的边长（宽高）
	bool isCanPredict;					// 是否可以推测

	double PredictSoilVarible;			// 推测的土壤属性值
	double PredictUncertainty;			// 推测不确定性
	double PredictCredibility;			// 推测可信度

	double Credibility;					// 样点的可信度
	int Number_Support;					// 支持样点数量
	int Number_Contradict;				// 矛盾样点数量

	int Density;						// 密度（与该点相似度在阈值以上的点的数量）
	double DSimi;						// 比该点密度更高且与该点最相似的点的相似度

public:
	EnvUnit(void);
	~EnvUnit(void);

	void AddEnvValue(double envValue);
};

#endif