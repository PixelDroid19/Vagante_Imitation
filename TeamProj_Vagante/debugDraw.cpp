#include "stdafx.h"
#include "debugDraw.h"

static bool g_debugDrawEnabled = false;
static int g_debugDrawLayers = DEBUG_LAYER_ALL;

bool isDebugDrawEnabled()
{
	return g_debugDrawEnabled;
}

void setDebugDrawEnabled(bool enabled)
{
	g_debugDrawEnabled = enabled;
}

void toggleDebugDraw()
{
	g_debugDrawEnabled = !g_debugDrawEnabled;
}

bool isDebugLayerEnabled(int layer)
{
	return g_debugDrawEnabled && (g_debugDrawLayers & layer);
}

void setDebugDrawLayers(int layers)
{
	g_debugDrawLayers = layers;
}

void toggleDebugLayer(int layer)
{
	g_debugDrawLayers ^= layer;
}

static void setDrawColor(SDL_Renderer* renderer, DebugColor color)
{
	SDL_SetRenderDrawBlendMode(renderer, color.a < 255 ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void debugDrawRect(SDL_Renderer* renderer, int left, int top, int right, int bottom,
	Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	if (!renderer || right <= left || bottom <= top)
		return;

	setDrawColor(renderer, { red, green, blue, alpha });
	SDL_Rect r = { left, top, right - left, bottom - top };
	SDL_RenderDrawRect(renderer, &r);
}

void debugDrawFilledRect(SDL_Renderer* renderer, int left, int top, int right, int bottom,
	Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	if (!renderer || right <= left || bottom <= top)
		return;

	setDrawColor(renderer, { red, green, blue, alpha });
	SDL_Rect r = { left, top, right - left, bottom - top };
	SDL_RenderFillRect(renderer, &r);
}

void debugDrawRect(SDL_Renderer* renderer, const RECT& rc, POINT camera, DebugColor color)
{
	debugDrawRect(renderer,
		rc.left + camera.x, rc.top + camera.y,
		rc.right + camera.x, rc.bottom + camera.y,
		color.r, color.g, color.b, color.a);
}

void debugDrawFilledRect(SDL_Renderer* renderer, const RECT& rc, POINT camera, DebugColor color)
{
	debugDrawFilledRect(renderer,
		rc.left + camera.x, rc.top + camera.y,
		rc.right + camera.x, rc.bottom + camera.y,
		color.r, color.g, color.b, color.a);
}

void debugDrawRect(SDL_Renderer* renderer, const IOTA_UTIL::MYRECT& rc, POINT camera, DebugColor color)
{
	debugDrawRect(renderer,
		(int)rc.left + camera.x, (int)rc.top + camera.y,
		(int)rc.right + camera.x, (int)rc.bottom + camera.y,
		color.r, color.g, color.b, color.a);
}

void debugDrawPoint(SDL_Renderer* renderer, int x, int y, int size, DebugColor color)
{
	debugDrawFilledRect(renderer, x - size, y - size, x + size, y + size,
		color.r, color.g, color.b, color.a);
	setDrawColor(renderer, { color.r, color.g, color.b, 255 });
	SDL_Rect r = { x - size, y - size, size * 2, size * 2 };
	SDL_RenderDrawRect(renderer, &r);
}

void debugDrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, DebugColor color)
{
	if (!renderer)
		return;

	setDrawColor(renderer, color);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void debugDrawCrosshair(SDL_Renderer* renderer, int x, int y, int size, DebugColor color)
{
	debugDrawLine(renderer, x - size, y, x + size, y, color);
	debugDrawLine(renderer, x, y - size, x, y + size, color);
	debugDrawPoint(renderer, x, y, 2, color);
}
