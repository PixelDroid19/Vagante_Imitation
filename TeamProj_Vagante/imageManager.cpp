#include "stdafx.h"
#include "imageManager.h"

imageManager::imageManager()
{
}

imageManager::~imageManager()
{
}

HRESULT imageManager::init()
{
	return S_OK;
}

void imageManager::release()
{
	deleteAll();
}

image* imageManager::addImage(string strKey, int width, int height)
{
	image* img = findImage(strKey);
	if (img) return img;

	img = new image;
	img->init(width, height);
	_mImageList.insert(pair<string, image*>(strKey, img));

	return img;
}

image* imageManager::addImage(string strKey, const char* fileName, int width, int height, bool trans, COLORREF transColor)
{
	image* img = findImage(strKey);
	if (img) return img;

	img = new image;
	img->init(fileName, width, height, trans, transColor);
	_mImageList.insert(pair<string, image*>(strKey, img));

	return img;
}

image* imageManager::addImage(string strKey, const char* fileName, float x, float y, int width, int height, bool trans, COLORREF transColor)
{
	image* img = findImage(strKey);
	if (img) return img;

	img = new image;
	img->init(fileName, x, y, width, height, trans, transColor);
	_mImageList.insert(pair<string, image*>(strKey, img));

	return img;
}

image* imageManager::addFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor)
{
	image* img = findImage(strKey);
	if (img) return img;

	img = new image;
	img->init(fileName, x, y, width, height, frameX, frameY, trans, transColor);
	_mImageList.insert(pair<string, image*>(strKey, img));

	return img;
}

image* imageManager::addFrameImage(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor)
{
	image* img = findImage(strKey);
	if (img) return img;

	img = new image;
	img->init(fileName, width, height, frameX, frameY, trans, transColor);
	_mImageList.insert(pair<string, image*>(strKey, img));

	return img;
}

image* imageManager::findImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);
	if (key != _mImageList.end()) return key->second;

	return NULL;
}

BOOL imageManager::deleteImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);
	if (key != _mImageList.end())
	{
		key->second->release();
		SAFE_DELETE(key->second);
		_mImageList.erase(key);

		return TRUE;
	}

	return FALSE;
}

BOOL imageManager::deleteAll(void)
{
	mapImageIter key;
	for (key = _mImageList.begin(); key != _mImageList.end(); )
	{
		key->second->release();
		SAFE_DELETE(key->second);
		key = _mImageList.erase(key);
	}

	return TRUE;
}

void imageManager::render(string strKey, SDL_Renderer* renderer, int destX, int destY)
{
	image* img = findImage(strKey);
	if (img) img->render(renderer, destX, destY);
}

void imageManager::render(string strKey, SDL_Renderer* renderer, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	image* img = findImage(strKey);
	if (img) img->render(renderer, destX, destY, sourX, sourY, sourWidth, sourHeight);
}

void imageManager::frameRender(string strKey, SDL_Renderer* renderer, int destX, int destY)
{
	image* img = findImage(strKey);
	if (img) img->frameRender(renderer, destX, destY);
}

void imageManager::frameRender(string strKey, SDL_Renderer* renderer, int destX, int destY, int currentFrameX, int currentFrameY)
{
	image* img = findImage(strKey);
	if (img) img->frameRender(renderer, destX, destY, currentFrameX, currentFrameY);
}

void imageManager::alphaFrameRender(string strKey, SDL_Renderer* renderer, int destX, int destY, BYTE alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaFrameRender(renderer, destX, destY, alpha);
}

void imageManager::alphaFrameRender(string strKey, SDL_Renderer* renderer, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaFrameRender(renderer, destX, destY, currentFrameX, currentFrameY, alpha);
}

void imageManager::loopRender(string strKey, SDL_Renderer* renderer, const RECT* drawArea, int offSetX, int offSetY)
{
	image* img = findImage(strKey);
	if (img) img->loopRender(renderer, drawArea, offSetX, offSetY);
}

void imageManager::alphaRender(string strKey, SDL_Renderer* renderer, BYTE alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaRender(renderer, alpha);
}

void imageManager::alphaRender(string strKey, SDL_Renderer* renderer, int destX, int destY, BYTE alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaRender(renderer, destX, destY, alpha);
}

void imageManager::alphaRender(string strKey, SDL_Renderer* renderer, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaRender(renderer, destX, destY, sourX, sourY, sourWidth, sourHeight, alpha);
}
