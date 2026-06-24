#include "stdafx.h"
#include "iniDataManager.h"

iniDataManager::iniDataManager()
{
}

iniDataManager::~iniDataManager()
{
}

HRESULT iniDataManager::init()
{
	return S_OK;
}

void iniDataManager::release()
{
}

void iniDataManager::addData(const char* subject, const char* title, const char* body)
{
	tagIniData iniData;
	iniData.subject = subject;
	iniData.title = title;
	iniData.body = body;

	arrIniData vIniData;
	vIniData.push_back(iniData);

	_vIniData.push_back(vIniData);
}

void iniDataManager::iniSave(char* fileName)
{
	char dir[512];

	sprintf(dir, "%s.ini", fileName);

	FILE* file = fopen(dir, "w");
	if (!file) return;

	for (int i = 0; i < (int)_vIniData.size(); i++)
	{
		arrIniData vData = _vIniData[i];
		fprintf(file, "[%s]\n", vData[0].subject);
		fprintf(file, "%s=%s\n", vData[0].title, vData[0].body);

		vData.clear();
	}

	fclose(file);

	_vIniData.clear();
}

char* iniDataManager::loadDataString(const char* fileName, const char* subject, const char* title)
{
	static char data[256];
	char dir[512];
	sprintf(dir, "%s.ini", fileName);

	data[0] = '\0';

	FILE* file = fopen(dir, "r");
	if (!file) return data;

	char line[256];
	char currentSubject[256] = "";
	bool foundSubject = false;

	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == '[')
		{
			char* end = strchr(line + 1, ']');
			if (end)
			{
				*end = '\0';
				strcpy(currentSubject, line + 1);
				foundSubject = (strcmp(currentSubject, subject) == 0);
			}
			continue;
		}

		if (foundSubject)
		{
			char* eq = strchr(line, '=');
			if (eq)
			{
				*eq = '\0';
				char* value = eq + 1;
				char* newline = strchr(value, '\n');
				if (newline) *newline = '\0';
				newline = strchr(value, '\r');
				if (newline) *newline = '\0';

				if (strcmp(line, title) == 0)
				{
					strcpy(data, value);
					fclose(file);
					return data;
				}
			}
		}
	}

	fclose(file);
	return data;
}

int iniDataManager::loadDataInterger(const char* fileName, const char* subject, const char* title)
{
	char* str = loadDataString(fileName, subject, title);
	return atoi(str);
}
