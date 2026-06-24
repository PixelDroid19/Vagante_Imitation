#pragma once
#include "image.h"

class gameNode
{
private:
	SDL_Renderer* _renderer;
	bool _managerInit;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(bool managerInit);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	SDL_Renderer* getMemDC() { return ::_renderer; }

	gameNode();
	virtual ~gameNode();
};
