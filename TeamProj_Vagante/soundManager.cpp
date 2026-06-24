#include "stdafx.h"
#include "soundManager.h"

soundManager::soundManager()
	: _audioDevice(0)
{
}

soundManager::~soundManager()
{
}

HRESULT soundManager::init(void)
{
	SDL_AudioSpec want;
	SDL_zero(want);
	want.freq = 44100;
	want.format = AUDIO_S16SYS;
	want.channels = 2;
	want.samples = 2048;

	_audioDevice = SDL_OpenAudioDevice(NULL, 0, &want, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
	if (_audioDevice == 0)
	{
		char buf[256];
		sprintf(buf, "Failed to open audio device: %s", SDL_GetError());
		return E_FAIL;
	}

	SDL_PauseAudioDevice(_audioDevice, 0);

	return S_OK;
}

void soundManager::release(void)
{
	for (arrSoundsIter it = _mTotalSounds.begin(); it != _mTotalSounds.end(); ++it)
	{
		if (it->second.wavBuffer)
		{
			if (it->second.deviceId > 0 && it->second.deviceId != _audioDevice)
			{
				SDL_CloseAudioDevice(it->second.deviceId);
			}
			SDL_FreeWAV(it->second.wavBuffer);
		}
	}
	_mTotalSounds.clear();

	if (_audioDevice > 0)
	{
		SDL_CloseAudioDevice(_audioDevice);
		_audioDevice = 0;
	}
}

void soundManager::update(void)
{
}

void soundManager::addSound(string keyName, string soundName, bool bgm, bool loop)
{
	SoundEntry entry;
	SDL_zero(entry);

	if (!SDL_LoadWAV(soundName.c_str(), &entry.wavSpec, &entry.wavBuffer, &entry.wavLength))
	{
		char buf[256];
		sprintf(buf, "Failed to load sound: %s, SDL error: %s",
			soundName.c_str(), SDL_GetError());
		return;
	}

	entry.isBGM = bgm;
	entry.deviceId = _audioDevice;

	_mTotalSounds.insert(pair<string, SoundEntry>(keyName, entry));
}

void soundManager::play(string keyName, float volume)
{
	arrSoundsIter it = _mTotalSounds.find(keyName);
	if (it == _mTotalSounds.end()) return;

	Uint8* buffer = it->second.wavBuffer;
	Uint32 length = it->second.wavLength;

	SDL_ClearQueuedAudio(_audioDevice);
	if (SDL_QueueAudio(_audioDevice, buffer, length) < 0)
	{
		return;
	}
}

void soundManager::stop(string keyName)
{
	SDL_ClearQueuedAudio(_audioDevice);
}

void soundManager::pause(string keyName)
{
	SDL_PauseAudioDevice(_audioDevice, 1);
}

void soundManager::resume(string keyName)
{
	SDL_PauseAudioDevice(_audioDevice, 0);
}

bool soundManager::isPlaySound(string keyName)
{
	return SDL_GetQueuedAudioSize(_audioDevice) > 0;
}

bool soundManager::isPauseSound(string keyName)
{
	return false;
}
