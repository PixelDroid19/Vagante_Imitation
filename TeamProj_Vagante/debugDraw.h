#pragma once

#include <SDL.h>
#include "collision.h"

struct DebugColor { Uint8 r, g, b, a; };

namespace DebugPalette
{
	constexpr DebugColor Grid      = {  70, 140, 255, 140 };
	constexpr DebugColor Wall      = {   0, 220, 255, 255 };
	constexpr DebugColor WallFill  = {   0, 180, 255,  28 };
	constexpr DebugColor Ladder    = { 200,  80, 255, 255 };
	constexpr DebugColor Trap      = { 255, 140,  40, 255 };
	constexpr DebugColor Player    = { 100, 220, 255, 255 };
	constexpr DebugColor PlayerFill= { 100, 220, 255,  35 };
	constexpr DebugColor Weapon    = { 255, 180,  60, 255 };
	constexpr DebugColor Enemy     = {  80, 255,  80, 255 };
	constexpr DebugColor EnemyCore = { 255, 160,  40, 255 };
	constexpr DebugColor Detect    = { 255,  80, 200,  40 };
	constexpr DebugColor DetectLine= { 255,  80, 200, 180 };
	constexpr DebugColor ItemBox   = { 255, 230,  60, 255 };
	constexpr DebugColor CoinBox   = { 255, 150,  50, 255 };
	constexpr DebugColor Path      = { 180, 255, 255, 255 };
	constexpr DebugColor Ray       = { 120, 200, 255, 160 };
	constexpr DebugColor RayTarget = { 200, 100, 255, 200 };
	constexpr DebugColor Attack    = { 255,  60,  60, 220 };
}

bool isDebugDrawEnabled();
void setDebugDrawEnabled(bool enabled);
void toggleDebugDraw();

enum DebugDrawLayer
{
	DEBUG_LAYER_GRID     = 1,
	DEBUG_LAYER_TILES    = 2,
	DEBUG_LAYER_PLAYER_9 = 4,
	DEBUG_LAYER_RAYS     = 8,
	DEBUG_LAYER_PATH     = 16,
	DEBUG_LAYER_HITBOX   = 32,
	DEBUG_LAYER_ALL      = 63
};

bool isDebugLayerEnabled(int layer);
void setDebugDrawLayers(int layers);
void toggleDebugLayer(int layer);

void debugDrawRect(SDL_Renderer* renderer, int left, int top, int right, int bottom,
	Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255);
void debugDrawFilledRect(SDL_Renderer* renderer, int left, int top, int right, int bottom,
	Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 160);
void debugDrawRect(SDL_Renderer* renderer, const RECT& rc, POINT camera, DebugColor color);
void debugDrawFilledRect(SDL_Renderer* renderer, const RECT& rc, POINT camera, DebugColor color);
void debugDrawRect(SDL_Renderer* renderer, const IOTA_UTIL::MYRECT& rc, POINT camera, DebugColor color);
void debugDrawPoint(SDL_Renderer* renderer, int x, int y, int size, DebugColor color);
void debugDrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, DebugColor color);
void debugDrawCrosshair(SDL_Renderer* renderer, int x, int y, int size, DebugColor color);
