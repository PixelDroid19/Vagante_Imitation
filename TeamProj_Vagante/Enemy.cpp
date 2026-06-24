#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "UI.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}


HRESULT Enemy::init()
{
	return S_OK;
}
HRESULT Enemy::init(POINT point, float minCog, float maxCog)
{
	//Initialize enemy recognition range
	//Set min/max recognition range
	_minCog = minCog;
	_maxCog = maxCog;
	//Set position
	_pointx = point.x;
	_pointy = point.y;

	//Initialize animation
	_frameFPS = 0;
	_frameTime = 0;
	_currentFrameX = _currentFrameY = 0;

	//Initialize physics
	_xspeed = _yspeed = _angle = _gravity = 0;
	//Initialize money amount
	_money = 0;
	//Initialize player detection state
	_isFindPlayer = false;
	//_statusEffect[5]
	//Initialize status effects
	for (int i = 0; i < 5; i++)
	{
		_statusEffect[i].damage = 0;
		_statusEffect[i].leftTime = 0;
		_statusEffect[i].type = STATUSEFFECT_NULL;
	}
	//Initialize stats
	memset(&_statistics, 0, sizeof(tagStat));
	//Set initial state to idle
	_state = ENEMYSTATE_IDLE;
	//Set rect, attackRect, lastPlayerPoint for collision detection
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	_attackRect = RectMakeCenter(_pointx, _pointy, 1, 1);
	_lastPlayerPoint = _player->getPoint();
	//Initialize death state
	_dead = false;
	_deadAlpha = 0;

	return S_OK;
}
void Enemy::release()
{

}
void Enemy::update()
{
	//If player is found, move toward the player
	//Otherwise, check if player is within recognition range
	attRectClear();
	//Apply status effects
	statusEffect();
	//Fall handling
	falling();
	//If dead, fade out
	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		if (_deadAlpha >= 255)
		{
			_dead = true;
		}
	}
	//If player is found and not dead, execute AI
	if (false)
	{
		//If player is farther than min range, move toward them
		//If within min range, set isFindPlayer to true
		_isFindPlayer = true;
		//Move
		move();
		//Jump
		jump();

		//Attack
		attack();

		//If player goes beyond max range, stop searching
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > _maxCog)
			_isFindPlayer = false;

	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//Note: _lastPlayerPoint and _player->getPoint() are different!~~//
		///////////////////////////////////////////////////////////////////////////////////////

		//Re-check line of sight when player is within recognition range
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < _minCog)
		{
			if (static_cast<int>(_pointx / TILESIZE) == static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) == static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//If on the same tile, detect immediately without line of sight check
				_isFindPlayer = true;
			}
			else
			{
				//If not on the same tile, line of sight check via virtual function (polymorphism)
				playerCog();
			}
		}
	}
	//Update frame and resize rect for rendering
	frameUpdate();
	rectResize();
}
void Enemy::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}

//Draw function x coordinate is camera.x offset
//			y coordinate is camera.y offset!!!!
void Enemy::render(POINT camera)
{
	draw(camera);
}
void Enemy::draw(POINT camera)
{

}

void Enemy::drawDebug(POINT camera)
{
	if (!isDebugLayerEnabled(DEBUG_LAYER_HITBOX))
		return;

	SDL_Renderer* dc = getMemDC();
	const int cx = (int)_pointx + camera.x;
	const int cy = (int)_pointy + camera.y;
	const int minR = (int)_minCog;
	const int maxR = (int)std::min(_maxCog, (float)(TILESIZE * 12));

	debugDrawRect(dc, _rc, camera, DebugPalette::Enemy);
	debugDrawRect(dc, _attackRect, camera, DebugPalette::Attack);
	debugDrawCrosshair(dc, cx, cy, 4, DebugPalette::EnemyCore);

	if (_maxCog <= TILESIZE * 12)
	{
		if (minR > 0)
			debugDrawRect(dc, cx - minR, cy - minR, cx + minR, cy + minR,
				DebugPalette::DetectLine.r, DebugPalette::DetectLine.g, DebugPalette::DetectLine.b, 180);
		if (maxR > minR)
			debugDrawRect(dc, cx - maxR, cy - maxR, cx + maxR, cy + maxR,
				DebugPalette::DetectLine.r, DebugPalette::DetectLine.g, DebugPalette::DetectLine.b, 120);
	}

	if (_player && _isFindPlayer)
	{
		int px = _player->getPoint().x + camera.x;
		int py = _player->getPoint().y + camera.y;
		debugDrawLine(dc, cx, cy, px, py, DebugPalette::DetectLine);
	}
}
void Enemy::move()
{

}
void Enemy::jump()
{

}
void Enemy::attack()
{

}
void Enemy::addStatusEffect(tagStatusEffect statuseffect)
{
	//Add status effect!
	for (int i = 0; i < 5; i++)
	{
		if (_statusEffect[i].type == NULL)
		{
			_statusEffect[i] = statuseffect;
			break;
		}
	}
}


void Enemy::statusEffect()
{
	//Apply status effect over time
	for (int i = 0; i < 5; i++)
	{
		if (_statusEffect[i].type == NULL) continue;

		switch (_statusEffect[i].type)
		{
		case STATUSEFFECT_POISON:
			break;
		case STATUSEFFECT_FIRE:
			break;
		case STATUSEFFECT_STUN:
			break;
		case STATUSEFFECT_HEAL:
			break;
		}
	}
}

void Enemy::falling()
{
}

void Enemy::rectResize()
{
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}

void Enemy::playerCog()
{
	// Re-check line of sight only when the player has moved
	if (_lastPlayerPoint.x != _player->getPoint().x && _lastPlayerPoint.y != _player->getPoint().y)
	{
		// Remember player position for next comparison
		_lastPlayerPoint = _player->getPoint();
		// Number of walls blocking the ray
		int count = 0;
		// Last sampled tile coordinates along the ray
		float x = 0;
		float y = 0;
		// Distance and angle from enemy to player
		float dist = getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
		float angle = getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
		// Step along the ray in tile-sized increments
		for (int i = 0; i < dist; i += TILESIZE)
		{
			// Convert world position to tile coordinates
			float ox = (_pointx + i*cosf(angle)) / TILESIZE;
			float oy = (_pointy + i*-sinf(angle)) / TILESIZE;

			if (ox == x && oy == y) continue;

			// Advance to the next tile along the ray
			x = ox;
			y = oy;

			// Wall blocks line of sight (row = y, col = x)
			if (_map->getMapInfo((int)y, (int)x).type == MAPTILE_WALL ||
				_map->getMapInfo((int)y, (int)x).type == MAPTILE_WALL2)
			{
				count++;
				break;
			}
		}
		// Blocked ray: player not visible; clear ray: player spotted
		if (count >= 1) _isFindPlayer = false;
		else _isFindPlayer = true;
	}
}
