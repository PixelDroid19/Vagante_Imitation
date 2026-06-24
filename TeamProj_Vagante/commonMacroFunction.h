#pragma once

inline POINT PointMake(int x, int y)
{
	POINT pt = { x, y };
	return pt;
}

inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2) };
	return rc;
}

inline void Rectangle(SDL_Renderer* renderer, int left, int top, int right, int bottom)
{
	SDL_Rect r = { left, top, right - left, bottom - top };
	SDL_RenderDrawRect(renderer, &r);
}
