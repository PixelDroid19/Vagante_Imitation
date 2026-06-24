#include "stdafx.h"
#include "Boss.h"
#include "Player.h"
#include "UI.h"
#include "FireBall.h"

Boss::Boss()
{
}


Boss::~Boss()
{
}


HRESULT Boss::init()
{
	return S_OK;
}
HRESULT Boss::init(POINT point)
{
	//Initialize variables
	_image = IMAGEMANAGER->findImage("boss_dragon_sleep");
	_state = BOSSSTATE_SLEEP;													//State
	for (int i = 0; i < STATUSEFFECTMAX; i++)
	{
		_statusEffect[i].type = STATUSEFFECT_NULL;  //Status effect
		_statusEffect[i].leftTime = 0;
		_statusEffect[i].damage = 0;
	}
	_currentFrameX = _currentFrameY = 0;									//Frame
	_pointx = point.x;
	_pointy = point.y;										//Coordinates
	_xspeed = _yspeed = 0;												//x,y movement speed
	_money = 10;															//Gold dropped when killed
	//_isFindPlayer = false;													//Whether player was found
	_fireball = new FireBall;
	_fireball->setUIAddressLink(_ui);
	_fireball->setMapAddressLink(_map);
	_fireball->setPlayerAddressLink(_player);
	_fireball->init(100, 1000, "fireball");
	_actTimer = TIMEMANAGER->getWorldTime();
	//_frameTime, _frameFPS;											//Frame change
	_minCog = 150;
	_maxCog = 1500;
	_statistics.hp = 100;
	_statistics.str = 5;
	_statistics.dex = 0;
	_statistics.vit = 0;
	_statistics.inl = 0;
	_statistics.lck = 0;
	_statistics.def = 0;
	_statistics.fir = 0;
	_statistics.ice = 0;
	_statistics.lgt = 0;
	_statistics.psn = 0;
	_statistics.mel = 0;
	_statistics.rng = 0;
	_statistics.crit = 0;
	_statistics.aspd = 1;
	_statistics.spd = 1;
	_canfire = true;
	_timerForFrameUpdate = TIMEMANAGER->getWorldTime();
	_timerForAstar = TIMEMANAGER->getWorldTime();
	_stampHitLand = false;
	_totallydead = false;
	upL = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE - 1);
	upM = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE);
	upR = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE + 1);
	midL = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE - 1);
	midM = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE);
	midR = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE + 1);
	botL = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE - 1);
	botM = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE);
	botR = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE + 1);
	_lookleft = true;
	_rc = RectMakeCenter(_pointx, _pointy, 30, 30);
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 58; j++)
		{
			_tileinfo[i][j] = _map->getMapInfo(i, j).type;
		}
	}

	_curTileX = int(_pointx) / TILESIZE;
	_curTileY = int(_pointy) / TILESIZE;
	_goalTileX = int(_player->getPoint().x) / TILESIZE;
	_goalTileY = int(_player->getPoint().y) / TILESIZE;
	astar();
	

	return S_OK;
}
void Boss::release()
{

}
void Boss::update()
{
	if (_totallydead) return;

	stateHandle();
	speedAdjust();
	_rc = RectMakeCenter(_pointx, _pointy, 30, 30);
	_rcHit = RectMakeCenter(_pointx, _pointy, 50, 50);

	move();
	if (TIMEMANAGER->getWorldTime() - _timerForFrameUpdate > 0.2)
	{
		frameUpdate();
		_timerForFrameUpdate = TIMEMANAGER->getWorldTime();
	}
	if (TIMEMANAGER->getWorldTime() - _timerForAstar > 0.2)
	{
		_timerForAstar = TIMEMANAGER->getWorldTime();
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > 150)
		{
			_curTileX = int(_pointx) / TILESIZE;
			_curTileY = int(_pointy) / TILESIZE;
			_goalTileX = int(_player->getPoint().x) / TILESIZE;
			_goalTileY = int(_player->getPoint().y) / TILESIZE;
			_openlist.clear();
			_closelist.clear();
			_wayToPlayer.clear();
			if(_state != BOSSSTATE_SLEEP) astar();
		}

	}
	_fireball->update();
	deadCheck();

}
void Boss::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);

}

//Draw: x coordinate by camera.x
//			y coordinate by camera.y offset!!!!
void Boss::render(POINT camera)
{
	draw(camera);
}
void Boss::draw(POINT camera)
{

	//if((_pointx > camera.x && _pointx < camera.x + WINSIZEX) &&
	//	(_pointy > camera.y && _pointy < camera.y + WINSIZEY))
	//	_image->frameRender(getMemDC(), _rc.left - camera.x, _rc.top - camera.y);
	//Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	//Rectangle(getMemDC(), _rcHit.left + camera.x, _rcHit.top + camera.y, _rcHit.right + camera.x, _rcHit.bottom + camera.y);
	_fireball->render(camera);
	_image->frameRender(getMemDC(), _pointx - _image->getFrameWidth() / 2 + camera.x, _pointy - _image->getFrameHeight() / 2 + camera.y, _currentFrameX, _currentFrameY);

}

void Boss::drawDebug(POINT camera)
{
	if (!isDebugDrawEnabled())
		return;

	SDL_Renderer* dc = getMemDC();

	if (isDebugLayerEnabled(DEBUG_LAYER_PATH))
	{
		for (size_t i = 0; i < _wayToPlayer.size(); i++)
		{
			int x = _wayToPlayer[i].vx * TILESIZE + TILESIZE / 2 + camera.x;
			int y = _wayToPlayer[i].vy * TILESIZE + TILESIZE / 2 + camera.y;
			debugDrawPoint(dc, x, y, 3, DebugPalette::Path);
		}

		for (size_t i = 1; i < _wayToPlayer.size(); i++)
		{
			int x0 = _wayToPlayer[i - 1].vx * TILESIZE + TILESIZE / 2 + camera.x;
			int y0 = _wayToPlayer[i - 1].vy * TILESIZE + TILESIZE / 2 + camera.y;
			int x1 = _wayToPlayer[i].vx * TILESIZE + TILESIZE / 2 + camera.x;
			int y1 = _wayToPlayer[i].vy * TILESIZE + TILESIZE / 2 + camera.y;
			debugDrawLine(dc, x0, y0, x1, y1, DebugPalette::Path);
		}
	}

	if (!isDebugLayerEnabled(DEBUG_LAYER_HITBOX))
		return;

	debugDrawRect(dc, _rc, camera, DebugPalette::Enemy);
	debugDrawRect(dc, _rcHit, camera, DebugPalette::Attack);
	debugDrawCrosshair(dc, (int)_pointx + camera.x, (int)_pointy + camera.y, 5, DebugPalette::EnemyCore);
}
void Boss::speedAdjust()
{

}
void Boss::move()
{
	if (_state == BOSSSTATE_SLEEP || _state == BOSSSTATE_DEAD)
	{
		_yspeed = -3;
	}
	_pointx += _xspeed;
	_pointy -= _yspeed;
	_rc = RectMakeCenter(_pointx, _pointy, 30, 30);
	mapCollisionHandle();
	_xspeed = 0;
	_yspeed = 0;
}
void Boss::mapCollisionHandle()
{

	upL = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE - 1);
	upM = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE);
	upR = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE + 1);
	midL = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE - 1);
	midM = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE);
	midR = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE + 1);
	botL = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE - 1);
	botM = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE);
	botR = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE + 1);

	if (_state != BOSSSTATE_SLEEP && _state != BOSSSTATE_DEAD)
	{
		mapInfo maptile;
		RECT temp;
		RECT smallrect;
		smallrect = RectMakeCenter(_pointx, _pointy, 15, 15);
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				maptile = _map->getMapInfo(int(_pointy) / TILESIZE - i, int(_pointx) / TILESIZE - j);
				if (maptile.type == MAPTILE_WALL || maptile.type == MAPTILE_WALL2)
				{
					if (IntersectRect(&temp, &maptile.rc, &smallrect))
					{
						_pointx += cosf(getAngle(maptile.point.x + TILESIZE / 2, maptile.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.right - temp.left + 1);
						_pointy -= sinf(getAngle(maptile.point.x + TILESIZE / 2, maptile.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.bottom - temp.top + 1);
						//Stamp check!
						if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
					}
				}
			}
		}
	}
	else
	{
		if ((upL.type == MAPTILE_WALL || upL.type == MAPTILE_WALL2) && isCollisionReaction(upL.rc, _rc))
		{
			_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
			_pointx = _rc.left + (_rc.right - _rc.left) / 2;
			//Stamp check!
			if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
		}
		else if ((upM.type == MAPTILE_WALL || upM.type == MAPTILE_WALL2) && isCollisionReaction(upM.rc, _rc))
		{
			_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
			_pointx = _rc.left + (_rc.right - _rc.left) / 2;
			//Stamp check!
			if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
		}
		else if ((upR.type == MAPTILE_WALL || upR.type == MAPTILE_WALL2) && isCollisionReaction(upR.rc, _rc))
		{
			_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
			_pointx = _rc.left + (_rc.right - _rc.left) / 2;
			//Stamp check!
			if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
		}
		else if ((midL.type == MAPTILE_WALL || midL.type == MAPTILE_WALL2) && isCollisionReaction(midL.rc, _rc))
		{
			_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
			_pointx = _rc.left + (_rc.right - _rc.left) / 2;
			//Stamp check!
			if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
		}
		else if ((midM.type == MAPTILE_WALL || midM.type == MAPTILE_WALL2) && isCollisionReaction(midM.rc, _rc))
		{
			_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
			_pointx = _rc.left + (_rc.right - _rc.left) / 2;
			//Stamp check!
			if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
		}
		else if ((midR.type == MAPTILE_WALL || midR.type == MAPTILE_WALL2) && isCollisionReaction(midR.rc, _rc))
		{
			_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
			_pointx = _rc.left + (_rc.right - _rc.left) / 2;
			//Stamp check!
			if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
		}
		else if ((botL.type == MAPTILE_WALL || botL.type == MAPTILE_WALL2) && isCollisionReaction(botL.rc, _rc))
		{
			_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
			_pointx = _rc.left + (_rc.right - _rc.left) / 2;
			//Stamp check!
			if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
		}
		else if ((botM.type == MAPTILE_WALL || botM.type == MAPTILE_WALL2) && isCollisionReaction(botM.rc, _rc))
		{
			_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
			_pointx = _rc.left + (_rc.right - _rc.left) / 2;
			//Stamp check!
			if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
		}
		else if ((botR.type == MAPTILE_WALL || botR.type == MAPTILE_WALL2) && isCollisionReaction(botR.rc, _rc))
		{
			_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
			_pointx = _rc.left + (_rc.right - _rc.left) / 2;
			//Stamp check!
			if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
		}
	}

	/*
	RECT temp;
	if ((upL.type == MAPTILE_WALL || upL.type == MAPTILE_WALL2) && IntersectRect(&temp,&upL.rc,&_rc))
	{
		_pointx += cosf(getAngle(upL.point.x + TILESIZE / 2, upL.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.right - temp.left);
		_pointy -= sinf(getAngle(upL.point.x + TILESIZE / 2, upL.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.bottom - temp.top);

	}
	else if ((upM.type == MAPTILE_WALL || upM.type == MAPTILE_WALL2) && IntersectRect(&temp, &upM.rc, &_rc))
	{
		_pointx += cosf(getAngle(upM.point.x + TILESIZE / 2, upM.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.right - temp.left);
		_pointy -= sinf(getAngle(upM.point.x + TILESIZE / 2, upM.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.bottom - temp.top);
	}
	else if ((upR.type == MAPTILE_WALL || upR.type == MAPTILE_WALL2) && IntersectRect(&temp, &upR.rc, &_rc))
	{
		_pointx += cosf(getAngle(upR.point.x + TILESIZE / 2, upR.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.right - temp.left);
		_pointy -= sinf(getAngle(upR.point.x + TILESIZE / 2, upR.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.bottom - temp.top);
	}
	else if ((midL.type == MAPTILE_WALL || midL.type == MAPTILE_WALL2) && IntersectRect(&temp, &midL.rc, &_rc))
	{
		_pointx += cosf(getAngle(midL.point.x + TILESIZE / 2, midL.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.right - temp.left);
		_pointy -= sinf(getAngle(midL.point.x + TILESIZE / 2, midL.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.bottom - temp.top);
	}
	else if ((midM.type == MAPTILE_WALL || midM.type == MAPTILE_WALL2) && IntersectRect(&temp, &midM.rc, &_rc))
	{
		_pointx += cosf(getAngle(midM.point.x + TILESIZE / 2, midM.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.right - temp.left);
		_pointy -= sinf(getAngle(midM.point.x + TILESIZE / 2, midM.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.bottom - temp.top);
	}
	else if ((midR.type == MAPTILE_WALL || midR.type == MAPTILE_WALL2) && IntersectRect(&temp, &midR.rc, &_rc))
	{
		_pointx += cosf(getAngle(midR.point.x + TILESIZE / 2, midR.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.right - temp.left);
		_pointy -= sinf(getAngle(midR.point.x + TILESIZE / 2, midR.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.bottom - temp.top);
	}
	else if ((botL.type == MAPTILE_WALL || botL.type == MAPTILE_WALL2) && IntersectRect(&temp, &botL.rc, &_rc))
	{
		_pointx += cosf(getAngle(botL.point.x + TILESIZE / 2, botL.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.right - temp.left);
		_pointy -= sinf(getAngle(botL.point.x + TILESIZE / 2, botL.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.bottom - temp.top);
	}
	else if ((botM.type == MAPTILE_WALL || botM.type == MAPTILE_WALL2) && IntersectRect(&temp, &botM.rc, &_rc))
	{
		_pointx += cosf(getAngle(botM.point.x + TILESIZE / 2, botM.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.right - temp.left);
		_pointy -= sinf(getAngle(botM.point.x + TILESIZE / 2, botM.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.bottom - temp.top);
	}
	else if ((botR.type == MAPTILE_WALL || botR.type == MAPTILE_WALL2) && IntersectRect(&temp, &botR.rc, &_rc))
	{
		_pointx += cosf(getAngle(botR.point.x + TILESIZE / 2, botR.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.right - temp.left);
		_pointy -= sinf(getAngle(botR.point.x + TILESIZE / 2, botR.point.y + TILESIZE / 2, _pointx, _pointy))*(temp.bottom - temp.top);
	}
	*/
	/*
	if ((upL.type == MAPTILE_WALL || upL.type == MAPTILE_WALL2) && isCollisionReaction(upL.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
		//Stamp check!
		if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
	}
	else if ((upM.type == MAPTILE_WALL || upM.type == MAPTILE_WALL2) && isCollisionReaction(upM.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
		//Stamp check!
		if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
	}
	else if ((upR.type == MAPTILE_WALL || upR.type == MAPTILE_WALL2) && isCollisionReaction(upR.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
		//Stamp check!
		if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
	}
	else if ((midL.type == MAPTILE_WALL || midL.type == MAPTILE_WALL2) && isCollisionReaction(midL.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
		//Stamp check!
		if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
	}
	else if ((midM.type == MAPTILE_WALL || midM.type == MAPTILE_WALL2) && isCollisionReaction(midM.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
		//Stamp check!
		if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
	}
	else if ((midR.type == MAPTILE_WALL || midR.type == MAPTILE_WALL2) && isCollisionReaction(midR.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
		//Stamp check!
		if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
	}
	else if ((botL.type == MAPTILE_WALL || botL.type == MAPTILE_WALL2) && isCollisionReaction(botL.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
		//Stamp check!
		if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
	}
	else if ((botM.type == MAPTILE_WALL || botM.type == MAPTILE_WALL2) && isCollisionReaction(botM.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
		//Stamp check!
		if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
	}
	else if ((botR.type == MAPTILE_WALL || botR.type == MAPTILE_WALL2) && isCollisionReaction(botR.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
		//Stamp check!
		if (_state == BOSSSTATE_STAMPING && _currentFrameX == _image->getMaxFrameX()) _stampHitLand = true;
	}
	*/
	_rc = RectMakeCenter(_pointx, _pointy, 30, 30);
}
void Boss::addStatusEffect(tagStatusEffect statuseffect)
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

//Status effect processing
void Boss::statusEffect()
{
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

void Boss::frameUpdate()
{
	if (_state != BOSSSTATE_SLEEP && _state != BOSSSTATE_DEAD && _state != BOSSSTATE_STAMPING)
	{
		if (_player->getPoint().x > _pointx)
		{
			_currentFrameY = 0;
		}
		else
		{
			_currentFrameY = 1;
		}
	}

	switch (_state)
	{
	case BOSSSTATE_SLEEP:

		break;
	case BOSSSTATE_ACTIVATE:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	case BOSSSTATE_FLYING:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	case BOSSSTATE_FIREING:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	case BOSSSTATE_STAMPING:
		if (_currentFrameX >= _image->getMaxFrameX()) {}
		else _currentFrameX++;
		break;
	case BOSSSTATE_DEAD:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	}
}
void Boss::imageChange()
{
	switch (_state)
	{
	case BOSSSTATE_SLEEP:
		_image = IMAGEMANAGER->findImage("boss_dragon_sleep");
		break;
	case BOSSSTATE_ACTIVATE:
		_image = IMAGEMANAGER->findImage("boss_dragon_cry");
		break;
	case BOSSSTATE_FLYING:
		_image = IMAGEMANAGER->findImage("boss_dragon_fly");
		break;
	case BOSSSTATE_FIREING:
		_image = IMAGEMANAGER->findImage("boss_dragon_fire");
		break;
	case BOSSSTATE_STAMPING:
		_image = IMAGEMANAGER->findImage("boss_dragon_stamp");
		break;
	case BOSSSTATE_DEAD:
		_image = IMAGEMANAGER->findImage("boss_dragon_dead");
		break;
	}
	_currentFrameX = 0;
}
void Boss::stateHandle()
{
	switch (_state)
	{
	case BOSSSTATE_SLEEP:
		//Activate boss
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < _minCog)
		{
			//State transition
			_state = BOSSSTATE_ACTIVATE;
			//Change image according to state change
			imageChange();
			//Timer reset
			_actTimer = TIMEMANAGER->getWorldTime();

			SOUNDMANAGER->stop("7_DarkCave_Music");
			SOUNDMANAGER->play("0_boss_Backgound_Music", 0.5);
		}
		break;
	case BOSSSTATE_ACTIVATE:
		//When roar ends (if within range, transition to BOSSSTATE_FLYING)
		if (TIMEMANAGER->getWorldTime() - _actTimer > 2)
		{
			astar();
			_state = BOSSSTATE_FLYING;
			//Change image according to state change
			imageChange();
			_actTimer = TIMEMANAGER->getWorldTime();
		}
		else
		{
			_yspeed = _statistics.spd;
		}
		break;
	case BOSSSTATE_FLYING:
		//Move towards player and if within certain distance, transition to BOSSSTATE_FIREING
		RECT temp;
		if (TIMEMANAGER->getWorldTime() - _actTimer > 4 && getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 300)
		{
			if ((_player->getPoint().x - 50 < _pointx && _pointx < _player->getPoint().x + 50) && _player->getPoint().y > _pointy)
			{
				_state = BOSSSTATE_STAMPING;
				_stampHitLand = false;
			}
			else
			{
				_state = BOSSSTATE_FIREING;
				if (_statistics.hp <= 50) _fireballCount = 7;
				else _fireballCount = 5;
				_canfire = true;
			}
			//Change image according to state change
			imageChange();
			_actTimer = TIMEMANAGER->getWorldTime();
		}
		else
		{
			RECT playerRect = _player->getRect();
			if (IntersectRect(&temp, &_rc, &playerRect)) {
			_state = BOSSSTATE_FIREING;
			if (_statistics.hp <= 50) _fireballCount = 7;
			else _fireballCount = 5;
			imageChange();
			_actTimer = TIMEMANAGER->getWorldTime();
		}
		else
		{
			if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > 150)
			{
				if (!(_wayToPlayer[_wayToPlayer.size() - 1].vx*TILESIZE + TILESIZE / 2 - 1 < _pointx && _pointx < _wayToPlayer[_wayToPlayer.size() - 1].vx*TILESIZE + TILESIZE / 2 + 1)) {
					_xspeed = cosf(getAngle(_pointx, _pointy, _wayToPlayer[_wayToPlayer.size() - 1].vx*TILESIZE + TILESIZE / 2, _wayToPlayer[_wayToPlayer.size() - 1].vy*TILESIZE + TILESIZE / 2)) * _statistics.spd;
				}
				if (!(_wayToPlayer[_wayToPlayer.size() - 1].vy*TILESIZE + TILESIZE / 2 - 1 < _pointy && _pointy < _wayToPlayer[_wayToPlayer.size() - 1].vy*TILESIZE + TILESIZE / 2 + 1)) {
					_yspeed = sinf(getAngle(_pointx, _pointy, _wayToPlayer[_wayToPlayer.size() - 1].vx*TILESIZE + TILESIZE / 2, _wayToPlayer[_wayToPlayer.size() - 1].vy*TILESIZE + TILESIZE / 2)) * _statistics.spd;
				}
			}
			else
			{
				_xspeed = cosf(getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y)) * _statistics.spd;
				_yspeed = sinf(getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y)) * _statistics.spd;

			}
		}
		}
		break;
	case BOSSSTATE_FIREING:
		if (_fireballCount == 0 && _currentFrameX == _image->getMaxFrameX())
		{
			_state = BOSSSTATE_FLYING;
			//Change image according to state change
			imageChange();
			_actTimer = TIMEMANAGER->getWorldTime();
		}
		else
		{
			fireFireBall();
		}
		break;
	case BOSSSTATE_STAMPING:
		stamping();
		break;
	case BOSSSTATE_DEAD:
		if (TIMEMANAGER->getWorldTime() - _actTimer > 20)
		{
			_totallydead = true;
		}
		break;
	}
}

void Boss::fireFireBall()
{
	if (_canfire && _currentFrameX >= 4 && _fireballCount > 0)
	{
		_fireball->fire(_pointx, _pointy, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 4);
		_canfire = false;
		_fireballCount--;
		SOUNDMANAGER->play("8_BossFireball",0.5);
	}
	if (_currentFrameX == 0) _canfire = true;
}
void Boss::stamping()
{
	if (_currentFrameX == _image->getMaxFrameX())
	{
		_yspeed = -10;
	}
	else if (_currentFrameX < 2)
	{
		_yspeed = 3;
	}
	if (_stampHitLand)
	{
		if (_player->getState() != PLAYERSTATE_JUMPING && _player->getState() != PLAYERSTATE_FALLING)
		{
			_player->getDamaged(10, PI / 2, -10);
		}
		_stampHitLand = false;
		SOUNDMANAGER->play("9_BossStamp", 0.5);
		_state = BOSSSTATE_FLYING;
		imageChange();
		_actTimer = TIMEMANAGER->getWorldTime();
	}
}
void Boss::deadCheck()
{
	if (_statistics.hp <= 0 && _state != BOSSSTATE_DEAD)
	{
		_yspeed -= 5;
		_state = BOSSSTATE_DEAD;
		imageChange();
		SOUNDMANAGER->play("7_DarkCave_Music", 0.5);
		SOUNDMANAGER->stop("0_boss_Backgound_Music");
		_actTimer = TIMEMANAGER->getWorldTime();
	}
	else if (_statistics.hp <= 0 && _state == BOSSSTATE_DEAD)
	{
		_yspeed -= 5;
	}
}

void Boss::add_openlist(vertex v)
{
	//Duplicate check
	for (int i = 0; i < _openlist.size(); i++)
	{
		if (_openlist[i].vx == v.vx && _openlist[i].vy == v.vy)
		{
			return;
		}
	}
	for (int i = 0; i < _closelist.size(); i++)
	{
		if (_closelist[i].vx == v.vx && _closelist[i].vy == v.vy)
		{
			return;
		}
	}
	//Add
	_openlist.push_back(v);
	//Sort
	sort(_openlist.begin(), _openlist.end());
}
void Boss::add_closelist(vertex v)
{
	//Duplicate check
	bool check = true;;
	for (int i = 0; i < _closelist.size(); i++)
	{
		if (_closelist[i].vx == v.vx && _closelist[i].vy == v.vy)
		{
			check = false;
		}
	}
	//Add
	_closelist.push_back(v);
}
vertex Boss::pop_openlist()
{
	vertex temp;
	temp = _openlist[_openlist.size() - 1];
	_openlist.pop_back();
	return temp;
}
vertex Boss::pop_closelist()
{
	vertex temp;
	temp = _closelist[_closelist.size() - 1];
	_closelist.pop_back();
	return temp;
}
vertex Boss::pop_closelist(int vx, int vy)
{
	for (int i = 0; i < _closelist.size(); i++)
	{
		if (_closelist[i].vx == vx, _closelist[i].vy == vy)
		{
			return _closelist[i];
		}
	}
	vertex temp;
	return temp;
}
vertex Boss::calc_vertex(vertex v, vertex p)
{
	//if (v.p == NULL)
	if (v.px == _startpoint.vx || v.py == _startpoint.vy)
	{
		//Calculate movement cost
		if (v.vx != _startpoint.vx && v.vy != _startpoint.vy)
		{
			v.h = _startpoint.h + 14;
		}
		else if (v.vx == _startpoint.vx && v.vy != _startpoint.vy)
		{
			v.h = _startpoint.h + 10;
		}
		else if (v.vx != _startpoint.vx && v.vy == _startpoint.vy)
		{
			v.h = _startpoint.h + 10;
		}
		// Sort movement distance calculation
		v.g = 0;
		if (v.vx != _goalTileX)
		{
			v.g += 10 * (abs(_goalTileX - v.vx));
		}
		if (v.vy != _goalTileY)
		{
			v.g += 10 * (abs(_goalTileY - v.vy));
		}
		//Calculate total
		v.f = v.g + v.h;
		return v;
	}
	else
	{
		//Calculate movement cost
		if (v.vx != v.px && v.vy != v.py)
		{
			v.h = p.h + 14;
		}
		else if (v.vx == v.px && v.vy != v.py)
		{
			v.h = p.h + 10;
		}
		else if (v.vx != v.px && v.vy == v.py)
		{
			v.h = p.h + 10;
		}
		//if (v.vx != v.p->vx && v.vy != v.p->vy)
		//{
		//	v.h = v.p->h + 14;
		//}
		//else if (v.vx == v.p->vx && v.vy != v.p->vy)
		//{
		//	v.h = v.p->h + 10;
		//}
		//else if (v.vx != v.p->vx && v.vy == v.p->vy)
		//{
		//	v.h = v.p->h + 10;
		//}
		//Calculate heuristic cost
		v.g = 0;
		if (v.vx != _goalTileX)
		{
			v.g += 10 * (abs(_goalTileX - v.vx));
		}
		if (v.vy != _goalTileY)
		{
			v.g += 10 * (abs(_goalTileY - v.vy));
		}
		//Calculate total
		v.f = v.g + v.h;
		return v;
	}

}

void Boss::astar()
{
	_startpoint.vx = _curTileX;
	_startpoint.vy = _curTileY;
	_startpoint.h = 0;
	_startpoint.g = 0;
	if (_startpoint.vx != _goalTileX)
	{
		_startpoint.g += 10 * (abs(_goalTileX - _startpoint.vx));
	}
	if (_startpoint.vy != _goalTileY)
	{
		_startpoint.g += 10 * (abs(_goalTileY - _startpoint.vy));
	}
	_startpoint.f = _startpoint.g + _startpoint.h;
	//_startpoint.p = NULL;
	_startpoint.px = _curTileX;
	_startpoint.py = _curTileY;
	_currentvertex = &_startpoint;
	add_opelistEightWay(_currentvertex);

	while (true)
	{

		if (checkGoal())
		{
			break;
		}
		else
		{
			_currentvertex = NULL;
			_npcurrentvertex = pop_openlist();
			//Duplicate check
			bool check = true;;
			for (int i = 0; i < _closelist.size(); i++)
			{
				if (_closelist[i].vx == _npcurrentvertex.vx && _closelist[i].vy == _npcurrentvertex.vy)
				{
					check = false;
				}
			}
			if (check) _closelist.push_back(_npcurrentvertex);
			add_opelistEightWay(&_npcurrentvertex);
			int a = 5;
		}
	}
	makeWay();
}

void Boss::add_opelistEightWay(vertex* v)
{
	vertex temp;
	//temp.p = v;
	temp.px = (*v).vx;
	temp.py = (*v).vy;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (i == 0 && j == 0) continue;

			temp.vx = i + (*v).vx;;
			temp.vy = j + (*v).vy;;
			if (0 <= temp.vx && temp.vx <= 58 && 0 <= temp.vy&&temp.vy <= 40)
			{
				if ((_map->getMapInfo(temp.vy, temp.vx).type != MAPTILE_WALL && _map->getMapInfo(temp.vy, temp.vx).type != MAPTILE_WALL2))
				{
					temp = calc_vertex(temp, *v);
					bool check = true;
					//Duplicate check
					for (int i = 0; i < _openlist.size(); i++)
					{
						if (_openlist[i].vx == temp.vx && _openlist[i].vy == temp.vy)
						{
							check = false;
						}
					}
					for (int i = 0; i < _closelist.size(); i++)
					{
						if (_closelist[i].vx == temp.vx && _closelist[i].vy == temp.vy)
						{
							check = false;
						}
					}
					//Add
					if (check)_openlist.push_back(temp);
				}
			}
		}
	}
	//Sort
	sort(_openlist.begin(), _openlist.end());

}

bool Boss::checkGoal()
{
	for (int i = 0; i < _openlist.size(); i++)
	{
		if (_openlist[i].vx == _goalTileX && _openlist[i].vy == _goalTileY)
		{
			return true;
		}
	}
	return false;
}

void Boss::makeWay()
{
	vertex temp;
	temp = pop_closelist();
	_wayToPlayer.push_back(temp);

	while (true)
	{
		if (_wayToPlayer[_wayToPlayer.size() - 1].px == _startpoint.vx && _wayToPlayer[_wayToPlayer.size() - 1].py == _startpoint.vy)
		{
			break;
		}
		else
		{
			temp = getcloselist(_wayToPlayer[_wayToPlayer.size() - 1].px, _wayToPlayer[_wayToPlayer.size() - 1].py);
			_wayToPlayer.push_back(temp);
		}
	}
}
vertex Boss::getcloselist(int x, int y)
{
	vertex temp;
	for (int i = 0; i < _closelist.size(); i++)
	{
		if (_closelist[i].vx == x && _closelist[i].vy == y)
		{
			return _closelist[i];
		}
	}
	return temp;
}
vertex Boss::getopenlist(int x, int y)
{
	vertex temp;
	for (int i = 0; i < _openlist.size(); i++)
	{
		if (_openlist[i].vx == x && _openlist[i].vy == y)
		{
			return _openlist[i];
		}
	}
	return temp;
}