#include "Processing.h"


Processing::Processing(void)
{
}

Processing::Processing(EnvDataset *envDataset, vector<EnvUnit *> sampleEnvUnits)
{
	this->EDS = envDataset;
	this->SampleEnvUnits = SampleEnvUnits;
	this->unc_thred_max = 0.2;
	this->unc_thred_min = 0.1;
	this->unc_thred = 0.2;
	this->w1 = 1.0;
	this->w2 = 0.0;
	this->p_factor = 1.0;
	this->imporve_factor = 0.2;
}

Processing::Processing(EnvDataset *envDataset)
{
	this->EDS = envDataset;
	this->unc_thred_max = 0.2;
	this->unc_thred_min = 0.1;
	this->unc_thred = 0.2;
	this->w1 = 1.0;
	this->w2 = 0.0;
	this->p_factor = 1.0;
	this->imporve_factor = 0.2;
}

Processing::~Processing(void)
{
	if (this->EDS != NULL)
	{
		delete this->EDS;
		this->EDS = NULL;
	}
}

double Processing::CalcSimi_Single(double e1, double e2, double range, DataTypeEnum dataType)
{
	double simi = -1;
	if (dataType == FACTOR)
	{
		if (e1 == e2)
		{
			simi = 1;
		}
		else
		{
			simi = 0;
		}
	}
	else if (dataType == SINGLEVALUE)
	{
		simi = 1 - abs(e1 - e2) / range;
	}
	return simi;
}

double Processing::CalcSimi(EnvUnit *e1, EnvUnit *e2)
{
	double simi = -1;
	if (!e1->IsCal || !e2->IsCal)
	{
		return -1;	// 不参与计算的点
	}
	if (e1->EnvValues.size() == e2->EnvValues.size())
	{
		simi = 1;
		for (int i = 0; i < e1->EnvValues.size(); i++)
		{
			double range = this->EDS->Layers.at(i)->Data_Range;
			DataTypeEnum dataType = e1->DataTypes.at(i);
			double simi_temp = this->CalcSimi_Single(e1->EnvValues.at(i), e2->EnvValues.at(i), range, dataType);
			if (simi_temp < simi)
			{
				simi = simi_temp;
			}
		}
		return simi;
	}
	else
	{
		return -1;
	}
}

double Processing::CalcUncertainty(vector<EnvUnit *> samples, EnvUnit *e)
{
	double simi = 0;
	for (int i = 0; i < samples.size(); ++i)
	{
		EnvUnit *se = samples[i];
		double simi_temp = this->CalcSimi(se, e);
		if (simi < simi_temp)
		{
			simi = simi_temp;
		}
	}
	return 1-simi;
}

int Processing::CalcCanPredictArea(double thred_unc)
{
	int area = 0;
	int count = this->EDS->EnvUnits.size();
	for (int i = 0; i < count; ++i)
	{
		EnvUnit *e = this->EDS->EnvUnits[i];
		if (e->IsCal && e->Uncertainty <= thred_unc)
		{
			area++;
		}
	}
	return area;
}

void Processing::RefreshUncertainty( vector<EnvUnit *> samples )
{
	int count = this->EDS->EnvUnits.size();
	for (int i = 0; i < count; ++i)
	{
		EnvUnit *e = this->EDS->EnvUnits[i];
		if (e->IsCal)
		{
			e->Uncertainty = this->CalcUncertainty(samples, e);
		}
		else
		{
			e->Uncertainty = 1;
		}
	}
}

void Processing::RefreshUncertainty()
{
	int count = this->EDS->EnvUnits.size();
	for (int i = 0; i < count; ++i)
	{
		EnvUnit *e = this->EDS->EnvUnits[i];
		if (e->IsCal)
		{
			e->Uncertainty = this->CalcUncertainty(this->SampleEnvUnits, e);
		}
		else
		{
			e->Uncertainty = 1;
		}
	}
}

void Processing::RefreshUncertainty_Parallel()
{
	int count = this->EDS->EnvUnits.size();
#pragma omp parallel for
	for (int i = 0; i < count; ++i)
	{
		EnvUnit *e = this->EDS->EnvUnits[i];
		if (e->IsCal)
		{
			e->Uncertainty = this->CalcUncertainty(this->SampleEnvUnits, e);
		}
		else
		{
			e->Uncertainty = 1;
		}
	}
}

void Processing::RefreshIsCanPredict()
{
	int count = this->EDS->EnvUnits.size();
	for (int i = 0; i < count; ++i)
	{
		EnvUnit *e = this->EDS->EnvUnits[i];
		if (e->IsCal)
		{
			if (e->Uncertainty <= this->unc_thred /*this->unc_thred_min*/)
			{
				e->isCanPredict = true;
			}
			else
			{
				e->isCanPredict = false;
			}
		}
		else
		{
			e->isCanPredict = false;
		}
	}
}

double Processing::CalcUncertainty_Sum()
{
	double unc_sum = 0;
	int count = this->EDS->EnvUnits.size();
	for (int i = 0; i < count; ++i)
	{
		EnvUnit *e = this->EDS->EnvUnits[i];
		if (e->IsCal)
		{
			unc_sum += e->Uncertainty;
		}
	}
	return unc_sum;
}

double Processing::ObjectFunction()
{
	double o = 0;
	double canPreArea = this->CalcCanPredictArea(this->unc_thred);
	double totalArea = this->EDS->CalcArea;
	double unc_sum = this->CalcUncertainty_Sum();
	double o1 = 1.0 - 1.0 * canPreArea / totalArea;
	double o2 = 1.0 * unc_sum / totalArea;
	//cout<<o1<<"\t"<<o2<<"\n";
	o = this->w1 * o1 + this->w2 * o2;
	return o;
}


void Processing::UpdateUncertaintyThred()
{
	double unc_mean = this->CalcUncertainty_Sum() / this->EDS->CalcArea;
	double unc_thred_new = this->unc_thred_max - this->imporve_factor * (this->unc_thred_max - unc_mean);
	this->unc_thred = max(this->unc_thred_min, min(unc_thred_new, this->unc_thred_max));
}

void Processing::UpdateWeights()
{
	double canPreArea = this->CalcCanPredictArea(this->unc_thred);
	double totalArea = this->EDS->CalcArea;
	double cannotPredRatio = 1.0 - 1.0 * canPreArea / totalArea;
	this->w1 = pow(cannotPredRatio, this->p_factor);
	this->w2 = 1 - this->w1;
}

double Processing::ObjectFunctionByNewSample( EnvUnit *newSample )
{
	double o = 0;
	this->SampleEnvUnits.push_back(newSample);
	this->RefreshUncertainty_Parallel();
	double canPreArea = this->CalcCanPredictArea(this->unc_thred);
	double totalArea = this->EDS->CalcArea;
	double unc_sum = this->CalcUncertainty_Sum();
	double o1 = 1.0 - 1.0 * canPreArea / totalArea;
	double o2 = 1.0 * unc_sum / totalArea;
	//cout<<o1<<"\t"<<o2<<"\n";
	o = this->w1 * o1 + this->w2 * o2;
	this->SampleEnvUnits.pop_back();
	return o;
}

EnvUnit* Processing::FindBestNewSampleByObj()
{
	EnvUnit *sample_best = NULL;
	EnvUnit *sample_tmp = NULL;
	double obj_min = 9999;
	int segementCount = 1000;
	int count = this->EDS->EnvUnits.size();
	for (int i = 0; i < count; ++i)
	{
		if(i % (count/segementCount) == 0)
		{
			cout<<'\r';
			cout<<"Completed "<<setw(5)<<(int((i*1.0/count+0.5/segementCount)*segementCount))/(segementCount/100.0)<<"%";
		}
		sample_tmp = this->EDS->EnvUnits[i];
		if (sample_tmp->IsCal && sample_tmp->isCanPredict == false)
		{
			double obj = this->ObjectFunctionByNewSample(sample_tmp);
			if (obj_min > obj)
			{
				obj_min = obj;
				sample_best = sample_tmp;
			}
		}
	}
	cout<<'\n';
	return sample_best;
}

vector<EnvUnit*> Processing::FindBestNewSampleListByObj(int newSampleCount)
{
	for (int i = 0; i < newSampleCount; i++)
	{
		this->RefreshUncertainty();
		this->RefreshIsCanPredict();
		this->UpdateUncertaintyThred();		// update the unc_thred with the itheration (key step)
		this->UpdateWeights();				// update w1 and w2 with the itheration (key step)
		EnvUnit *bestSample = this->FindBestNewSampleByObj();
		if(bestSample == NULL)
		{
			cout<<"no suitable sample can be found.\n";
			break;
		}
		this->SampleEnvUnits.push_back(bestSample);
		
		this->ShowInfo(i+1);
		Utility::WriteCSV("./addSamples.csv", this->SampleEnvUnits);
	}
	return this->SampleEnvUnits;
}

void Processing::GetSampleListByDifferentPowerFactor( int newSampleCount, double pfactor_min, double pfactor_max, double pfactor_step )
{
	for(double p_factor = pfactor_min; p_factor <= pfactor_max+0.0001; p_factor+=pfactor_step)
	{
		cout<<"\n-------------------p_factor: "<<p_factor<<"--------------------\n";
		this->SampleEnvUnits.clear();
		this->p_factor = p_factor;
		this->FindBestNewSampleListByObj(newSampleCount);
		string outfilename = "./addSamples_" + Utility::ConvertToString(p_factor) + ".csv";
		Utility::WriteCSV(outfilename, this->SampleEnvUnits);
		cout<<"\n--------------------------------------------\n";
	}
}

void Processing::GetSampleListByDifferentPowerFactorAndImproveFactor( int newSampleCount, double pfactor_min, double pfactor_max, double pfactor_step, double ifactor_min, double ifactor_max, double ifactor_step )
{
	for(double p_factor = pfactor_min; p_factor <= pfactor_max+0.0001; p_factor+=pfactor_step)
	{
		for(double i_factor = ifactor_min; i_factor <= ifactor_max+0.0001; i_factor+=ifactor_step)
		{
			cout<<"\n-------------------p_factor: "<<p_factor<<", i_factor: "<<i_factor<<"--------------------\n";
			this->SampleEnvUnits.clear();
			this->p_factor = p_factor;
			this->imporve_factor = i_factor;
			this->FindBestNewSampleListByObj(newSampleCount);
			string outfilename = "./addSamples_" + Utility::ConvertToString(p_factor) + "-" + Utility::ConvertToString(i_factor) + ".csv";
			Utility::WriteCSV(outfilename, this->SampleEnvUnits);
			cout<<"\n--------------------------------------------\n";
		}
	}
}

void Processing::ShowSampleListInfoByDifferentPowerFactorAndImproveFactor( int newSampleCount, string dir, string prefix, double pfactor_min, double pfactor_max, double pfactor_step, double ifactor_min, double ifactor_max, double ifactor_step )
{
	for(double p_factor = pfactor_min; p_factor <= pfactor_max+0.0001; p_factor+=pfactor_step)
	{
		for(double i_factor = ifactor_min; i_factor <= ifactor_max+0.0001; i_factor+=ifactor_step)
		{
			this->SampleEnvUnits.clear();
			string filename = dir + prefix + Utility::ConvertToString(p_factor) + "-" + Utility::ConvertToString(i_factor) + ".csv";
			this->SampleEnvUnits = Utility::ReadCSV(filename, this->EDS);
			this->RefreshUncertainty();
			//cout<<"\n-- p_factor: "<<p_factor<<", i_factor: "<<i_factor<<"\t"<<this->CalcUncertainty_Sum()<<"\n";
			cout<<p_factor<<","<<i_factor<<","<<this->CalcUncertainty_Sum()<<"\n";
			//cout<<this->CalcUncertainty_Sum();
		}
	}
}

void Processing::ShowInfo(int iter)
{
	double canPreArea = this->CalcCanPredictArea(this->unc_thred);
	double totalArea = this->EDS->CalcArea;
	double unc_sum = this->CalcUncertainty_Sum();
	double o1 = 1.0 - 1.0 * canPreArea / totalArea;
	double o2 = 1.0 * unc_sum / totalArea;

	cout<<"\n-------------------------------------\n";
	cout<<"iteration:\t"<<iter<<"\n";
	//Utility::ShowEnvUnit(this->SampleEnvUnits);
	cout<<"sample size:\t"<<this->SampleEnvUnits.size()<<"\n";
	cout<<"unc_sum:\t"<<this->CalcUncertainty_Sum()<<"\n";
	cout<<"unc_mean:\t"<<this->CalcUncertainty_Sum() / this->EDS->CalcArea<<"\n";
	cout<<"unc_thred:\t"<<this->unc_thred<<"\n";
	cout<<"Obj:\t"<<this->ObjectFunction()<<"\n";
	cout<<"O1:\t"<<o1<<"\n";
	cout<<"O2:\t"<<o2<<"\n";
	cout<<"W1:\t"<<this->w1<<"\n";
	cout<<"W2:\t"<<this->w2<<"\n";
}

void Processing::ShowProcessInfo( string sampleFilename )
{
	
	vector<EnvUnit *> samples = Utility::ReadCSV(sampleFilename, this->EDS);
	vector<EnvUnit *> samples_tmp;
	for(int i = 0; i < samples.size(); i++)
	{
		samples_tmp.clear();
		for(int j = 0; j <= i; j++)
		{
			samples_tmp.push_back(samples[j]);
		}
		this->SampleEnvUnits = samples_tmp;
		this->RefreshUncertainty();
		this->UpdateUncertaintyThred();
		this->UpdateWeights();
		double canPreArea = this->CalcCanPredictArea(this->unc_thred);
		double totalArea = this->EDS->CalcArea;
		double unc_sum = this->CalcUncertainty_Sum();
		double o1 = 1.0 - 1.0 * canPreArea / totalArea;
		double o2 = 1.0 * unc_sum / totalArea;

		//cout<<"\n-------------------------------------\n";
		//Utility::ShowEnvUnit(this->SampleEnvUnits);
		cout<<"unc_sum:\t"<<this->CalcUncertainty_Sum()<<"\n";
		//cout<<"unc_mean:\t"<<this->CalcUncertainty_Sum() / this->EDS->CalcArea<<"\n";
		//cout<<"unc_thred:\t"<<this->unc_thred<<"\n";
		//cout<<"Obj:\t"<<this->ObjectFunction()<<"\n";
		//cout<<"O1:\t"<<o1<<"\n";
		//cout<<"O2:\t"<<o2<<"\n";
		//cout<<"W1:\t"<<this->w1<<"\n";
		//cout<<"W2:\t"<<this->w2<<"\n";
	}
}

void Processing::ShowParameters()
{
	cout<<"\n*************** parameters ***************\n";
	cout<<"unc_thred_min:\t"<<this->unc_thred_min<<"\n";
	cout<<"unc_thred_max:\t"<<this->unc_thred_max<<"\n";
	cout<<"p-factor:\t"<<this->p_factor<<"\n";
	cout<<"improve-factor:\t"<<this->imporve_factor<<"\n";
	cout<<"*************** parameters ***************\n\n";
}


