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
	//string dataDir = "../data/raffelson/";
	//LoadData_raffelson(dataDir, envDataset);
	//string dataDir = "../data/heshan/";
	//LoadData_heshan(dataDir, envDataset);
	string dataDir = "../data/xc/";	
	LoadData_xc(dataDir, envDataset);
	cout<<"\nRead data OK!\n";
	string sampleFname = "../data/raffelson/addSamples.csv";

	// set the hyperparameters
	Processing *processing = new Processing(envDataset);
	processing->unc_thred_max = 0.4;
	processing->unc_thred_min = 0.1;
	processing->unc_thred = 0.4;
	processing->p_factor = 1.0;
	processing->imporve_factor = 1.0;
	int maxSampleNumber = 50;
	processing->ShowParameters();

	processing->FindBestNewSampleListByObj(maxSampleNumber);
	Utility::WriteCSV("./addSamples.csv", processing->SampleEnvUnits);

	// set different power factor of W1 (the ratio of the area can be predicted)
	//processing->GetSampleListByDifferentPowerFactor(maxSampleNumber, 0.5, 1.5, 0.1);

	// set different power factor and improve factor
	//processing->GetSampleListByDifferentPowerFactorAndImproveFactor(maxSampleNumber, 0.2, 2.0, 0.2, 0.2, 2.0, 0.2);
	//processing->GetSampleListByDifferentPowerFactorAndImproveFactor(maxSampleNumber, 2.2, 4.0, 0.2, 0.2, 2.0, 0.2);
	//processing->ShowSampleListInfoByDifferentPowerFactorAndImproveFactor(maxSampleNumber, "../data/raffelson/samples_pfactor_ifactor_20171016/", "addSamples_", 0.2, 4.0, 0.2, 0.2, 2.0, 0.2);
	//processing->GetSampleListByDifferentUncThredMaxAndImproveFactor(maxSampleNumber, 0.1, 0.5, 0.01, 1.0, 1.0, 0.2);
	//processing->ShowSampleListInfoByDifferentUncThredMaxAndImproveFactor(maxSampleNumber, "../data/raffelson/samples_uncmax_ifactor_20171018/", "addSamples_", 0.02, 0.4, 0.02, 0.2, 2.0, 0.2);

	//processing->ShowProcessInfo("../data/raffelson/samples_kmeans/samples_kmeans_25_.csv");
	//processing->ShowProcessInfo("../data/raffelson/addSamples.csv");
	//processing->ShowProcessInfo("../data/raffelson/compare/sss_25/sss5.csv");

	//Utility::WriteEnvDataCSV("envData_raf.csv", envDataset->EnvUnits);

	// read samples by kmeans algorithm
	/*vector<EnvUnit *> samples_kmeans = Utility::ReadCSV("../data/raffelson/samples_kmeans/samples_kmeans_20_.csv", processing->EDS);
	processing->SampleEnvUnits = samples_kmeans;
	processing->RefreshUncertainty();
	cout<<processing->CalcUncertainty_Sum();*/
	
	// get stratified random samples
	/*double factorList[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
	int sampleCountList[5] = {6, 3, 5, 4, 7};
	vector<EnvUnit*> stratifiedRandomSamples = Utility::GetStratifiedRandomSamples(envDataset->EnvUnits, factorList, sampleCountList, 5);
	processing->SampleEnvUnits = stratifiedRandomSamples;
	processing->RefreshUncertainty();
	cout<<processing->CalcUncertainty_Sum()<<"\n";
	Utility::ShowEnvUnit(processing->SampleEnvUnits);*/

	// predict and calculate the accuracy of validate data
	//processing->SampleEnvUnits = Utility::ReadCSV("../data/raffelson/addSamples_25.csv", envDataset);
	//processing->SampleEnvUnits = Utility::ReadCSV("../data/raffelson/compare/sss_25/sss1.csv", envDataset);
	//processing->ValidateSampleEnvUnits = Utility::ReadCSV("../data/raffelson/ValidationSamples_real.csv", envDataset);
	//processing->ShowProcessInfo("../data/raffelson/addSamples_25.csv");


	// final handle
	delete processing;
	processing = NULL;

	cout<<"\n\n----- DONE! -----\n";
	system("pause");
	return 0;
}