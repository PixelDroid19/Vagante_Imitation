#include "stdafx.h"
#include "txtData.h"
#include <fstream>

txtData::txtData()
{
}


txtData::~txtData()
{

}

HRESULT txtData::init()
{

	return S_OK;
}

void txtData::release()
{

}


// Save
void txtData::txtSave(const char* saveFileName, vector<string> vStr)
{
	std::ofstream file(saveFileName);
	if (!file.is_open()) return;

	file << vectorArrayCombine(vStr);

	file.close();
}

string txtData::vectorArrayCombine(vector<string> vArray)			
{
	string result;

	for (int i = 0; i < vArray.size(); i++)
	{
		result += vArray[i];
		if (i + 1 < vArray.size()) result += ",";
	}

	return result;
}


// Load
vector<string> txtData::txtLoad(const char* loadFileName)	 
{
	std::ifstream file(loadFileName);
	if (!file.is_open()) return vector<string>();

	char str[128] = {};
	file.read(str, 127);

	file.close();

	return charArraySeparation(str);
}

vector<string> txtData::charArraySeparation(char charArray[])
{
	vector<string> vArray;

	char* temp;
	const char* separator = ",";
	char* token;

	token = strtok(charArray, separator);

	if (token != NULL) vArray.push_back(token);

	while (NULL != (token = strtok(NULL, separator)))
	{
		vArray.push_back(token);
	}

	return vArray;
}
