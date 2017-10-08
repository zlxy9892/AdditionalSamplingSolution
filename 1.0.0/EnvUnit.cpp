#include "EnvUnit.h"

EnvUnit::EnvUnit(void)
{
	this->IsCal = true;
	this->Loc = new Location();
	this->Uncertainty = 1.0;
	this->MaxSimi = 0;
	this->CellSize = 10;
	this->isCanPredict = false;
	this->Credibility = 0;
	this->Number_Contradict = 0;
	this->Number_Support = 0;
	this->PredictCredibility = -1;
	this->PredictSoilVarible = -1;
	this->PredictUncertainty = -1;
	
	this->Density = 0;
	this->DSimi = 0;
}

EnvUnit::~EnvUnit(void)
{
	delete this->Loc;
}

void EnvUnit::AddEnvValue(double envValue)
{
	this->EnvValues.push_back(envValue);
}
