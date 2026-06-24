#pragma once
#include "animation.h"

class image
{
public:
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE,
		LOAD_FILE,
		LOAD_EMPTY,
		LOAD_END
	};

	typedef struct tagImageInfo
	{
		int resID;
		SDL_Texture* texture;
		float x;
		float y;
		int width;
		int height;
		int currentFrameX;
		int currentFrameY;
		int maxFrameX;
		int maxFrameY;
		int frameWidth;
		int frameHeight;
		BYTE loadType;

		tagImageInfo()
		{
			resID = 0;
			texture = NULL;
			x = 0;
			y = 0;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			loadType = LOAD_RESOURCE;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO _imageInfo;
	char* _fileName;
	BOOL _trans;
	COLORREF _transColor;

public:
	image();
	~image();

	int init(int width, int height, BOOL trans = FALSE, COLORREF transColor = 0);
	int init(const char* fileName, int width, int height,
		BOOL trans = FALSE, COLORREF transColor = 0);

	int init(const char* fileName, float x, float y, int width, int height,
		BOOL trans = FALSE, COLORREF transColor = 0);

	int init(const char* fileName, float x, float y, int width, int height,
		int frameX, int frameY, BOOL trans, COLORREF transColor);

	int init(const char* fileName, int width, int height,
		int frameX, int frameY, BOOL trans, COLORREF transColor);

	void release(void);

	void setTransColor(BOOL trans, COLORREF transColor);

	void render(SDL_Renderer* renderer);
	void render(SDL_Renderer* renderer, int destX, int destY);
	void render(SDL_Renderer* renderer, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	void frameRender(SDL_Renderer* renderer, int destX, int destY);
	void frameRender(SDL_Renderer* renderer, int destX, int destY, int currentFrameX, int currentFrameY);
	void alphaFrameRender(SDL_Renderer* renderer, int destX, int destY, BYTE alpha);
	void alphaFrameRender(SDL_Renderer* renderer, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha);

	void loopRender(SDL_Renderer* renderer, const RECT* drawArea, int offSetX, int offSetY);

	void alphaRender(SDL_Renderer* renderer, BYTE alpha);
	void alphaRender(SDL_Renderer* renderer, int destX, int destY, BYTE alpha);
	void alphaRender(SDL_Renderer* renderer, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);

	void aniRender(SDL_Renderer* renderer, int destX, int destY, animation* ani);

	inline SDL_Texture* getTexture() { return _imageInfo->texture; }

	inline void setX(float x) { _imageInfo->x = x; }
	inline float getX() { return _imageInfo->x; }

	inline void setY(float y) { _imageInfo->y = y; }
	inline float getY() { return _imageInfo->y; }

	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	inline float getCenterX()
	{
		return _imageInfo->maxFrameX <= 0 ? _imageInfo->x + (_imageInfo->width / 2) :
			_imageInfo->x + (_imageInfo->frameWidth / 2);
	}

	inline float getCenterY()
	{
		return _imageInfo->maxFrameY <= 0 ? _imageInfo->y + (_imageInfo->height / 2) :
			_imageInfo->y + (_imageInfo->frameHeight / 2);
	}

	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (_imageInfo->maxFrameX < frameX) _imageInfo->maxFrameX = frameX;
	}

	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (_imageInfo->maxFrameY < frameY) _imageInfo->maxFrameY = frameY;
	}

	inline int getMaxFrameX(void) { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY(void) { return _imageInfo->maxFrameY; }

	inline int getFrameX(void) { return _imageInfo->currentFrameX; }
	inline int getFrameY(void) { return _imageInfo->currentFrameY; }

	inline int getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline int getFrameHeight(void) { return _imageInfo->frameHeight; }

	inline int getWidth(void) { return _imageInfo->width; }
	inline int getHeight(void) { return _imageInfo->height; }
};
