#include "stdafx.h"
#include "keyManager.h"

keyManager::keyManager()
{
}

keyManager::~keyManager()
{
}

HRESULT keyManager::init()
{
	for (int i = 0; i < KEYMAX; i++)
	{
		_keyDown.set(i, false);
		_keyUp.set(i, false);
	}

	return S_OK;
}

void keyManager::release()
{
}

void keyManager::update()
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	static Uint8 prevState[KEYMAX] = {0};

	for (int i = 0; i < KEYMAX; i++)
	{
		bool current = state[i] != 0;
		bool prev = prevState[i] != 0;

		// Edge detection: pressed/released this frame only
		_keyDown.set(i, current && !prev);
		_keyUp.set(i, !current && prev);

		prevState[i] = state[i];
	}

	SDL_Keymod mod = SDL_GetModState();

	if (mod & KMOD_SHIFT) _keyDown.set(KEYMAX - 1, true);
	else _keyDown.set(KEYMAX - 1, false);

	if (mod & KMOD_CTRL) _keyDown.set(KEYMAX - 2, true);
	else _keyDown.set(KEYMAX - 2, false);
}

bool keyManager::isOnceKeyDown(int key)
{
	if (key < 0 || key >= KEYMAX)
		return false;

	return _keyDown[key];
}

bool keyManager::isOnceKeyUp(int key)
{
	if (key < 0 || key >= KEYMAX)
		return false;

	return _keyUp[key];
}

bool keyManager::isStayKeyDown(int key)
{
	if (key < 0 || key >= KEYMAX)
		return false;

	return SDL_GetKeyboardState(NULL)[key] != 0;
}

bool keyManager::isToggleKey(int key)
{
	return false;
}
