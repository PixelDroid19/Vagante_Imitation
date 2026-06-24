#include "stdafx.h"
#include "animation.h"

animation::animation()
	: _frameNum(0),
	_frameWidth(0),
	_frameHeight(0),
	_loop(FALSE),
	_frameUpdateSec(0.0f),
	_elapsedSec(0.0f),
	_nowPlayIndex(0),
	_play(FALSE),
	_obj(NULL),
	_callbackFunction(NULL),
	_callbackFunctionParameter(NULL)
{
}

animation::~animation()
{
}

HRESULT animation::init(int totalW, int totalH, int frameW, int frameH)
{
	POINT pt;

	_frameWidth = frameW;
	_frameHeight = frameH;

	int frameX = totalW / frameW;
	int frameY = totalH / frameH;

	for (int y = 0; y < frameY; y++)
	{
		for (int x = 0; x < frameX; x++)
		{
			pt.x = x * frameW;
			pt.y = y * frameH;
			_frameList.push_back(pt);
		}
	}

	_frameNum = frameX * frameY;

	return S_OK;
}

void animation::release(void)
{
	_frameList.clear();
	_playList.clear();

	_frameNum = 0;
	_nowPlayIndex = 0;
	_play = FALSE;
}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop)
{
	_loop = loop;

	_playList.clear();

	for (DWORD i = 0; i < _frameList.size(); i++)
	{
		_playList.push_back(i);
	}

	if (reverse)
	{
		_playList.clear();

		for (int i = _frameNum - 1; i >= 0; i--)
		{
			_playList.push_back(i);
		}
	}
}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	setDefPlayFrame(reverse, loop);
	_callbackFunction = cbFunction;
}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj)
{
	setDefPlayFrame(reverse, loop);
	_callbackFunctionParameter = cbFunction;
	_obj = obj;
}

void animation::setPlayFrame(int* playArr, int arrLen, BOOL loop)
{
	_loop = loop;

	_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		_playList.push_back(playArr[i]);
	}
}

void animation::setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	setPlayFrame(playArr, arrLen, loop);
	_callbackFunction = cbFunction;
}

void animation::setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj)
{
	setPlayFrame(playArr, arrLen, loop);
	_callbackFunctionParameter = cbFunction;
	_obj = obj;
}

void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop)
{
	_loop = loop;

	_playList.clear();

	if (reverse)
	{
		for (int i = end; i >= start; i--)
		{
			_playList.push_back(i);
		}
	}
	else
	{
		for (int i = start; i <= end; i++)
		{
			_playList.push_back(i);
		}
	}
}

void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	setPlayFrame(start, end, reverse, loop);
	_callbackFunction = cbFunction;
}

void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj)
{
	setPlayFrame(start, end, reverse, loop);
	_callbackFunctionParameter = cbFunction;
	_obj = obj;
}

void animation::setFPS(int framePerSec)
{
	_frameUpdateSec = 1.0f / framePerSec;
}

void animation::frameUpdate(float elapsedTime)
{
	if (!_play) return;

	_elapsedSec += elapsedTime;

	if (_elapsedSec >= _frameUpdateSec)
	{
		_elapsedSec -= _frameUpdateSec;

		_nowPlayIndex++;

		if (_nowPlayIndex >= _playList.size())
		{
			if (_loop) _nowPlayIndex = 0;
			else
			{
				_nowPlayIndex = _playList.size() - 1;
				_play = FALSE;
				if (_callbackFunction != NULL) _callbackFunction();
				if (_callbackFunctionParameter != NULL) _callbackFunctionParameter(_obj);
			}
		}
	}
}

void animation::start(void)
{
	_play = TRUE;
	_nowPlayIndex = 0;
	_elapsedSec = 0.0f;
}

void animation::stop(void)
{
	_play = FALSE;
	_nowPlayIndex = 0;
	_elapsedSec = 0.0f;
}

void animation::pause(void)
{
	_play = FALSE;
}

void animation::resume(void)
{
	_play = TRUE;
}
