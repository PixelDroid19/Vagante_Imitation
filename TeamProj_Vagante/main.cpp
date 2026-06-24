#include "stdafx.h"
#include "mainGame.h"
#include <sys/stat.h>
#include <unistd.h>

mainGame _mg;
SDL_Window* _hWnd = NULL;
SDL_Renderer* _renderer = NULL;
POINT _ptMouse = {0, 0};

static bool assetFileExists(const char* path)
{
	struct stat st;
	return stat(path, &st) == 0;
}

static void ensureAssetWorkingDirectory()
{
	if (assetFileExists("Img/map/Box.bmp"))
		return;

	if (assetFileExists("../Img/map/Box.bmp"))
	{
		chdir("..");
		return;
	}

	char* basePath = SDL_GetBasePath();
	if (basePath)
	{
		std::string parent = basePath;
		parent += "..";
		if (assetFileExists((parent + "/Img/map/Box.bmp").c_str()))
			chdir(parent.c_str());
		SDL_free(basePath);
	}
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
	{
		return 1;
	}

	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		SDL_Quit();
		return 1;
	}

	ensureAssetWorkingDirectory();

	_hWnd = SDL_CreateWindow(
		WINNAME,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINSIZEX,
		WINSIZEY,
		SDL_WINDOW_SHOWN);

	if (!_hWnd)
	{
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	_renderer = SDL_CreateRenderer(_hWnd, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!_renderer)
	{
		SDL_DestroyWindow(_hWnd);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	if (FAILED(_mg.init()))
	{
		SDL_DestroyRenderer(_renderer);
		SDL_DestroyWindow(_hWnd);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Event event;
	bool running = true;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					running = false;
				else if (event.key.keysym.scancode == SDL_SCANCODE_F4 &&
					(event.key.keysym.mod & KMOD_ALT))
					running = false;
				break;
			case SDL_MOUSEMOTION:
				_ptMouse.x = event.motion.x;
				_ptMouse.y = event.motion.y;
				break;
			}
		}

		KEYMANAGER->update();

		// 60 Hz fixed update: physics/animation are per-frame, not delta-scaled
		TIMEMANAGER->update(60.0f);
		_mg.update();
		_mg.render();

		SDL_RenderPresent(_renderer);
	}

	_mg.release();

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_hWnd);

	IMG_Quit();
	SDL_Quit();

	return 0;
}
