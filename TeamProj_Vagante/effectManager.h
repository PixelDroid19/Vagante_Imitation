#pragma once
#include "singletonBase.h"
#include <vector>
#include <map>

class effect;

class effectManager : public singletonBase <effectManager>
{
private:
	// Effect
	typedef vector<effect*> arrEffects;
	typedef vector<effect*>::iterator iterEffects;
	// Effects
	typedef map<string, arrEffects> arrEffect;
	typedef map<string, arrEffects>::iterator iterEffect;
	// Total effects
	typedef vector<map<string, arrEffects>> arrTotalEffect;
	typedef vector<map<string, arrEffects>>::iterator iterTotalEffect;

private:
	arrTotalEffect _vTotalEffect;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void addEffect(string effectName, const char* imageName, int imageWidth, int imageHeight, int effectWidth, int effectHeight, int fps, float elapsedTime, int buffer);

	void play(string effectName, int x, int y);


	effectManager();
	~effectManager();
};
