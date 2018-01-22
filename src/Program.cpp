#include <iostream>
#include "Utility.h"
#include "Processing.h"

using namespace std;

void LoadData_heshan(string dataDir, EnvDataset *envDataset)
{
	envDataset->Layers.push_back(new EnvLayer(0, "slope", dataDir + "slp.tif", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(1, "planc", dataDir + "planc.tif", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(2, "profc", dataDir + "profc.tif", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(3, "wet", dataDir + "wet.tif", DataTypeEnum::SINGLEVALUE));
	envDataset->Layers.push_back(new EnvLayer(4, "soiltype", dataDir + "soiltype_hs.tif", DataTypeEnum::SINGLEVALUE));
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
	//string dataDir = "../data/raffelson/covariables/";
	//LoadData_raffelson(dataDir, envDataset);
	//string dataDir = "../data/heshan/envdata/covariables/";
	//LoadData_heshan(dataDir, envDataset);
	string dataDir = "../data/xc/covariables/";	
	LoadData_xc(dataDir, envDataset);
	cout<<"\nRead data OK!\n";

	// set the hyperparameters
	Processing *processing = new Processing(envDataset);
	processing->unc_thred_max = 0.2;
	processing->unc_thred_min = 0.01;
	processing->unc_thred = 0.2;
	processing->p_factor = 1.0;
	processing->imporve_factor = 1.0;
	int maxSampleNumber = 30;
	processing->ShowParameters();

	// set existed samples
	vector<EnvUnit*> existedSamples = Utility::ReadCSV("../data/xc/samples_designed/existed_samples_10.csv", processing->EDS);
	//vector<EnvUnit*> existedSamples = Utility::ReadCSV("../data/xc/samples_designed/add_samples_sss_10_30_5.csv", processing->EDS);
	//vector<EnvUnit*> existedSamples = Utility::ReadTable("../data/xc/samples_designed/add_samples_mymethod_10_30.csv", processing->EDS);
	//vector<EnvUnit*> existedSamples = Utility::ReadTable("../data/xc/samples_designed/add_samples_oldmethod_10_30.csv", processing->EDS);
	processing->ValidateSampleEnvUnits = Utility::ReadTable("../data/xc/samples_validation.csv", envDataset, "SOMA");
	processing->SampleEnvUnits = existedSamples;
	//for (int i = 0; i < processing->SampleEnvUnits.size(); i++)
	//{ processing->GetMostSimiSample(processing->ValidateSampleEnvUnits, processing->SampleEnvUnits[i]); }
	
	//for (int i = 0; i < processing->SampleEnvUnits.size(); i++)
	//{ cout<<processing->SampleEnvUnits[i]->EnvValues[0]<<' '; }
	//Utility::ShowSampleNumberForEachStrata(processing->EDS->EnvUnits, 40);
	//processing->ShowProcessInfo("../data/raffelson/samples_designed/add_samples_cluster_10_20.csv");
	//processing->ShowProcessInfo("../data/xc/samples_designed/add_samples_mymethod_10_30.csv");
	//processing->ShowProcessInfo("../data/xc/samples_designed/add_samples_sss_10_30_5.csv");
	//processing->ShowProcessInfo("../data/raffelson/samples_designed/add_samples_oldmethod_25.csv");

	// add additional samples
	processing->FindBestNewSampleListByObj(maxSampleNumber, 4);
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

	//Utility::WriteEnvDataCSV("envData_hs.csv", envDataset->EnvUnits);

	// read samples by kmeans algorithm
	/*vector<EnvUnit *> samples_kmeans = Utility::ReadCSV("../data/raffelson/samples_kmeans/samples_kmeans_20_.csv", processing->EDS);
	processing->SampleEnvUnits = samples_kmeans;
	processing->RefreshUncertainty();
	cout<<processing->CalcUncertainty_Sum();*/
	
	// get stratified random samples
	/*double factorList[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
	int sampleCountList[5] = {6, 3, 5, 4, 7};
	int sampleCountList[5] = {1, 1, 1, 1, 1};
	vector<EnvUnit*> stratifiedRandomSamples = Utility::GetStratifiedRandomSamples(envDataset->EnvUnits, factorList, sampleCountList, 5);
	processing->SampleEnvUnits = stratifiedRandomSamples;
	processing->RefreshUncertainty();
	cout<<processing->CalcUncertainty_Sum()<<"\n";
	Utility::ShowEnvUnit(processing->SampleEnvUnits);*/
	/*double factorList[8] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
	int sampleCountList[8] = { 3, 1, 4, 4, 0, 11, 5, 2 };
	vector<EnvUnit*> stratifiedRandomSamples = Utility::GetStratifiedRandomSamples(envDataset->EnvUnits, factorList, sampleCountList, 8);
	Utility::ShowEnvUnit(stratifiedRandomSamples);*/

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