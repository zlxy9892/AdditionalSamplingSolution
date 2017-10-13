#include <iostream>
#include "Utility.h"
#include "Processing.h"

using namespace std;

void LoadData_heshan(string dataDir, EnvDataset *envDataset)
{
	envDataset->Layers.push_back(new EnvLayer(0, "slope", dataDir + "slp.tif", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(1, "planc", dataDir + "plan.tif", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(2, "profc", dataDir + "prof.tif", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(3, "twi", dataDir + "twi.tif", DataTypeEnum::SINGLEVALUE));
	envDataset->RefreshAll();
}

void LoadData_raffelson(string dataDir, EnvDataset *envDataset)
{
	envDataset->Layers.push_back(new EnvLayer(0, "bedrock", dataDir + "Bedrock.asc", DataTypeEnum::FACTOR));
	envDataset->Layers.push_back(new EnvLayer(1, "elev", dataDir + "Elevation.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(2, "slope", dataDir + "Slope.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(3, "planc", dataDir + "PlanformCurve.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(4, "profc", dataDir + "ProfileCurve.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(5, "wet", dataDir + "Wet.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(6, "pctg", dataDir + "Pctglmult.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->RefreshAll();
}

void LoadData_xc(string dataDir, EnvDataset *envDataset)
{
	envDataset->Layers.push_back(new EnvLayer(0, "bedrock", dataDir + "geo.asc", DataTypeEnum::FACTOR));
	envDataset->Layers.push_back(new EnvLayer(1, "planc", dataDir + "planc.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(2, "preci", dataDir + "preci.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(3, "profc", dataDir + "profc.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(4, "slope", dataDir + "slope.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(5, "tempr", dataDir + "tempr.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(6, "wet", dataDir + "twi.asc", DataTypeEnum::SINGLEVALUE));
	envDataset->RefreshAll();
}

int main(int argc, char *argv[])
{
	cout<<"Start!\n";

	// 初始化环境因子数据
	cout<<"\nLoading data ...";
	GDALAllRegister();
	EnvDataset *envDataset = new EnvDataset();
	string dataDir = "../data/raffelson/";
	LoadData_raffelson(dataDir, envDataset);
	//string dataDir = "../data/heshan/";
	//LoadData_heshan(dataDir, envDataset);
	//string dataDir = "../data/xc/";	
	//LoadData_xc(dataDir, envDataset);
	cout<<"\nRead data OK!\n";
	string sampleFname = "../data/raffelson/addSamples.csv";

	// set the hyperparameters
	Processing *processing = new Processing(envDataset);
	processing->unc_thred_max = 0.4;
	processing->unc_thred_min = 0.1;
	processing->unc_thred = 0.4;
	processing->p_factor = 100.0;
	processing->imporve_factor = 100.0;
	int maxSampleNumber = 20;
	processing->ShowParameters();

	processing->FindBestNewSampleListByObj(maxSampleNumber);
	Utility::WriteCSV("./addSamples.csv", processing->SampleEnvUnits);

	// set different power factor of W1 (the ratio of the area can be predicted)
	//processing->GetSampleListByDifferentPowerFactor(maxSampleNumber, 0.5, 1.5, 0.1);

	// set different power factor and improve factor
	//processing->GetSampleListByDifferentPowerFactorAndImproveFactor(maxSampleNumber, 0.2, 2.0, 0.2, 0.2, 2.0, 0.2);
	//processing->ShowSampleListInfoByDifferentPowerFactorAndImproveFactor(maxSampleNumber, "../data/raffelson/samples_pfactor_ifactor/", "addSamples_", 0.2, 2.0, 0.2, 0.2, 2.0, 0.2);

	//processing->ShowProcessInfo("../data/raffelson/samples_thred_max/addSamples_0.4.csv");

	//Utility::WriteEnvDataCSV("envData_raf.csv", envDataset->EnvUnits);

	// read samples by kmeans algorithm
	/*vector<EnvUnit *> samples_kmeans = Utility::ReadCSV("../data/raffelson/samples_kmeans/samples_kmeans_30.csv", processing->EDS);
	processing->SampleEnvUnits = samples_kmeans;
	processing->RefreshUncertainty();
	cout<<processing->CalcUncertainty_Sum();*/


	// final handle
	delete processing;
	processing = NULL;


	cout<<"\n\n----- DONE! -----\n";
	system("pause");
	return 0;
}