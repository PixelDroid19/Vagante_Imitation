#include "stdafx.h"
#include "manEater.h"
#include "Player.h"

manEater::manEater()
{
}
manEater::~manEater()
{
}
				  // Constructor receives minCog and maxCog
HRESULT manEater::init(POINT point, float minCog, float maxCog)
{
	_state = ENEMYSTATE_IDLE;
	_pointx = point.x;
	_pointy = point.y;

	_money = RND->getFromIntTo(5, 1);
	_isFindPlayer = false;
	_attack = false;
	_count = 0;

	_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);

	_findRange = minCog = maxCog;

	_findRectRange = RectMakeCenter(_pointx + 16, _pointy + 16, _findRange, _findRange);

	_attacki = new image;
	_attackAfteri = new image;
	_deadi = new image;
	
	_deadi->init("Img/enemy/manEater_dead.bmp", 32, 32, 1, 1, true, RGB(255, 0, 255));
	_attacki->init("Img/enemy/manEater_attack.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	_attackAfteri->init("Img/enemy/manEater_after_attack.bmp", 224, 32, 7, 1, true, RGB(255, 0, 255));

	_statistics.hp = 3;
	_statistics.str = 1;
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
	_statistics.aspd = 0;
	_statistics.spd = 0;

	//_image = IMAGEMANAGER->findImage("maneater_Attack_After");
	_image = _attacki;

	_currentFrameX = 0;
	_currentFrameY = 0;

	_frameFPS = 10;
	_frameTime = 0;
	
	_dead = false;

	_deadAlpha = 0;

	return S_OK;
}

void manEater::release()
{

}

void manEater::update()
{

	//if (KEYMANAGER->isOnceKeyDown('M')) // For checking button press
	//{
	//	getDamaged(1);
	//}


	if (PtInRect(&_findRectRange,_player->getPoint())) // Detection
	{
		_isFindPlayer = true;
	}

	if (_isFindPlayer == true)
	{
		attack();// Attack function
		frameUpdate();// Function to execute frame update
	}

	if (_statistics.hp <= 0)// Death check
	{
		_image = _deadi;
	}

	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		if (_deadAlpha > 255)
		{
			_dead = true;
		}
	}
}


void manEater::render(POINT camera)
{
	draw(camera);
}

void manEater::draw(POINT camera)
{
	//Rectangle(getMemDC(), _findRectRange.left + camera.x, _findRectRange.top + camera.y, _findRectRange.right + camera.x, _findRectRange.bottom + camera.y); // For confirming player position detection
	//_image->frameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _currentFrameX, 0);
	if (!_image)
		return;

	_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y,
		_currentFrameX, 0, getSpriteAlpha());

	//_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _currentFrameX, 0);
	//Rectangle(getMemDC(), 50,50,100,100);
	//
	//char *str;
	//
	//sprintf(str , "elapsedTime : %f", _currentFrameX);
	//TextOut(getMemDC(), _rc.left, _rc.top, str, strlen(str));
}

void manEater::attack()
{
	// Start changing to attack image!
	if (_currentFrameX > _image->getMaxFrameX() && _state == ENEMYSTATE_IDLE) // Rising to attack
	{
		_rc = RectMake(_pointx, _pointy, 0, 0);// Idle collision rect size // Non-attacking idle collision rect size
		_attackRect = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);	 // Attack collision rect size
		_statistics.mel = RND->getFromIntTo(4, 15);			 // Attack power
		_currentFrameX = 0;									 // Reset frame
		_image = _attackAfteri;								 // Reset image
		_state = ENEMYSTATE_ATTACKING;						 // State change 
	}
	
	if (_currentFrameX >= _image->getMaxFrameX() && _state == ENEMYSTATE_ATTACKING) // After attacking, return 
	{
		_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);// Attack collision rect size
		_attackRect = RectMake(_pointx, _pointy, 0, 0);	 // Attack collision rect size // Remove attack rect for death
		//_statistics.mel = RND->getFromIntTo(4, 15);			 // Attack power
		_currentFrameX = 0;									 // Reset frame
		_image = _image = _attacki;					         // Reset image
		_state = ENEMYSTATE_IDLE;					      	 // State change 

		_isFindPlayer = false;
	}

	RECT temp;
	RECT playerRect = _player->getRect();

	if (IntersectRect(&temp, &playerRect, &_attackRect)) // Hit the player
	{
		_player->getDamaged(5, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), _statistics.mel);
		// Knock player back in opposite direction
		_xspeed = cosf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;
		_yspeed = -sinf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;

		tagStatusEffect tStatEff;
		tStatEff.type = STATUSEFFECT_NULL;
		tStatEff.damage = 0;
		tStatEff.leftTime = 0;
		_player->addStatusEffect(tStatEff);
	}
}

void manEater::frameUpdate()
{
	// Execute frame update to render

	_frameFPS = 10;
	_frameTime++;
	if (_frameFPS <= _frameTime)
	{
		_currentFrameX++;
		_frameTime = 0;
		//if (_statistics.hp <= 0) // When last frame
		//{
		//
		//}
	}
}