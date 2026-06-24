#pragma once
#include "singletonBase.h"
#include <vector>
#include <string>

class txtData : public singletonBase<txtData>
{
public:
	HRESULT init();
	void release();

	// Save
	void txtSave(const char* saveFileName, vector<string> vStr);
	string vectorArrayCombine(vector<string> vArray);

	// Load
	vector<string> txtLoad(const char* loadFileName);
	vector<string> charArraySeparation(char charArray[]);

	txtData();
	~txtData();
};
