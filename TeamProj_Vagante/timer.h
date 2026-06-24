#pragma once
#include <chrono>

class timer
{
private:
	bool _isHardware;
	float _timeScale;
	float _timeElapsed;

	std::chrono::high_resolution_clock::time_point _lastTime;
	std::chrono::high_resolution_clock::time_point _curTime;

	unsigned long _frameRate;
	unsigned long _FPSFrameCount;
	float _FPSTimeElapsed;
	float _worldTime;

public:
	HRESULT init(void);
	void tick(float lockFPS = 0.0f);
	unsigned long getFrameRate(char* str = NULL) const;

	inline float getElapsedTime(void) const { return _timeElapsed; }
	inline float getWorldTime(void) const { return _worldTime; }

	timer();
	~timer();
};
