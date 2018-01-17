#include "Utility.h"

Utility::Utility( void )
{
}

Utility::~Utility( void )
{
}

string Utility::ConvertToString( double value )
{
	ostringstream os;
	os.unsetf(ios::scientific);
	os.precision(10);
	if (os << value)
		return os.str();  
	return "invalid conversion";
}

void Utility::ParseStr( string str, char c, vector<string>& tokens ) {
	unsigned int posL = 0;
	unsigned int posR = 0;
	while(posR < str.length()-1) {
		posR = str.find_first_of(c, posL);
		string sub = str.substr(posL, posR-posL);
		tokens.push_back(sub);
		posL = posR + 1;
	}
}

vector<EnvUnit *> Utility::ReadCSV( string filename, EnvDataset *envDataset )
{
	vector<EnvUnit*> envUnits;
	ifstream file(filename); // declare file stream:
	if (!file.is_open())
	{
		return envUnits;
	}

	// 处理第一行，获取X,Y属性在文件中的列数位置
	string line;
	getline(file, line);
	vector<string> names;
	int pos_X = 0;
	int pos_Y = 1;
	int pos_value = 2;
	Utility::ParseStr(line, ',', names);
	for(int i = 0; i < names.size(); i++)
	{
		if(names[i] == "X" || names[i] == "x")
		{
			pos_X = i;
			break;
		}
	}
	for(int i = 0; i < names.size(); i++)
	{
		if(names[i] == "Y" || names[i] == "y")
		{
			pos_Y = i;
			break;
		}
	}
	for(int i = 0; i < names.size(); i++)
	{
		if(names[i] == "value" || names[i] == "VALUE")
		{
			pos_value = i;
			break;
		}
	}

	// 根据x，y值读取样点信息
	while (getline(file, line))
	{
		vector<string> values;
		Utility::ParseStr(line, ',', values);
		const char *xstr = values[pos_X].c_str();
		const char *ystr = values[pos_Y].c_str();
		double x = atof(xstr);
		double y = atof(ystr);
		EnvUnit *e = envDataset->GetEnvUnit(x, y);
		if (values.size() >= 3)
		{
			const char *valuestr = values[pos_value].c_str();
			double value = atof(valuestr);
			e->SoilVarible = value;
		}
		if(e != NULL)
		{
			envUnits.push_back(e);
		}
	}
	file.close();
	return envUnits;
}

vector<EnvUnit *> Utility::ReadCSV( string filename, EnvDataset *envDataset, string targetVName, string idName )
{
	vector<EnvUnit*> envUnits;
	ifstream file(filename); // declare file stream:

	// 处理第一行，获取X,Y属性、土壤属性值名称、样点ID号在文件中的列数位置
	string line;
	getline(file, line);
	vector<string> names;
	int pos_X = 0;
	int pos_Y = 1;
	int pos_targetVName = 2;
	int pos_idName = 0;
	Utility::ParseStr(line, ',', names);
	for(int i = 0; i < names.size(); i++)
	{
		if(names[i] == "X" || names[i] == "x")
		{
			pos_X = i;
			break;
		}
	}
	for(int i = 0; i < names.size(); i++)
	{
		if(names[i] == "Y" || names[i] == "y")
		{
			pos_Y = i;
			break;
		}
	}
	for(int i = 0; i < names.size(); i++)
	{
		if(names[i] == targetVName)
		{
			pos_targetVName = i;
			break;
		}
	}
	for(int i = 0; i < names.size(); i++)
	{
		if(names[i] == idName)
		{
			pos_idName = i;
			break;
		}
	}

	// 根据x，y值读取样点信息
	while (getline(file, line))
	{
		vector<string> values;
		Utility::ParseStr(line, ',', values);
		const char *xstr = values[pos_X].c_str();
		const char *ystr = values[pos_Y].c_str();
		const char *targetVstr = values[pos_targetVName].c_str();
		string id = values[pos_idName];
		double x = atof(xstr);
		double y = atof(ystr);
		double targetV = atof(targetVstr);
		EnvUnit *e = envDataset->GetEnvUnit(x, y);
		if(e != NULL)
		{
			e->SoilVarible = targetV;
			e->SampleID = id;
			envUnits.push_back(e);
		}
	}
	file.close();
	return envUnits;
}

void Utility::WriteCSV( string filename, vector<EnvUnit *> envUnit )
{
	if(envUnit.size() <= 0)
	{
		return;
	}
	ofstream file(filename);
	string firstLine = "X,Y\n";
	file<<firstLine;
	
	double cellSize = envUnit[0]->CellSize;
	for(int i = 0; i < envUnit.size(); i++)
	{
		double x = envUnit[i]->Loc->X + cellSize / 2;
		double y = envUnit[i]->Loc->Y - cellSize / 2;
		string xstr = Utility::ConvertToString(x);
		string ystr = Utility::ConvertToString(y);
		string line = xstr + "," +ystr + "\n";
		file<<line;
	}
	file.flush();
	file.close();
}

void Utility::WriteCSV2( string filename, vector<EnvUnit *> envUnit, vector<int> predictedAreaList )
{
	if(envUnit.size() <= 0)
	{
		return;
	}
	ofstream file(filename);
	string firstLine = "X,Y,PredictedArea\n";
	file<<firstLine;
	
	double cellSize = envUnit[0]->CellSize;
	for(int i = 0; i < envUnit.size(); i++)
	{
		double x = envUnit[i]->Loc->X + cellSize / 2;
		double y = envUnit[i]->Loc->Y - cellSize / 2;
		int predictedArea = predictedAreaList[i];
		string xstr = Utility::ConvertToString(x);
		string ystr = Utility::ConvertToString(y);
		string predictedAreaStr = Utility::ConvertToString(predictedArea);
		string line = xstr + "," +ystr + "," + predictedAreaStr + "\n";
		file<<line;
	}
	file.flush();
	file.close();
}

EnvUnit* Utility::GetOneRandomEnvUnit( vector<EnvUnit *> envUnits )
{
	vector<int> indexList;
	int count = envUnits.size();
	for (int i = 0; i < count; ++i)
	{
		if (envUnits[i]->IsCal)
		{
			indexList.push_back(i);
		}
	}

	int maxnum = indexList.size();

	boost::random::uniform_int_distribution<> dist(0, maxnum-1);		// maxnum 为最大值，其随机域为 0 ~ maxnum-1
	int ii = dist(rng);
	int index = indexList[ii];

	return envUnits[index];
}

vector<EnvUnit *> Utility::GetEnvUnitsByFactor( vector<EnvUnit *> envUnits, double factorVal )
{
	vector<EnvUnit *> res;
	int count = envUnits.size();
	for (int i = 0; i < count; ++i)
	{
		EnvUnit *e = envUnits[i];
		if (e->IsCal && e->EnvValues[0] == factorVal)
		{
			res.push_back(e);
		}
	}
	return res;
}

vector<EnvUnit *> Utility::GetRandomEnvUnitsByFactor( vector<EnvUnit *> envUnits, double factorVal, int sampleCount )
{
	vector<EnvUnit *> res;
	vector<EnvUnit *> eus = Utility::GetEnvUnitsByFactor(envUnits, factorVal);
	for (int i = 0; i < sampleCount; i++)
	{
		res.push_back(Utility::GetOneRandomEnvUnit(eus));
	}
	return res;
}

vector<EnvUnit *> Utility::GetStratifiedRandomSamples( vector<EnvUnit *> envUnits, double factorValList[], int sampleCountList[], int n )
{
	vector<EnvUnit *> res;
	for (int i = 0; i < n; i++)
	{
		vector<EnvUnit*> randomSamplesFromOneFactor = Utility::GetRandomEnvUnitsByFactor(envUnits, factorValList[i], sampleCountList[i]);
		res.insert(res.end(), randomSamplesFromOneFactor.begin(), randomSamplesFromOneFactor.end());
	}
	std::random_shuffle(res.begin(), res.end());
	return res;
}

void Utility::WriteCSV_Temp( string filename, vector<EnvUnit *> envUnits )
{
	if(envUnits.size() <= 0)
	{
		return;
	}
	ofstream file(filename);
	string firstLine = "x,y,Density,DSimi\n";
	file<<firstLine;

	double cellSize = envUnits[0]->CellSize;
	for(int i = 0; i < envUnits.size(); i++)
	{
		double x = envUnits[i]->Loc->X + cellSize / 2;
		double y = envUnits[i]->Loc->Y - cellSize / 2;
		string xstr = Utility::ConvertToString(x);
		string ystr = Utility::ConvertToString(y);

		double density = envUnits[i]->Density;
		double dsimi = envUnits[i]->DSimi;
		string str_density = Utility::ConvertToString(density);
		string str_dsimi = Utility::ConvertToString(dsimi);

		string line = xstr + "," + ystr + "," + str_density + "," + str_dsimi + "\n";
		file<<line;
	}
	file.flush();
	file.close();
}

void Utility::WriteEnvDataCSV( string filename, vector<EnvUnit *> envUnits )
{
	if(envUnits.size() <= 0)
	{
		return;
	}
	ofstream file(filename);
	string firstLine = "x,y,env1,env2...\n";
	file<<firstLine;

	double cellSize = envUnits[0]->CellSize;
	EnvUnit *e = nullptr;
	for(int i = 0; i < envUnits.size(); i++)
	{
		string line = "";
		e = envUnits[i];
		if (!e->IsCal)
		{
			continue;
		}
		double x = e->Loc->X + cellSize / 2;
		double y = e->Loc->Y - cellSize / 2;
		string xstr = Utility::ConvertToString(x);
		string ystr = Utility::ConvertToString(y);
		line += xstr + "," + ystr;
		for (int j = 0; j < e->EnvValues.size(); j++)
		{
			double envVal = e->EnvValues[j];
			line += "," + Utility::ConvertToString(envVal);
		}
		line += "\n";
		file<<line;
	}
	e = nullptr;
	file.flush();
	file.close();
}

void Utility::ShowEnvUnit( vector<EnvUnit *> envUnits )
{
	if(envUnits.empty() || envUnits.size() <= 0)
	{
		return;
	}
	double cellSize = envUnits[0]->CellSize;
	cout<<"x,y\n";
	for (int i = 0; i < envUnits.size(); i++)
	{
		double x = envUnits[i]->Loc->X + cellSize / 2;
		double y = envUnits[i]->Loc->Y - cellSize / 2;
		cout<<std::fixed<<setprecision(0)<<x<<","<<y<<"\n";
	}
}

vector<EnvUnit *> Utility::ReadEnvDataFromCSV( string filename, int envCount )
{
	vector<EnvUnit*> envUnits;
	ifstream file(filename); // declare file stream:
	if (!file.is_open())
	{
		return envUnits;
	}

	// 处理第一行，获取X,Y属性在文件中的列数位置
	string line;

	// 根据x，y值读取样点信息
	while (getline(file, line))
	{
		vector<string> values;
		Utility::ParseStr(line, ',', values);
		EnvUnit *e = new EnvUnit();
		for (int i = 0; i < values.size(); i++)
		{
			double val = atof(values[i].c_str());
			e->EnvValues.push_back(val);
			e->DataTypes.push_back(DataTypeEnum::SINGLEVALUE);
		}
		envUnits.push_back(e);
	}
	file.close();
	return envUnits;
}
