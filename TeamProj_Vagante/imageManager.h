#pragma once
#include "singletonBase.h"
#include "image.h"
#include <map>

class imageManager : public singletonBase<imageManager>
{
private:
	typedef map<string, image*> mapImageList;
	typedef map<string, image*>::iterator mapImageIter;

private:
	mapImageList _mImageList;

public:
	HRESULT init();
	void release();

	image* addImage(string strKey, int width, int height);
	image* addImage(string strKey, const char* fileName, int width, int height, bool trans, COLORREF transColor);
	image* addImage(string strKey, const char* fileName, float x, float y, int width, int height, bool trans, COLORREF transColor);

	image* addFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor);
	image* addFrameImage(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor);

	image* findImage(string strKey);

	BOOL deleteImage(string strKey);

	BOOL deleteAll(void);

	void render(string strKey, SDL_Renderer* renderer, int destX, int destY);
	void render(string strKey, SDL_Renderer* renderer, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);
	void frameRender(string strKey, SDL_Renderer* renderer, int destX, int destY);
	void frameRender(string strKey, SDL_Renderer* renderer, int destX, int destY, int currentFrameX, int currentFrameY);
	void alphaFrameRender(string strKey, SDL_Renderer* renderer, int destX, int destY, BYTE alpha);
	void alphaFrameRender(string strKey, SDL_Renderer* renderer, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha);

	void loopRender(string strKey, SDL_Renderer* renderer, const RECT* drawArea, int offSetX, int offSetY);
	void alphaRender(string strKey, SDL_Renderer* renderer, BYTE alpha);
	void alphaRender(string strKey, SDL_Renderer* renderer, int destX, int destY, BYTE alpha);
	void alphaRender(string strKey, SDL_Renderer* renderer, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);

	imageManager();
	~imageManager();
};
