#include "stdafx.h"
#include "bat.h"
#include "Player.h"

bat::bat()
{
}


bat::~bat()
{
}


HRESULT bat::init(POINT point, float minCog, float maxCog)
{
	//_image = IMAGEMANAGER->findImage("wormMoveUp");
	//Initialize self-cognition range
	//Min, Max
	_minCog = minCog;
	_maxCog = maxCog;

	//Initialize spawn position
	_pointx = point.x;
	_pointy = point.y;


	_xspeed = _yspeed = _angle = _gravity = 0;
	//Drop money
	_money = RND->getFromIntTo(5, 1);

	//Initialize stats randomly
	_statistics.hp = 7;
	_statistics.str = 5;
	_statistics.dex = 2;
	_statistics.vit = 2;
	_statistics.inl = 2;
	_statistics.lck = 2;
	_statistics.def = 2;
	_statistics.fir = 0;
	_statistics.ice = 0;
	_statistics.lgt = 0;
	_statistics.psn = 0;
	_statistics.mel = 0;
	_statistics.rng = 0;
	_statistics.crit = 0;
	_statistics.aspd = 0;
	_statistics.spd = 1;

	//Check death
	_dead = false;
	_deadAlpha = 0;
	_rc = RectMakeCenter(_pointx, _pointy, 10, 10);
	_isFindPlayer = true;	//Player detection range
	_isOnTop = false;			//Whether on ceiling
	_image = IMAGEMANAGER->findImage("batflying");
	_currentFrameX = 0;
	_currentFrameY = 0;
	_timerForFrame = TIMEMANAGER->getWorldTime();
	_batstate = BATSTATE_FLYING;
	return S_OK;
}

void bat::update() {

	//If player is within detection range, activate; otherwise deactivate
	if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 200) _isFindPlayer = true;
	else _isFindPlayer = false;
	actByState();
	move();
	mapCollisionCheck();

	if(_statistics.hp>0) hitPlayer();
	deadcheck();
	imgHandleByState();
	if (TIMEMANAGER->getWorldTime() - _timerForFrame > 0.1)
	{
		_timerForFrame = TIMEMANAGER->getWorldTime();
		frameUpdate();
	}

	//if (_dead) _alpha += 1;
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}

void bat::move()
{
	_pointx += _xspeed;
	_pointy += _yspeed;

	if (_xspeed > 0) _xspeed -= 0.1;
	else if (_xspeed < 0) _xspeed += 0.1;
	if (_yspeed > 0) _yspeed -= 0.1;
	else if (_yspeed < 0) _yspeed += 0.1;

	if (abs(_xspeed) <= 0.5) _xspeed = 0;
	if (abs(_yspeed) <= 0.5) _yspeed = 0;

	//Speed limit
	if (_xspeed > 2) _xspeed = 2;
	else if (_xspeed < -2) _xspeed = -2;
	if (_yspeed > 2) _yspeed = 2;
	else if (_yspeed < -2) _yspeed = -2;
	
}

void bat::actByState()
{
	if (_state == ENEMYSTATE_DEAD)
	{
		_xspeed = 0;
		_yspeed = 0.2;
	}
	else
	{
		switch (_batstate)
		{
		case BATSTATE_FLYING:
			if (_isFindPlayer)
			{
				if(abs(_xspeed) < 1)
				_xspeed += cosf(getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y))*_statistics.spd;
				if(abs(_yspeed) < 1)
				_yspeed += -sinf(getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y))*_statistics.spd;
			}
			else
			{
				_yspeed += -0.8;
			}
			break;
		case BATSTATE_HIT:
			if (TIMEMANAGER->getWorldTime() - _hittimer > 0.7) _batstate = BATSTATE_FLYING;
			break;
		case BATSTATE_SLEEP:
			if (_isFindPlayer) _batstate = BATSTATE_FLYING;
			break;
		}
	}


}
void bat::hitPlayer()
{
	RECT temp;
	RECT playerRect = _player->getRect();
	if (IntersectRect(&temp, &playerRect, &_rc))
	{
		_player->getDamaged(5, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), _statistics.str);
		_xspeed = cosf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 10;
		_yspeed = -sinf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 10;
	}
}
void bat::mapCollisionCheck()
{
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	
	//mapInfo maptile;
	//RECT temp;
	//RECT smallrect;
	//
	//for (int i = -1; i < 2; i++)
	//{
	//	for (int j = -1; j < 2; j++)
	//	{
	//		maptile = _map->getMapInfo(int(_pointy) / TILESIZE - i, int(_pointx) / TILESIZE - j);
	//		if (maptile.type == MAPTILE_WALL || maptile.type == MAPTILE_WALL2)
	//		{
	//			if (IntersectRect(&temp, &maptile.rc, &smallrect))
	//			{
	//				_pointx += cosf(getAngle(maptile.point.x + TILESIZE / 2, maptile.point.y + TILESIZE / 2, _pointx, _pointy)) * 1;//(temp.right - temp.left + 2);
	//				_pointy -= sinf(getAngle(maptile.point.x + TILESIZE / 2, maptile.point.y + TILESIZE / 2, _pointx, _pointy)) * 1;//(temp.bottom - temp.top + 2);
	//				if (!_isFindPlayer && _batstate != BATSTATE_SLEEP)
	//				{
	//					_batstate = BATSTATE_SLEEP;
	//				}
	//			}
	//		}
	//	}
	//}
	RECT temp;
	mapInfo upL = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE - 1);
	mapInfo upM = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE);
	mapInfo upR = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE + 1);
	mapInfo midL = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE - 1);
	mapInfo midM = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE);
	mapInfo midR = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE + 1);
	mapInfo botL = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE - 1);
	mapInfo botM = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE);
	mapInfo botR = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE + 1);
	if ((midL.type == MAPTILE_WALL || midL.type == MAPTILE_WALL2) && IntersectRect(&temp, &midL.rc, &_rc))
	{
		_pointx += temp.right - temp.left;
	}
	if ((midR.type == MAPTILE_WALL || midR.type == MAPTILE_WALL2) && IntersectRect(&temp, &midR.rc, &_rc))
	{
		_pointx -= temp.right - temp.left;
	}
	if ((upM.type == MAPTILE_WALL || upM.type == MAPTILE_WALL2) && IntersectRect(&temp, &upM.rc, &_rc))
	{
		_pointy += temp.bottom - temp.top;
		if (!_isFindPlayer && _batstate != BATSTATE_SLEEP)
		{
			_batstate = BATSTATE_SLEEP;
		}
	}
	if ((botM.type == MAPTILE_WALL || botM.type == MAPTILE_WALL2) && IntersectRect(&temp, &botM.rc, &_rc))
	{
		_pointy -= temp.bottom - temp.top;
	}
	
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}

void bat::deadcheck()
{
	if (_statistics.hp <= 0)
	{
		_deadAlpha += 5;
		if (_deadAlpha >= 255)
		{
			_dead = true;
		}
	}
}
void bat::imgHandleByState()
{
	switch (_batstate)
	{
	case BATSTATE_FLYING:
		_image = IMAGEMANAGER->findImage("batflying");
		break;
	case BATSTATE_HIT:
		_image = IMAGEMANAGER->findImage("bathit");
		break;
	case BATSTATE_SLEEP:
		_image = IMAGEMANAGER->findImage("batsleep");
		break;
	}
}
void bat::render(POINT camera)
{
	draw(camera);
}

void bat::draw(POINT camera)
{

	//Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	_image->alphaFrameRender(getMemDC(),
		_pointx - _image->getFrameWidth() / 2 + camera.x,
		_pointy - _image->getFrameHeight() / 2 + camera.y,
		_currentFrameX, _currentFrameY, getSpriteAlpha());
}

void bat::frameUpdate()
{
	if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
	else _currentFrameX++;
}