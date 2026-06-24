#include "stdafx.h"
#include "image.h"
#include <cstring>

static void normalizePath(char* path)
{
	for (int i = 0; path[i]; i++)
		if (path[i] == '\\') path[i] = '/';
}

image::image()
	: _imageInfo(NULL),
	_fileName(NULL),
	_trans(false),
	_transColor(0)
{
}

image::~image()
{
}

int image::init(int width, int height, BOOL trans, COLORREF transColor)
{
	if (_imageInfo != NULL) release();

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->resID = 0;
	_imageInfo->texture = SDL_CreateTexture(_renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		width, height);
	_imageInfo->frameWidth = width;
	_imageInfo->frameHeight = height;
	_imageInfo->width = width;
	_imageInfo->height = height;

	_fileName = NULL;
	_trans = FALSE;
	_transColor = 0;

	if (_imageInfo->texture == NULL)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

int image::init(const char* fileName, int width, int height,
	BOOL trans, COLORREF transColor)
{
	if (fileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;

	char fixedPath[256];
	strncpy(fixedPath, fileName, sizeof(fixedPath) - 1);
	fixedPath[sizeof(fixedPath) - 1] = '\0';
	normalizePath(fixedPath);

	SDL_Surface* surface = IMG_Load(fixedPath);
	if (!surface)
	{
		release();
		return E_FAIL;
	}

	if (trans)
	{
		Uint32 colorKey = SDL_MapRGB(surface->format,
			(transColor >> 16) & 0xFF,
			(transColor >> 8) & 0xFF,
			transColor & 0xFF);
		SDL_SetColorKey(surface, SDL_TRUE, colorKey);
	}

	_imageInfo->texture = SDL_CreateTextureFromSurface(_renderer, surface);
	SDL_FreeSurface(surface);

	if (!_imageInfo->texture)
	{
		release();
		return E_FAIL;
	}

	_imageInfo->frameWidth = width;
	_imageInfo->frameHeight = height;
	_imageInfo->width = width;
	_imageInfo->height = height;

	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fixedPath);

	_trans = trans;
	_transColor = transColor;

	return S_OK;
}

int image::init(const char* fileName, float x, float y, int width, int height,
	BOOL trans, COLORREF transColor)
{
	if (fileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;

	char fixedPath[256];
	strncpy(fixedPath, fileName, sizeof(fixedPath) - 1);
	fixedPath[sizeof(fixedPath) - 1] = '\0';
	normalizePath(fixedPath);

	SDL_Surface* surface = IMG_Load(fixedPath);
	if (!surface)
	{
		release();
		return E_FAIL;
	}

	if (trans)
	{
		Uint32 colorKey = SDL_MapRGB(surface->format,
			(transColor >> 16) & 0xFF,
			(transColor >> 8) & 0xFF,
			transColor & 0xFF);
		SDL_SetColorKey(surface, SDL_TRUE, colorKey);
	}

	_imageInfo->texture = SDL_CreateTextureFromSurface(_renderer, surface);
	SDL_FreeSurface(surface);

	if (!_imageInfo->texture)
	{
		release();
		return E_FAIL;
	}

	_imageInfo->frameWidth = width;
	_imageInfo->frameHeight = height;
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;

	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fixedPath);

	_trans = trans;
	_transColor = transColor;

	return S_OK;
}

int image::init(const char* fileName, float x, float y, int width, int height,
	int frameX, int frameY, BOOL trans, COLORREF transColor)
{
	if (fileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;

	char fixedPath[256];
	strncpy(fixedPath, fileName, sizeof(fixedPath) - 1);
	fixedPath[sizeof(fixedPath) - 1] = '\0';
	normalizePath(fixedPath);

	SDL_Surface* surface = IMG_Load(fixedPath);
	if (!surface)
	{
		release();
		return E_FAIL;
	}

	if (trans)
	{
		Uint32 colorKey = SDL_MapRGB(surface->format,
			(transColor >> 16) & 0xFF,
			(transColor >> 8) & 0xFF,
			transColor & 0xFF);
		SDL_SetColorKey(surface, SDL_TRUE, colorKey);
	}

	_imageInfo->texture = SDL_CreateTextureFromSurface(_renderer, surface);
	SDL_FreeSurface(surface);

	if (!_imageInfo->texture)
	{
		release();
		return E_FAIL;
	}

	_imageInfo->x = x - (width / 2);
	_imageInfo->y = y - (height / 2);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fixedPath);

	_trans = trans;
	_transColor = transColor;

	return S_OK;
}

int image::init(const char* fileName, int width, int height,
	int frameX, int frameY, BOOL trans, COLORREF transColor)
{
	if (fileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;

	char fixedPath[256];
	strncpy(fixedPath, fileName, sizeof(fixedPath) - 1);
	fixedPath[sizeof(fixedPath) - 1] = '\0';
	normalizePath(fixedPath);

	SDL_Surface* surface = IMG_Load(fixedPath);
	if (!surface)
	{
		release();
		return E_FAIL;
	}

	if (trans)
	{
		Uint32 colorKey = SDL_MapRGB(surface->format,
			(transColor >> 16) & 0xFF,
			(transColor >> 8) & 0xFF,
			transColor & 0xFF);
		SDL_SetColorKey(surface, SDL_TRUE, colorKey);
	}

	_imageInfo->texture = SDL_CreateTextureFromSurface(_renderer, surface);
	SDL_FreeSurface(surface);

	if (!_imageInfo->texture)
	{
		release();
		return E_FAIL;
	}

	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fixedPath);

	_trans = trans;
	_transColor = transColor;

	return S_OK;
}

void image::release(void)
{
	if (_imageInfo)
	{
		if (_imageInfo->texture)
		{
			SDL_DestroyTexture(_imageInfo->texture);
			_imageInfo->texture = NULL;
		}

		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);

		_trans = false;
		_transColor = 0;
	}
}

void image::setTransColor(BOOL trans, COLORREF transColor)
{
	_trans = trans;
	_transColor = transColor;
}

void image::render(SDL_Renderer* renderer)
{
	SDL_Rect dest;
	dest.x = (int)_imageInfo->x;
	dest.y = (int)_imageInfo->y;
	dest.w = _imageInfo->width;
	dest.h = _imageInfo->height;

	SDL_RenderCopy(renderer, _imageInfo->texture, NULL, &dest);
}

void image::render(SDL_Renderer* renderer, int destX, int destY)
{
	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = _imageInfo->width;
	dest.h = _imageInfo->height;

	SDL_RenderCopy(renderer, _imageInfo->texture, NULL, &dest);
}

void image::render(SDL_Renderer* renderer, int destX, int destY,
	int sourX, int sourY, int sourWidth, int sourHeight)
{
	SDL_Rect src = { sourX, sourY, sourWidth, sourHeight };
	SDL_Rect dest = { destX, destY, sourWidth, sourHeight };

	SDL_RenderCopy(renderer, _imageInfo->texture, &src, &dest);
}

void image::frameRender(SDL_Renderer* renderer, int destX, int destY)
{
	SDL_Rect src;
	src.x = _imageInfo->currentFrameX * _imageInfo->frameWidth;
	src.y = _imageInfo->currentFrameY * _imageInfo->frameHeight;
	src.w = _imageInfo->frameWidth;
	src.h = _imageInfo->frameHeight;

	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = _imageInfo->frameWidth;
	dest.h = _imageInfo->frameHeight;

	SDL_RenderCopy(renderer, _imageInfo->texture, &src, &dest);
}

void image::frameRender(SDL_Renderer* renderer, int destX, int destY,
	int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	SDL_Rect src;
	src.x = currentFrameX * _imageInfo->frameWidth;
	src.y = currentFrameY * _imageInfo->frameHeight;
	src.w = _imageInfo->frameWidth;
	src.h = _imageInfo->frameHeight;

	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = _imageInfo->frameWidth;
	dest.h = _imageInfo->frameHeight;

	SDL_RenderCopy(renderer, _imageInfo->texture, &src, &dest);
}

void image::alphaFrameRender(SDL_Renderer* renderer, int destX, int destY, BYTE alpha)
{
	SDL_SetTextureAlphaMod(_imageInfo->texture, alpha);

	SDL_Rect src;
	src.x = _imageInfo->currentFrameX * _imageInfo->frameWidth;
	src.y = _imageInfo->currentFrameY * _imageInfo->frameHeight;
	src.w = _imageInfo->frameWidth;
	src.h = _imageInfo->frameHeight;

	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = _imageInfo->frameWidth;
	dest.h = _imageInfo->frameHeight;

	SDL_RenderCopy(renderer, _imageInfo->texture, &src, &dest);

	SDL_SetTextureAlphaMod(_imageInfo->texture, 255);
}

void image::alphaFrameRender(SDL_Renderer* renderer, int destX, int destY,
	int currentFrameX, int currentFrameY, BYTE alpha)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	SDL_SetTextureAlphaMod(_imageInfo->texture, alpha);

	SDL_Rect src;
	src.x = currentFrameX * _imageInfo->frameWidth;
	src.y = currentFrameY * _imageInfo->frameHeight;
	src.w = _imageInfo->frameWidth;
	src.h = _imageInfo->frameHeight;

	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = _imageInfo->frameWidth;
	dest.h = _imageInfo->frameHeight;

	SDL_RenderCopy(renderer, _imageInfo->texture, &src, &dest);

	SDL_SetTextureAlphaMod(_imageInfo->texture, 255);
}

void image::loopRender(SDL_Renderer* renderer, const RECT* drawArea, int offSetX, int offSetY)
{
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	RECT rcSour;
	int sourWidth;
	int sourHeight;

	RECT rcDest;

	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			render(renderer, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top,
				rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top);
		}
	}
}

void image::alphaRender(SDL_Renderer* renderer, BYTE alpha)
{
	SDL_SetTextureAlphaMod(_imageInfo->texture, alpha);

	SDL_Rect dest;
	dest.x = (int)_imageInfo->x;
	dest.y = (int)_imageInfo->y;
	dest.w = _imageInfo->width;
	dest.h = _imageInfo->height;

	SDL_RenderCopy(renderer, _imageInfo->texture, NULL, &dest);

	SDL_SetTextureAlphaMod(_imageInfo->texture, 255);
}

void image::alphaRender(SDL_Renderer* renderer, int destX, int destY, BYTE alpha)
{
	SDL_SetTextureAlphaMod(_imageInfo->texture, alpha);

	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = _imageInfo->width;
	dest.h = _imageInfo->height;

	SDL_RenderCopy(renderer, _imageInfo->texture, NULL, &dest);

	SDL_SetTextureAlphaMod(_imageInfo->texture, 255);
}

void image::alphaRender(SDL_Renderer* renderer, int destX, int destY,
	int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	SDL_SetTextureAlphaMod(_imageInfo->texture, alpha);

	SDL_Rect src = { sourX, sourY, sourWidth, sourHeight };
	SDL_Rect dest = { destX, destY, sourWidth, sourHeight };

	SDL_RenderCopy(renderer, _imageInfo->texture, &src, &dest);

	SDL_SetTextureAlphaMod(_imageInfo->texture, 255);
}

void image::aniRender(SDL_Renderer* renderer, int destX, int destY, animation* ani)
{
	render(renderer, destX, destY, ani->getFramePos().x, ani->getFramePos().y,
		ani->getFrameWidth(), ani->getFrameHeight());
}
