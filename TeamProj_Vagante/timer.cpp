#include "stdafx.h"
#include "timer.h"

timer::timer()
{
}

timer::~timer()
{
}

HRESULT timer::init(void)
{
	_isHardware = true;
	_lastTime = std::chrono::high_resolution_clock::now();
	_timeScale = 1.0f / 1000000.0f;

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0;

	return S_OK;
}

void timer::tick(float lockFPS)
{
	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed = now - _lastTime;

	long long elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	_timeElapsed = (float)(elapsedUs * _timeScale);

	if (lockFPS > 0.0f)
	{
		const float targetFrameTime = 1.0f / lockFPS;

		// Sleep-based frame cap (SDL wiki pattern); avoids busy-wait CPU burn
		while (_timeElapsed < targetFrameTime)
		{
			const float remaining = targetFrameTime - _timeElapsed;
			if (remaining > 0.002f)
				SDL_Delay((Uint32)((remaining - 0.001f) * 1000.0f));
			else
			{
				now = std::chrono::high_resolution_clock::now();
				elapsed = now - _lastTime;
				elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
				_timeElapsed = (float)(elapsedUs * _timeScale);
				continue;
			}

			now = std::chrono::high_resolution_clock::now();
			elapsed = now - _lastTime;
			elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
			_timeElapsed = (float)(elapsedUs * _timeScale);
		}

		// Fixed timestep: movement and _animCount (int) assume ~60 Hz updates
		_timeElapsed = targetFrameTime;
	}

	_lastTime = now;
	_FPSFrameCount++;
	_FPSTimeElapsed += _timeElapsed;
	_worldTime += _timeElapsed;

	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}
}

unsigned long timer::getFrameRate(char* str) const
{
	if (str != NULL)
	{
		sprintf(str, "FPS : %d", _frameRate);
	}

	return _frameRate;
}
