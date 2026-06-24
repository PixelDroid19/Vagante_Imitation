#pragma once
#include "singletonBase.h"
#include <map>

#define TOTALSOUNDBUFFER 30

class soundManager : public singletonBase<soundManager>
{
private:
	struct SoundEntry {
		Uint8* wavBuffer;
		Uint32 wavLength;
		SDL_AudioSpec wavSpec;
		SDL_AudioDeviceID deviceId;
		bool isBGM;
	};

	typedef map<string, SoundEntry> arrSounds;
	typedef map<string, SoundEntry>::iterator arrSoundsIter;

private:
	arrSounds _mTotalSounds;
	SDL_AudioDeviceID _audioDevice;

public:
	HRESULT init(void);
	void release(void);
	void update(void);

	void addSound(string keyName, string soundName, bool bgm, bool loop);
	void play(string keyName, float volume = 1.0f);
	void stop(string keyName);
	void pause(string keyName);
	void resume(string keyName);

	bool isPlaySound(string keyName);
	bool isPauseSound(string keyName);

	soundManager();
	~soundManager();
};
