#include "stdafx.h"
#include "FireBall.h"
#include "Map.h"
#include "Player.h"
#include "UI.h"

FireBall::FireBall() {}
FireBall::~FireBall() {}

HRESULT FireBall::init(int FireBallMax, float range, const char* imageName)
{
	_FireBallMax = FireBallMax;
	_range = range;
	_fireBallImage = IMAGEMANAGER->findImage(imageName);
	_timerForFrameUpdate = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void FireBall::release(void)
{

}

void FireBall::update(void)
{
	if (TIMEMANAGER->getWorldTime() - _timerForFrameUpdate > 0.1)
	{
		frameHandle();
		_timerForFrameUpdate = TIMEMANAGER->getWorldTime();
	}
	
	move();
	hitPlayer();
}

void FireBall::render(POINT camera)
{
	for (_viFireBall = _vFireBall.begin(); _viFireBall != _vFireBall.end(); ++_viFireBall)
	{
		_fireBallImage->frameRender(getMemDC(), _viFireBall->rc.left + camera.x, _viFireBall->rc.top + camera.y, _viFireBall->currentFrameX, _viFireBall->currentFrameY);
	}
}
void FireBall::frameHandle()
{
	for (_viFireBall = _vFireBall.begin(); _viFireBall != _vFireBall.end(); ++_viFireBall)
	{
		if (_viFireBall->currentFrameY < _fireBallImage->getMaxFrameY())
		{
			_viFireBall->currentFrameY++;
		}
		else _viFireBall->currentFrameY = 0;

		int frame;
		float angle;

		angle = _viFireBall->angle + PI16;
		if (angle >= PI2) angle -= PI2;

		frame = int(angle / PI8);
		_viFireBall->currentFrameX = frame;
	}




}

void FireBall::fire(float x, float y, float angle, float speed)
{

	if (_FireBallMax < _vFireBall.size()) return;

	tagFireBall FireBall;
	ZeroMemory(&FireBall, sizeof(tagFireBall));
	FireBall.speed = speed;
	FireBall.radius = _fireBallImage->getFrameWidth() / 2;
	FireBall.x = FireBall.fireX = x;
	FireBall.y = FireBall.fireY = y;
	FireBall.angle = angle;
	FireBall.currentFrameX = FireBall.currentFrameY = 0;
	FireBall.power = 8;
	FireBall.rc = RectMakeCenter(FireBall.x, FireBall.y,
		_fireBallImage->getFrameWidth(),
		_fireBallImage->getFrameHeight());

	_vFireBall.push_back(FireBall);

}


void FireBall::move(void)
{
	for (int i = _vFireBall.size()-1; i >=0; i--)
	{
		_vFireBall[i].x += cosf(_vFireBall[i].angle) * _vFireBall[i].speed;
		_vFireBall[i].y += -sinf(_vFireBall[i].angle) * _vFireBall[i].speed;

		_vFireBall[i].rc = RectMakeCenter(_vFireBall[i].x, _vFireBall[i].y,
			_fireBallImage->getFrameWidth() - 5,
			_fireBallImage->getFrameHeight() - 5);

		if (!(0<_vFireBall[i].y / TILESIZE && _vFireBall[i].y / TILESIZE <40) ||
			!(0<_vFireBall[i].x / TILESIZE && _vFireBall[i].x / TILESIZE <58))
		{
			removeFireBall(i);
		}
		else if (_range < getDistance(_vFireBall[i].x, _vFireBall[i].y, _vFireBall[i].fireX, _vFireBall[i].fireY) ||
			_map->getMapInfo(_vFireBall[i].y / TILESIZE, _vFireBall[i].x / TILESIZE).type == MAPTILE_WALL ||
			_map->getMapInfo(_vFireBall[i].y / TILESIZE, _vFireBall[i].x / TILESIZE).type == MAPTILE_WALL2)
		{
			removeFireBall(i);
		}
	}
}

void FireBall::removeFireBall(int arrNum)
{
	_vFireBall.erase(_vFireBall.begin() + arrNum);
}

void FireBall::hitPlayer()
{
	RECT temp;
	for (int i = _vFireBall.size() - 1; i >= 0; i--)
	{
		RECT playerRect = _player->getRect(); if (IntersectRect(&temp, &_vFireBall[i].rc, &playerRect))
		{
			_player->getDamaged(_vFireBall[i].power);
			tagStatusEffect temp;
			temp.type = STATUSEFFECT_FIRE;
			temp.leftTime = 5;
			temp.damage = 1;
			_player->addStatusEffect(temp);
			removeFireBall(i);
		}
	}
}