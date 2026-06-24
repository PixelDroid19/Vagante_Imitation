#include "stdafx.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Map.h"
#include "UI.h"
#include "Enemy.h"

// Created by YOOJOOWON 17.11

Player::Player()
{
}


Player::~Player()
{
}
HRESULT Player::init(POINT point)
{
	_invincible = false;
	_invincibleTime = 0;
	_test = 0;

	// Image addition (to be newly added from main later)

	IMAGEMANAGER->addFrameImage("player_idle", "Img/player/player_idle.bmp", 96, 96, 2, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_jumping", "Img/player/player_jumping.bmp", 192, 96, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_moving", "Img/player/player_moving.bmp", 384, 96, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_ladderup", "Img/player/player_ladderup.bmp", 384, 96, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_ladderdown", "Img/player/player_ladderdown.bmp", 48, 96, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_holdingwall", "Img/player/player_holdingwall.bmp", 48, 96, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_attacking", "Img/player/player_attacking.bmp", 192, 96, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_lookingdown", "Img/player/player_lookingdown.bmp", 48, 96, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_lookingdownmoving", "Img/player/player_lookingdownmoving.bmp", 384, 96, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_down", "Img/player/player_down.bmp", 288, 96, 6, 2, true, RGB(255, 0, 255));


	// Initial animation and status settings
	firstSettingStat();
	_player.img = IMAGEMANAGER->findImage("player_idle");
	_animSpeed = 5;
	_animCount = 0;
	_animDelay = 0;

	// Player position and RECT
	_player.pointx = point.x;
	_player.pointy = point.y;
	_player.rc = RectMakeCenter(_player.pointx, _player.pointy, 24, 24);


	// Save tile info
	_curTileX = _prevTileX = int(_player.pointx) / TILESIZE;
	_curTileY = _prevTileY = int(_player.pointy) / TILESIZE;

	upL = _map->getMapInfo(_curTileY - 1, _curTileX - 1);
	upM = _map->getMapInfo(_curTileY - 1, _curTileX);
	upR = _map->getMapInfo(_curTileY - 1, _curTileX + 1);
	midL = _map->getMapInfo(_curTileY, _curTileX - 1);
	midM = _map->getMapInfo(_curTileY, _curTileX);
	midR = _map->getMapInfo(_curTileY, _curTileX + 1);
	botL = _map->getMapInfo(_curTileY + 1, _curTileX - 1);
	botM = _map->getMapInfo(_curTileY + 1, _curTileX);
	botR = _map->getMapInfo(_curTileY + 1, _curTileX + 1);

	_attackDelay = 0;

	return S_OK;
}
void Player::release()
{

}
void Player::update()
{
	//test~
	_player.rc = RectMakeCenter(_player.pointx, _player.pointy, 24, 24);


	// Collision temp

	
	checkInvincible();
	checkStatusEffect();
	checkHitEnemy();
	checkItemBox();


	// Check collision
	
	
	mapcollision();	
	//enemyCollision


	// Receive input if not in collision (when going to inventory in UI, _canCtrl is false)
	if (_canCtrl) keyintput();
	// Movement processing
	move();
	// Tile collision
	setmaptileInfo();

	// Collision update
	frameUpdate();
	}


void Player::render()
{

}


// Draw function   x coordinate is camera.x offset
//			y coordinate is camera.y offset!!!!
void Player::render(POINT camera)
{
	draw(camera);
}

void Player::draw(POINT camera)
{
	//test
	//Rectangle(getMemDC(), _player.rc.left + camera.x, _player.rc.top + camera.y, _player.rc.right + camera.x, _player.rc.bottom + camera.y);

	if (_invincible && _player.state != PLAYERSTATE_DEAD)
	{
		_player.img->alphaFrameRender(getMemDC(), _player.rc.left - 12 + camera.x, _player.rc.top - 24 + camera.y, _player.currentFrameX, _player.curretFrameY, int(TIMEMANAGER->getWorldTime() * 100)%2 * 200);
	}
	else
	{
		_player.img->frameRender(getMemDC(), _player.rc.left - 12 + camera.x, _player.rc.top - 24 + camera.y, _player.currentFrameX, _player.curretFrameY);
	}
	

	//_player.img->frameRender(getMemDC(), WINSIZEX / 2, WINSIZEY / 2, _player.img->getFrameX(), _player.img->getFrameY());

	if (_player.state == PLAYERSTATE_ATTACKING || _player.state == PLAYERSTATE_ATTACKING_JUMP)
	{
		//Rectangle(getMemDC(), _equipWeaponRect.left + camera.x, _equipWeaponRect.top + camera.y, _equipWeaponRect.right + camera.x, _equipWeaponRect.bottom + camera.y);

		if (_equipWeapon.name != NAME_HAND)
			if (!_player.lookingRight)
				_equipWeapon.img0->frameRender(getMemDC(), _equipWeaponRect.left + camera.x, _equipWeaponRect.top + camera.y, 1, 0);
			else
				_equipWeapon.img0->frameRender(getMemDC(), _equipWeaponRect.left + camera.x, _equipWeaponRect.top + camera.y, 0, 0);


	}


/*
	Rectangle(getMemDC(), 100, 100, 200, 200);
	char str1[256];
	char str2[256];
	char str3[256];
	char str4[256];
	char str5[256];
	sprintf(str1, "%d %d %d", upL.type, upM.type, upR.type);
	sprintf(str2, "%d %d %d", midL.type, midM.type, midR.type);
	sprintf(str3, "%d %d %d", botL.type, botM.type, botR.type);
	sprintf(str4, "%ld %ld", _map->getCoinBox(1)._openBox, _map->getCoinBox(1)._eventChk);
	sprintf(str5, "%d", _em->getEnemyVector().size());
	TextOut(getMemDC(), 120, 110, str1, strlen(str1));
	TextOut(getMemDC(), 120, 130, str2, strlen(str2));
	TextOut(getMemDC(), 120, 150, str3, strlen(str3));
	TextOut(getMemDC(), 120, 170, str4, strlen(str4));
	TextOut(getMemDC(), 120, 190, str5, strlen(str5));*/
}


void Player::setStateImg() {

	_player.currentFrameX = 0;

	switch (_player.state)
	{
	case PLAYERSTATE_IDLE:
		_player.img = IMAGEMANAGER->findImage("player_idle");
		break;
	case PLAYERSTATE_LOOKING_DOWN:
		_player.img = IMAGEMANAGER->findImage("player_lookingdown");
		break;
	case PLAYERSTATE_LOOKING_DOWN_MOVING:
		_player.img = IMAGEMANAGER->findImage("player_lookingdownmoving");
		break;
	case PLAYERSTATE_MOVING:
		_player.img = IMAGEMANAGER->findImage("player_moving");
		break;
	case PLAYERSTATE_JUMPING:
	case PLAYERSTATE_FALLING:
		_player.img = IMAGEMANAGER->findImage("player_jumping");
		break;
	case PLAYERSTATE_ATTACKING:
	case PLAYERSTATE_ATTACKING_JUMP:
	case PLAYERSTATE_ATTACKING_3:
		_player.img = IMAGEMANAGER->findImage("player_attacking");
		_animDelay = 0;
		break;
	case PLAYERSTATE_HIT:
		_player.img = IMAGEMANAGER->findImage("player_idle");
		break;
	case PLAYERSTATE_HOLDING_WALL:
		_player.img = IMAGEMANAGER->findImage("player_holdingwall");
		break;
	case PLAYERSTATE_HOLDING_LADDERUP:
		_player.img = IMAGEMANAGER->findImage("player_ladderup");
		break;
	case PLAYERSTATE_HOLDING_LADDERDOWN:
		_player.img = IMAGEMANAGER->findImage("player_ladderdown");
		break;
	case PLAYERSTATE_CHEKINGINVENTORY:
		_player.img = IMAGEMANAGER->findImage("player_idle");
		break;
	case PLAYERSTATE_DEAD:
		_player.img = IMAGEMANAGER->findImage("player_down");
		break;
	}
}

void Player::frameUpdate() {
	_animCount += TIMEMANAGER->getElapsedTime() * 75;

	if (_player.lookingRight)
		_player.curretFrameY = 0;
	else
		_player.curretFrameY = 1;


	if (_animCount % _animSpeed == 0)
	{
		_animCount = 0;

		switch (_player.state)
		{
		case PLAYERSTATE_IDLE:
			break;

		case PLAYERSTATE_MOVING:
			_animSpeed = 3;
			_player.currentFrameX++;

			if (_player.currentFrameX >= _player.img->getMaxFrameX())  _player.currentFrameX = 0;

			break;

		case PLAYERSTATE_LOOKING_DOWN:
			break;

		case PLAYERSTATE_LOOKING_DOWN_MOVING:
			_animSpeed = 3;
			_player.currentFrameX++;

			if (_player.currentFrameX >= _player.img->getMaxFrameX()) _player.currentFrameX = 0;
			break;

		case PLAYERSTATE_JUMPING:
			_animSpeed = 15;

			_player.currentFrameX++;
			if (_player.currentFrameX >= 1) _player.currentFrameX = 1;
			break;
		case PLAYERSTATE_FALLING:
			_animSpeed = 5;

			if (_player.currentFrameX >= _player.img->getMaxFrameX()) _player.currentFrameX = _player.img->getMaxFrameX();
			else _player.currentFrameX++;
			break;

		case PLAYERSTATE_ATTACKING:
		case PLAYERSTATE_ATTACKING_JUMP:
			_animSpeed = 5;
			_animDelay++;

			if (_player.currentFrameX == 3) _player.currentFrameX++;
			if (_player.currentFrameX == 2) _player.currentFrameX++;
			if (_player.currentFrameX == 1) _player.currentFrameX++;
			if (_player.currentFrameX == 0 && _animDelay > 2) _player.currentFrameX++;
				



			break;
		case PLAYERSTATE_ATTACKING_3:
			break;
		case PLAYERSTATE_HIT:
			break;
		case PLAYERSTATE_HOLDING_WALL:
			break;
		case PLAYERSTATE_HOLDING_LADDERUP:
			_animSpeed = 5;

			if (_player.yspeed > 0) {
				if (_player.currentFrameX >= _player.img->getMaxFrameX()) _player.currentFrameX = 0;
				else _player.currentFrameX++;
			}
			break;
		case PLAYERSTATE_HOLDING_LADDERDOWN:
			break;
		case PLAYERSTATE_CHEKINGINVENTORY:
			break;
		case PLAYERSTATE_DEAD:
			_animSpeed = 3;

			if (_player.currentFrameX >= _player.img->getMaxFrameX()) _player.currentFrameX = _player.img->getMaxFrameX();
			else _player.currentFrameX++;
			break;

		default:
			break;
		}
	}


//	_player.img->setX(_player.rc.left);
//	_player.img->setY(_player.rc.top);
}

void Player::move()
{

	// State must be processed separately
	switch (_player.state) {
	case PLAYERSTATE_IDLE: 

	break;

	case PLAYERSTATE_JUMPING:
		// Gravity
		_player.yspeed -= _player.gravity;
		// Gravity (clamp to max fall speed)
		if (_player.yspeed < -FALLPOWERMAX)
			_player.yspeed = -FALLPOWERMAX;

		// Gradually reduce horizontal speed when airborne (sliding friction)
		if (_player.xspeed > 0)
			_player.xspeed -= 0.2;
		else if (_player.xspeed < 0)
			_player.xspeed += 0.2;

		// Transition to FALLING state
		if (_player.yspeed < 0) {
			_player.state = PLAYERSTATE_FALLING;
			setStateImg();
		}
	break;

	case PLAYERSTATE_FALLING:
	case PLAYERSTATE_ATTACKING_JUMP:
		// Gravity
		_player.yspeed -= _player.gravity;
		// Gravity (clamp to max fall speed)
		if (_player.yspeed < -FALLPOWERMAX)
			_player.yspeed = -FALLPOWERMAX;

		// Gradually reduce horizontal speed when airborne (sliding friction)
		if (_player.xspeed > 0)
			_player.xspeed -= 0.2;
		else if (_player.xspeed < 0)
			_player.xspeed += 0.2;
	break;


	case PLAYERSTATE_ATTACKING:	
		if (_player.xspeed > 0)
			_player.xspeed -= 0.2;
		else if (_player.xspeed < 0)
			_player.xspeed += 0.2;
		if (_player.yspeed != 0)
		{
			// Gravity
			_player.yspeed -= _player.gravity;
			// Gravity (clamp to max fall speed)
			if (_player.yspeed < -FALLPOWERMAX)
				_player.yspeed = -FALLPOWERMAX;
		}
	break;

	case PLAYERSTATE_HIT:
		_player.yspeed -= _player.gravity;
		_player.xspeed -= 0.1;

	break;

	case PLAYERSTATE_HOLDING_WALL:

		_player.xspeed = 0;

	break;

	case PLAYERSTATE_DEAD:
		// Gravity
		_player.yspeed -= _player.gravity;
		// Gravity (clamp to max fall speed)
		if (_player.yspeed < -FALLPOWERMAX)
			_player.yspeed = -FALLPOWERMAX;
	break;
		
	}

	// Movement (multiply speed by spd stat factor to determine final speed)
	_player.pointx += (_player.xspeed  * (_player.stat.spd * 0.01 + 1));
	_player.pointy -= (_player.yspeed  * (_player.stat.spd * 0.01 + 1));

	

}

void Player::keyintput()
{
	/*
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_player.pointy -= 2;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		_player.pointy += 2;
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_player.pointx -= 2;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_player.pointx += 2;
	}
	*/
	//~test

	// Movement possible?
	bool canMove = true;
	// Check stun status
	for (int i = 0; i < 5; i++)
	{
		if (_player.statusEffect[i].type == STATUSEFFECT_STUN) canMove = false;
	}
	// If not stunned, take action

	if (_player.state == PLAYERSTATE_DEAD)
		canMove = false;

	if (canMove)
	{
		// Direction setting
		if (_player.state != PLAYERSTATE_HOLDING_WALL &&
			_player.state != PLAYERSTATE_ATTACKING &&
			_player.state != PLAYERSTATE_ATTACKING_JUMP) {
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
				_player.lookingRight = true;
			if (KEYMANAGER->isStayKeyDown(VK_LEFT))
				_player.lookingRight = false;
		}

		switch (_player.state) {
		case PLAYERSTATE_IDLE:

			_player.xspeed = 0;

			// Looking up at ladder
			if (KEYMANAGER->isStayKeyDown(VK_UP))
				_player.currentFrameX = 1;
			if (KEYMANAGER->isOnceKeyUp(VK_UP))
				_player.currentFrameX = 0;


			// Movement
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
				_player.xspeed = RUNPOWERSTART;
				_player.state = PLAYERSTATE_MOVING;
				setStateImg();
			}
			if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
				_player.xspeed = -RUNPOWERSTART;
				_player.state = PLAYERSTATE_MOVING;
				setStateImg();
			}



			if (KEYMANAGER->isStayKeyDown(VK_DOWN)) {
				_player.state = PLAYERSTATE_LOOKING_DOWN;
				checkDropItem();
				setStateImg();
			}

			jump();
			holdLadder();
			canDown();
			attack();

			break;


		case PLAYERSTATE_MOVING:

			// Stop moving
			if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && !_player.lookingRight) {
				_player.xspeed = 0;
				_player.state = PLAYERSTATE_IDLE;
				setStateImg();
			}
			if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && _player.lookingRight) {
				_player.xspeed = 0;
				_player.state = PLAYERSTATE_IDLE;
				setStateImg();

			}

			// Look down

			if (KEYMANAGER->isStayKeyDown(VK_DOWN)) {
				_player.state = PLAYERSTATE_LOOKING_DOWN_MOVING;
				setStateImg();
			}

			// Acceleration
			if (KEYMANAGER->isStayKeyDown(VK_LEFT) && !_player.lookingRight) {
				_player.xspeed -= 1;
			}
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _player.lookingRight) {
				_player.xspeed += 1;
			}
			if (_player.xspeed > RUNPOWERMAX)
				_player.xspeed = RUNPOWERMAX;
			if (_player.xspeed < -RUNPOWERMAX)
				_player.xspeed = -RUNPOWERMAX;


			jump();
			holdLadder();
			canDown();
			attack();

			break;

		case PLAYERSTATE_LOOKING_DOWN:
			if (KEYMANAGER->isOnceKeyUp(VK_DOWN)) {
				_player.state = PLAYERSTATE_IDLE;
				setStateImg();
			}

			if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
				_player.xspeed = RUNPOWERSTART * 0.5;
				_player.state = PLAYERSTATE_LOOKING_DOWN_MOVING;
				setStateImg();
			}
			if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
				_player.xspeed = -RUNPOWERSTART * 0.5;
				_player.state = PLAYERSTATE_LOOKING_DOWN_MOVING;
				setStateImg();
			}

			attack();
			jump();
			canDown();

			break;

		case PLAYERSTATE_LOOKING_DOWN_MOVING:
			// Stop moving
			if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && !_player.lookingRight) {
				_player.xspeed = 0;
				_player.state = PLAYERSTATE_LOOKING_DOWN;
				setStateImg();
			}
			if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && _player.lookingRight) {
				_player.xspeed = 0;
				_player.state = PLAYERSTATE_LOOKING_DOWN;
				setStateImg();
			}

			// Stand up
			if (KEYMANAGER->isOnceKeyUp(VK_DOWN)) {
				_player.state = PLAYERSTATE_MOVING;
				setStateImg();
			}

			// Acceleration
			if (KEYMANAGER->isStayKeyDown(VK_LEFT) && !_player.lookingRight) {
				_player.xspeed -= 1 * 0.5;
			}
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _player.lookingRight) {
				_player.xspeed += 1 * 0.5;
			}
			if (_player.xspeed > RUNPOWERMAX * 0.5)
				_player.xspeed = RUNPOWERMAX * 0.5;
			if (_player.xspeed < -RUNPOWERMAX * 0.5)
				_player.xspeed = -RUNPOWERMAX * 0.5;


			jump();
			holdLadder();
			canDown();
			attack();

			break;


		case PLAYERSTATE_JUMPING:

			// Climb higher while holding Z button
			if (KEYMANAGER->isStayKeyDown(VK_Z) && _player.yspeed > 3) {
				_player.yspeed += 0.3;
			}

			// Left/right movement in air
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
				_player.xspeed += 0.5;
				if (_player.xspeed > RUNPOWERMAX)
					_player.xspeed = RUNPOWERMAX;
			}
			if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
				_player.xspeed -= 0.5;
				if (_player.xspeed < -RUNPOWERMAX)
					_player.xspeed = -RUNPOWERMAX;
			}


			holdLadder();
			attackjump();

			break;

		case PLAYERSTATE_FALLING:


			// Left/right movement in air
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
				_player.xspeed += 0.5;
				if (_player.xspeed > RUNPOWERMAX)
					_player.xspeed = RUNPOWERMAX;
			}

			if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
				_player.xspeed -= 0.5;
				if (_player.xspeed < -RUNPOWERMAX)
					_player.xspeed = -RUNPOWERMAX;
			}


			holdLadder();
			attackjump();

			break;


		case PLAYERSTATE_ATTACKING:
			attackingNow();

			// Jump is allowed during attack (handled inside function)
			jump();

			if (_player.currentFrameX >= _player.img->getMaxFrameX()) {
				_player.state = PLAYERSTATE_IDLE;
				setStateImg();
			}
			break;

		case PLAYERSTATE_ATTACKING_JUMP:
			attackingNow();

			// Left/right movement in air
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
				_player.xspeed += 0.5;
				if (_player.xspeed > RUNPOWERMAX)
					_player.xspeed = RUNPOWERMAX;
			}

			if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
				_player.xspeed -= 0.5;
				if (_player.xspeed < -RUNPOWERMAX)
					_player.xspeed = -RUNPOWERMAX;
			}


			// Climb higher while holding Z button
			if (KEYMANAGER->isStayKeyDown(VK_Z) && _player.yspeed > 0) {
				_player.yspeed += 0.3;
			}

			if (_player.currentFrameX >= _player.img->getMaxFrameX()) {
				_player.state = PLAYERSTATE_FALLING;
				setStateImg();
			}
			break;

		case PLAYERSTATE_ATTACKING_3:

			break;
		case PLAYERSTATE_HIT:

			break;
		case PLAYERSTATE_HOLDING_WALL:

			// Jump while holding wall
			// Jump without climbing again, move slightly in opposite direction, then perform wall jump
			if (KEYMANAGER->isOnceKeyDown(VK_Z)) {
				_player.yspeed = JUMPPOWERSTART * 0.8;
				if (_player.lookingRight) {
					_player.pointx -= 5;
					_player.xspeed = RUNPOWERSTART;
				}
				else {
					_player.pointx += 5;
					_player.xspeed = -RUNPOWERSTART;
				}
				_player.state = PLAYERSTATE_JUMPING;
				setStateImg();
			}


			attackjump();



			/*
			// Move in opposite direction while holding wall
			// If looking right, pressing left reverses direction
			if (_player.lookingRight)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_LEFT)) {
					_player.pointx -= 1;
					_player.yspeed = -3;
					_player.xspeed = -RUNPOWERSTART;
					_player.state = PLAYERSTATE_FALLING;

					_player.lookingRight = false;
				}
			}
			else
			{
				if (KEYMANAGER->isOnceKeyDown(VK_RIGHT)) {
					_player.pointx += 1;
					_player.yspeed = -3;
					_player.xspeed = RUNPOWERSTART;
					_player.state = PLAYERSTATE_FALLING;

					_player.lookingRight = true;
				}
			}

			// Move downward while holding wall
			// Without climbing again, move slightly in opposite direction then fall
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN)) {
				_player.yspeed = -5;
				if (_player.lookingRight) {
					_player.pointx -= 2;
					_player.xspeed = -RUNPOWERSTART * 0.2;
				}
				else
				{
					_player.pointx += 2;
					_player.xspeed = +RUNPOWERSTART * 0.2;
				}
				_player.state = PLAYERSTATE_FALLING;
			}

			*/

			break;


		case PLAYERSTATE_HOLDING_LADDERUP:

			if ((midM.type == MAPTILE_LADDER || upM.type == MAPTILE_LADDER)
				&& KEYMANAGER->isStayKeyDown(VK_UP)) {
				_player.yspeed = LADDERUPSPEED;
			}
			else {
				_player.yspeed = 0;
			}


			if (KEYMANAGER->isOnceKeyDown(VK_DOWN)) {
				_player.pointy += 1;
				_player.state = PLAYERSTATE_HOLDING_LADDERDOWN;
				setStateImg();
			}

			jump();
			attackjump();

			break;


		case PLAYERSTATE_HOLDING_LADDERDOWN:


			if ((midM.type == MAPTILE_LADDER || botM.type == MAPTILE_LADDER)
				&& KEYMANAGER->isStayKeyDown(VK_DOWN))
			{
				_player.yspeed = LADDERDOWNSPEED;
			}
			else
				_player.yspeed = 0;

			if (KEYMANAGER->isOnceKeyDown(VK_UP)) {
				_player.pointy -= 1;
				_player.state = PLAYERSTATE_HOLDING_LADDERUP;
				setStateImg();
			}

			jump();
			attackjump();

			break;
		case PLAYERSTATE_CHEKINGINVENTORY:

			break;
		}
	}

	/*
	if (canMove)
	{
		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			_player.state = PLAYERSTATE_LOOKING_UP;
		}
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			_player.state = PLAYERSTATE_LOOKING_DOWN;
		}
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			_player.state = PLAYERSTATE_MOVING;
			_player.lookingLeft = true;
			_player.lookingRight = false;
			_player.pointx -= _player.stat.spd;
		}
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			_player.state = PLAYERSTATE_MOVING;
			_player.lookingLeft = false;
			_player.lookingRight = true;
			_player.pointx += _player.stat.spd;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_Z))
		{
			jump();
		}
		if (KEYMANAGER->isOnceKeyDown(VK_X))
		{
			attack();
		}
		if (KEYMANAGER->isOnceKeyDown(VK_C))
		{

		}
		if (KEYMANAGER->isOnceKeyDown(VK_V))
		{

		}
	}
	*/

}
void Player::jump()
{
	if (KEYMANAGER->isOnceKeyDown(VK_Z)) {
		_player.yspeed = JUMPPOWERSTART;
		_player.gravity = 0.4;

		if (_player.state != PLAYERSTATE_ATTACKING) {
			_player.yspeed = JUMPPOWERSTART;
			_player.state = PLAYERSTATE_JUMPING;
			setStateImg();
		}
		else {
			_player.yspeed = JUMPPOWERSTART;
			_player.state = PLAYERSTATE_ATTACKING_JUMP;
		}
		SOUNDMANAGER->play("2_Player_Jump_Sound", 0.5);
	}
}
void Player::attack()
{
	if (KEYMANAGER->isOnceKeyDown(VK_X)) {
		float _offsetX, _offsetY;

		// Temporary offset (will be replaced with image size later)
		if (_equipWeapon.name == NAME_HAND)
		{
			_offsetY = 5;
			_offsetX = (_player.lookingRight) ? 5 : -5;
		}
		else {
			_offsetY = _equipWeapon.img0->getHeight();
			_offsetX = (_player.lookingRight) ? _equipWeapon.img0->getFrameWidth() : -_equipWeapon.img0->getFrameWidth();
		}

		if (_offsetX > 0)
			_equipWeaponRect.set(_player.pointx, _player.pointy, _player.pointx + _offsetX, _player.pointy + _offsetY);
		else
			_equipWeaponRect.set(_player.pointx + _offsetX, _player.pointy, _player.pointx, _player.pointy + _offsetY);

		_player.state = PLAYERSTATE_ATTACKING;
		setStateImg();
		SOUNDMANAGER->play("1_Player_Attack_Sound", 0.5);
	}
}

void Player::attackjump()
{
	if (KEYMANAGER->isOnceKeyDown(VK_X)) {
		float _offsetX, _offsetY;

		// Temporary offset
		if (_equipWeapon.name == NAME_HAND)
		{
			_offsetY = 5;
			_offsetX = (_player.lookingRight) ? 5 : -5;
		}
		else {
			_offsetY = _equipWeapon.img0->getHeight();
			_offsetX = (_player.lookingRight) ? _equipWeapon.img0->getFrameWidth() : -_equipWeapon.img0->getFrameWidth();
		}

		if (_offsetX > 0)
			_equipWeaponRect.set(_player.pointx, _player.pointy, _player.pointx + _offsetX, _player.pointy + _offsetY);
		else
			_equipWeaponRect.set(_player.pointx + _offsetX, _player.pointy, _player.pointx, _player.pointy + _offsetY);

		_player.state = PLAYERSTATE_ATTACKING_JUMP; 
		setStateImg();
		SOUNDMANAGER->play("1_Player_Attack_Sound", 0.5);
	}
}

void Player::attackingNow() {

	float _offsetX = 0, _offsetY = 0;

	if (_player.currentFrameX == 0) { _offsetY = 0; _offsetX = (_player.lookingRight) ? 5 : -5;}
	if (_player.currentFrameX == 1) { _offsetY = 0; _offsetX = (_player.lookingRight) ? 5  + _equipWeaponRect.getWidth() * 0.5 : -5 - _equipWeaponRect.getWidth() * 0.5;}
	if (_player.currentFrameX == 2) { _offsetY = 2; _offsetX = (_player.lookingRight) ? 7 : -7;}
	if (_player.currentFrameX == 3) { _attackDelay = 1;}
	
	_equipWeaponRect.setCenterPos(_player.pointx + _offsetX, _player.pointy + _offsetY);
}

void Player::attackCollision() {

}

void Player::holdLadder()
{
	if (midM.type == MAPTILE_LADDER && KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		_player.pointx = midM.rc.left + (midM.rc.right - midM.rc.left) * 0.5;
		_player.pointy -= 1;
		_player.state = PLAYERSTATE_HOLDING_LADDERUP;
		_player.xspeed = 0;
		_player.yspeed = 0;
		setStateImg();
	}

}

void Player::canDown()
{
	if ((botM.type == MAPTILE_GROUND_CAN_GO_DOWN_1)
		&& KEYMANAGER->isStayKeyDown(VK_DOWN) && KEYMANAGER->isStayKeyDown(VK_Z)) {
		_player.state = PLAYERSTATE_FALLING;
		_player.yspeed = 0;
		_player.pointy = botM.rc.top + (_player.rc.bottom - _player.rc.top) * 0.25;
		setStateImg();
	}


}

void Player::drawDebug(POINT camera)
{
	if (!isDebugDrawEnabled())
		return;

	SDL_Renderer* dc = getMemDC();
	const float px = _player.pointx;
	const float py = _player.pointy;
	const int screenX = (int)px + camera.x;
	const int screenY = (int)py + camera.y;

	if (isDebugLayerEnabled(DEBUG_LAYER_PLAYER_9))
	{
		const mapInfo tiles[9] = { upL, upM, upR, midL, midM, midR, botL, botM, botR };
		for (int i = 0; i < 9; i++)
		{
			debugDrawFilledRect(dc, tiles[i].rc, camera, DebugPalette::PlayerFill);
			debugDrawRect(dc, tiles[i].rc, camera, DebugPalette::Player);
		}
	}

	if (isDebugLayerEnabled(DEBUG_LAYER_RAYS) && _map)
	{
		const int rayCount = 28;
		const float maxDist = 420.f;
		for (int i = 0; i < rayCount; i++)
		{
			float angle = (PI2 * i) / rayCount;
			float endX = px;
			float endY = py;
			for (float dist = (float)TILESIZE; dist < maxDist; dist += TILESIZE * 0.5f)
			{
				float testX = px + cosf(angle) * dist;
				float testY = py - sinf(angle) * dist;
				int ty = (int)testY / TILESIZE;
				int tx = (int)testX / TILESIZE;
				int tileType = _map->getMapInfo(ty, tx).type;
				if (tileType == MAPTILE_WALL || tileType == MAPTILE_WALL2)
					break;
				endX = testX;
				endY = testY;
			}
			debugDrawLine(dc, screenX, screenY,
				(int)endX + camera.x, (int)endY + camera.y, DebugPalette::Ray);
		}

		if (_em)
		{
			vector<Enemy*> enemies = _em->getEnemyVector();
			for (size_t i = 0; i < enemies.size(); i++)
			{
				POINT target = enemies[i]->getPoint();
				float dist = getDistance(px, py, (float)target.x, (float)target.y);
				float angle = getAngle(px, py, (float)target.x, (float)target.y);
				float endX = px;
				float endY = py;
				for (float step = (float)TILESIZE; step < dist; step += TILESIZE * 0.5f)
				{
					float testX = px + cosf(angle) * step;
					float testY = py - sinf(angle) * step;
					int ty = (int)testY / TILESIZE;
					int tx = (int)testX / TILESIZE;
					int tileType = _map->getMapInfo(ty, tx).type;
					if (tileType == MAPTILE_WALL || tileType == MAPTILE_WALL2)
						break;
					endX = testX;
					endY = testY;
				}
				debugDrawLine(dc, screenX, screenY,
					(int)endX + camera.x, (int)endY + camera.y, DebugPalette::RayTarget);
			}
		}
	}

	if (isDebugLayerEnabled(DEBUG_LAYER_HITBOX))
	{
		debugDrawRect(dc, _player.rc, camera, DebugPalette::Player);
		debugDrawCrosshair(dc, screenX, screenY, 5, DebugPalette::Player);

		if (_player.state == PLAYERSTATE_ATTACKING || _player.state == PLAYERSTATE_ATTACKING_JUMP)
			debugDrawRect(dc, _equipWeaponRect, camera, DebugPalette::Weapon);
	}
}

void Player::addStatusEffect(tagStatusEffect statuseffect)
{
	// Add status effect!
	for (int i = 0; i < 5; i++)
	{
		if (_player.statusEffect[i].type == statuseffect.type)
		{
			if (_player.statusEffect[i].leftTime < statuseffect.leftTime)
				_player.statusEffect[i] = statuseffect;
			break;
		}
		if (_player.statusEffect[i].type == NULL)
		{			
			_player.statusEffect[i] = statuseffect;
			break;			
		}
	}
}
void Player::setmaptileInfo()
{
	/* Tile type
	type

	MAPTILE_NULL,
	MAPTILE_WALL,
	MAPTILE_WALL2,
	MAPTILE_LADDER, // Ladder
	MAPTILE_GROUND_CAN_GO_DOWN_1, // Ground
	MAPTILE_GROUND_CAN_GO_DOWN_2, // Ground
	MAPTILE_TRAP_SPIKE,
	MAPTILE_TRAP_ARROW,
	MAPTILE_TRAP_ROCK
	*/
	// Get info of 9 tiles around the character
	_curTileX = int(_player.pointx) / TILESIZE;
	_curTileY = int(_player.pointy) / TILESIZE;

	if (_curTileX != _prevTileX || _curTileY != _prevTileY) {
		upL = _map->getMapInfo(_curTileY - 1, _curTileX - 1);
		upM = _map->getMapInfo(_curTileY - 1, _curTileX);
		upR = _map->getMapInfo(_curTileY - 1, _curTileX + 1);
		midL = _map->getMapInfo(_curTileY, _curTileX - 1);
		midM = _map->getMapInfo(_curTileY, _curTileX);
		midR = _map->getMapInfo(_curTileY, _curTileX + 1);
		botL = _map->getMapInfo(_curTileY + 1, _curTileX - 1);
		botM = _map->getMapInfo(_curTileY + 1, _curTileX);
		botR = _map->getMapInfo(_curTileY + 1, _curTileX + 1);

		_prevTileX = _curTileX;
		_prevTileY = _curTileY;

	}

	// Tile activation processing

	// Check tile below player

	//midM
	//1. (If speed is downward) push out from wall when inside

	switch (midM.type) {

		// Wall type, pushes player outside
	case MAPTILE_WALL: case MAPTILE_WALL2:

		// If falling, when feet land on the floor, transition to idle state
		if (_player.state == PLAYERSTATE_FALLING) {
			_player.pointy = midM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
			_player.currentFrameX = 8;
			_player.xspeed = 0;
			_player.yspeed = 0;
			_player.state = PLAYERSTATE_IDLE;
			setStateImg();
		}

		// If ATTACKING_JUMP, land while keeping attack state
		if (_player.state == PLAYERSTATE_ATTACKING_JUMP) {
			_player.pointy = midM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
			_player.xspeed = 0;
			_player.yspeed = 0;
			_player.state = PLAYERSTATE_ATTACKING;
		}

		// If jumping, pop out downward when head hits ceiling
		if (_player.state == PLAYERSTATE_JUMPING) {
			_player.state = PLAYERSTATE_FALLING;
			_player.yspeed = -3;
			_player.pointy = midM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
			setStateImg();
		}

		// If holding ladder up state, push out downward
		if (_player.state == PLAYERSTATE_HOLDING_LADDERUP) {
			_player.pointy = midM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
			_player.yspeed = 0;
		}


		if (_player.state == PLAYERSTATE_DEAD) {
			_player.pointy = midM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
			_player.yspeed = 0;
		}
		break;

		// If ground can be fallen through, no need to push player out

	case MAPTILE_GROUND_CAN_GO_DOWN_1:
		// If falling, when feet land on floor, transition to idle state
		// It's possible to be falling while moving downward. This must be checked against player position and speed
		if (_player.state == PLAYERSTATE_FALLING
			&& _player.yspeed < -FALLPOWERMAX * 0.75) {
			_player.pointy = midM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5f);
			_player.currentFrameX = 8;
			_player.xspeed = 0;
			_player.yspeed = 0;
			_player.state = PLAYERSTATE_IDLE;
			setStateImg();
		}
		break;

	case MAPTILE_SPIKE_TRAP:
		if (_player.state == PLAYERSTATE_FALLING)
			getDamaged(999);
		break;
	}


	//botM
	// Basic ground collision

	switch (botM.type)
	{
		// Nothing or trap
	case MAPTILE_NULL:
	case MAPTILE_SPIKE_TRAP:

		// If idle or moving with nothing below, fall
		if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING
			|| _player.state == PLAYERSTATE_LOOKING_DOWN || _player.state == PLAYERSTATE_LOOKING_DOWN_MOVING) {
			_player.state = PLAYERSTATE_FALLING;
			setStateImg();
			_player.yspeed = -3;
		}

		// If ladder tile exists, keep position on it
		if (isCollision(_player.rc, botM.rc) && _player.state == PLAYERSTATE_HOLDING_LADDERDOWN) {
			_player.pointy = botM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
		}

		// If attacking while nothing below, transition to jump attack
		if (_player.state == PLAYERSTATE_ATTACKING) {
			_player.state = PLAYERSTATE_ATTACKING_JUMP;
		}
		break;


		// Ladder tile
		case MAPTILE_LADDER:
		// If idle or moving with nothing below, fall
		if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING
			|| _player.state == PLAYERSTATE_LOOKING_DOWN || _player.state == PLAYERSTATE_LOOKING_DOWN_MOVING) {
			_player.state = PLAYERSTATE_FALLING;
			setStateImg();
			_player.yspeed = -3;
		}

		// If attacking while nothing below, transition to jump attack
		if (_player.state == PLAYERSTATE_ATTACKING) {
			_player.state = PLAYERSTATE_ATTACKING_JUMP;
		}
		break;


		// Wall & passable ground
	case MAPTILE_WALL: case MAPTILE_WALL2:  case MAPTILE_GROUND_CAN_GO_DOWN_1:
		// Stand on ground on collision
		if (isCollision(_player.rc, botM.rc))
		{
			// If falling or holding ladder down, land on the ground
			if (_player.state == PLAYERSTATE_FALLING || _player.state == PLAYERSTATE_HOLDING_LADDERDOWN) {
				_player.pointy = botM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
				_player.currentFrameX = 8;
				_player.xspeed = 0;
				_player.yspeed = 0;
				_player.state = PLAYERSTATE_IDLE;
				setStateImg();
			}

			// If attacking jump, continue attacking on landing (Attacking jump flag added, when y speed is downward)
			if (_player.state == PLAYERSTATE_ATTACKING_JUMP && _player.yspeed < 0) {
				_player.pointy = botM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
				_player.xspeed = 0;
				//_player.yspeed = 0; (Keep attack momentum)
				_player.state = PLAYERSTATE_ATTACKING;			
			}

			if (_player.state == PLAYERSTATE_DEAD) {
				_player.pointy = botM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
			}
		}
		break;
		// Ladder case
	}


	/*
	//botR, botL
	// Diagonal and outside position catch handling

	// Temporary minimal processing using collision

	if (_player.state != PLAYERSTATE_IDLE && _player.state != PLAYERSTATE_MOVING) {
		switch (botR.type)
		{
			// Nothing below
		case MAPTILE_NULL:	case MAPTILE_LADDER:
			if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING) {
				_player.state = PLAYERSTATE_FALLING;
				_player.yspeed = -3;
			}
			break;
			// Wall & passable ground
		case MAPTILE_WALL: case MAPTILE_WALL2:  case MAPTILE_GROUND_CAN_GO_DOWN_1: case MAPTILE_GROUND_CAN_GO_DOWN_2:
			// Stand on ground on collision
			if (isCollision(_player.rc, botR.rc))
			{
				if (_player.state == PLAYERSTATE_FALLING) {
					_player.pointy = botR.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
					_player.currentFrameX = 8;
					_player.xspeed = 0;
					_player.yspeed = 0;
					_player.state = PLAYERSTATE_IDLE;
				}
				// Add
			}
			break;
			// Ladder case
		}

		switch (botL.type)
		{
			// Nothing below
		case MAPTILE_NULL:	case MAPTILE_LADDER:
			if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING) {
				_player.state = PLAYERSTATE_FALLING;
				_player.yspeed = -3;
			}
			break;
			// Wall & passable ground
		case MAPTILE_WALL: case MAPTILE_WALL2:  case MAPTILE_GROUND_CAN_GO_DOWN_1: case MAPTILE_GROUND_CAN_GO_DOWN_2:
			// Stand on ground on collision
			if (isCollision(_player.rc, botL.rc))
			{
				if (_player.state == PLAYERSTATE_FALLING) {
					_player.pointy = botL.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
					_player.currentFrameX = 8;
					_player.xspeed = 0;
					_player.yspeed = 0;
					_player.state = PLAYERSTATE_IDLE;
				}
				// Add
			}
			break;
			// Ladder case
		}
	}

	*/

	//midR
	// Right side wall, push from right
	switch (midR.type) {

	case MAPTILE_WALL: case MAPTILE_WALL2:
		if (_player.rc.right > midR.rc.left) {
			_player.pointx = midR.rc.left - ((_player.rc.right - _player.rc.left) * 0.5);
			_player.xspeed = 0;


			if ((upR.type != MAPTILE_WALL && upR.type != MAPTILE_WALL2) &&
				(_player.rc.top > midR.rc.top - 10 && _player.rc.top < midR.rc.top) && !_invincible)
			{
				_player.pointy = midR.rc.top + (_player.rc.bottom - _player.rc.top) * 0.5 + 2;
				_player.state = PLAYERSTATE_HOLDING_WALL;
				_player.xspeed = 0;
				_player.yspeed = 0;
				setStateImg();
			}
		}
		break;
	}


	//midL
	// Left side wall, push from left
	switch (midL.type) {

	case MAPTILE_WALL: case MAPTILE_WALL2:
		if (_player.rc.left < midL.rc.right) {
			_player.pointx = midL.rc.right + ((_player.rc.right - _player.rc.left) * 0.5);
			_player.xspeed = 0;
			
			if ((upL.type != MAPTILE_WALL && upL.type != MAPTILE_WALL2)
				&& (_player.rc.top > midL.rc.top - 10 && _player.rc.top < midL.rc.top) && !_invincible)
			{
				_player.pointy = midL.rc.top + (_player.rc.bottom - _player.rc.top) * 0.5 + 2;
				_player.state = PLAYERSTATE_HOLDING_WALL;
				_player.xspeed = 0;
				_player.yspeed = 0;
				setStateImg();
			}
		}
		break;
	}


	//upM
	switch (upM.type) {
	case MAPTILE_NULL: case MAPTILE_GROUND_CAN_GO_DOWN_1:
		if (isCollision(_player.rc, upM.rc) && _player.state == PLAYERSTATE_HOLDING_LADDERUP) {
			_player.pointy = upM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5;
		}
		break;

	case MAPTILE_WALL: case MAPTILE_WALL2:
		if (isCollision(_player.rc, upM.rc))
		{
			if (_player.state == PLAYERSTATE_JUMPING) {
				_player.state = PLAYERSTATE_FALLING;
				_player.yspeed = 0;
				_player.pointy = upM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
				setStateImg();
			}

			if (_player.state == PLAYERSTATE_HOLDING_LADDERUP) {
				_player.pointy = upM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5;
			}

			if (_player.state == PLAYERSTATE_ATTACKING_JUMP) {
				_player.yspeed = 0;
				_player.pointy = upM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
			}

		}
		break;
	}


	/*
	switch (downtile.type)
	{
	case MAPTILE_NULL:
		break;
	case MAPTILE_WALL:
		break;
	case MAPTILE_WALL2:
		break;
	case MAPTILE_LADDER:
		break;
	case MAPTILE_GROUND_CAN_GO_DOWN_1:
		break;
	case MAPTILE_GROUND_CAN_GO_DOWN_2:
		break;
	case MAPTILE_TRAP_SPIKE:
		break;
	case MAPTILE_TRAP_ARROW:
		break;
	case MAPTILE_TRAP_ROCK:
		break;
	}
	*/


}

void Player::enemyCollision() {
}


// When hit by damage (no knockback)
void Player::getDamaged(int damage) {
	
	if (!_invincible) {
		_player.stat.hp -= damage;
		_ui->hitOutput(_player.pointx, _player.pointy, damage, LETTER_RED);
		
		_invincible = true;
		_invincibleTime = 1;

		SOUNDMANAGER->play("3_Player_Damage_Sound", 0.5);
	}

	if (_player.stat.hp <= 0 && _player.state != PLAYERSTATE_DEAD) {
		_player.stat.hp = 0;
		_player.state = PLAYERSTATE_DEAD;
		setStateImg();
		_player.xspeed = 0;
		_player.yspeed -= 3;


		SOUNDMANAGER->stop("0_boss_Backgound_Music");
		SOUNDMANAGER->stop("7_DarkCave_Music");
		SOUNDMANAGER->play("4_Player_Death_Music", 1);
	}

}
// When hit by damage (angle & knockback)

void Player::getDamaged(int damage, float angle, float knockbackpower) {
	if (!_invincible) {
		_player.stat.hp -= damage;

		_ui->hitOutput(_player.pointx, _player.pointy, damage, LETTER_RED);

		float xKnock = cosf(angle)*knockbackpower;
		float yKnock = -(sinf(angle)*knockbackpower);

		_player.xspeed = xKnock;
		_player.yspeed = yKnock;

		if (_player.state == PLAYERSTATE_ATTACKING || _player.state == PLAYERSTATE_ATTACKING_JUMP)
			_player.state == PLAYERSTATE_ATTACKING_JUMP;
		else
			_player.state = PLAYERSTATE_JUMPING;

		_invincible = true;

		_invincibleTime = 0.5;

		SOUNDMANAGER->play("3_Player_Damage_Sound", 0.5);
	}


	if (_player.stat.hp <= 0 && _player.state != PLAYERSTATE_DEAD) {
		_player.stat.hp = 0;
		_player.state = PLAYERSTATE_DEAD;
		setStateImg();
		_player.xspeed = 0;
		_player.yspeed -= 3;

		
		SOUNDMANAGER->stop("0_boss_Backgound_Music");
		SOUNDMANAGER->stop("7_DarkCave_Music");
		SOUNDMANAGER->play("4_Player_Death_Music", 1);
	}

}


// Status effects (buff/debuff)
void Player::checkStatusEffect() {

	if (_player.state != PLAYERSTATE_DEAD) {
		_statusEffectTimer++;

		if (_statusEffectTimer > 1200) {
			_statusEffectTimer = 0;
		}

		for (int i = 0; i < 5; i++)
		{
			_player.statusEffect[i].leftTime -= TIMEMANAGER->getElapsedTime();
		}

		// Effect triggers every interval (should time differ per status effect..?)
		if ((int)_statusEffectTimer % 300 == 0) {
			for (int i = 0; i < 5; i++) {
				switch (_player.statusEffect[i].type)
				{
				case STATUSEFFECT_NULL:
					break;
					// Poison :: damage over time
				case STATUSEFFECT_POISON:
					getDamaged(_player.statusEffect[i].damage);
					break;

					// Fire : knockback while dealing damage
					// (Currently raises gravity to increase fall speed / duration)
				case STATUSEFFECT_FIRE:
					if (_player.lookingRight)
						getDamaged(_player.statusEffect[i].damage, PI, 1);
					else
						getDamaged(_player.statusEffect[i].damage, 0, 1);
					break;
					// Stun...?
				case STATUSEFFECT_STUN:
					break;
					// Heal : recover to max hp
				case STATUSEFFECT_HEAL:
					int maxHP = 50 + _player.stat.vit * 10;
					if (_player.stat.hp >= maxHP)
						_player.stat.hp = maxHP;
					else
						_player.stat.hp += _player.statusEffect[i].damage;

					_ui->hitOutput(_player.pointx, _player.pointy, _player.statusEffect[i].damage, LETTER_GREEN);

					break;
				}
				if (_player.statusEffect[i].leftTime < 0)
				{
					_player.statusEffect[i].type = STATUSEFFECT_NULL;
				}
			}
		}
	}
}

void Player::checkItemBox() {
	for (int i = 0; i < ITEMBOXMAX; i++) {
		tagObj box = _map->getitemBox(i);
		if (isCollision(_player.rc, box.rc))
		{
			_map->setItemBox(i, true);
		}
	}
	for (int i = 0; i < COINBOXMAX; i++) {
		tagObj box = _map->getCoinBox(i);
		if (isCollision(_player.rc, box.rc))
		{
			_map->setCoinBox(i, true);
		}
	}
}

void Player::checkDropItem() {
	_vItem = _ui->getvItem();
	
	for (int i = 0; i < _vItem.size(); i++) {
		tagItem item = _vItem[i];
		if (isCollision(_player.rc, item.rc) && item.name != NAME_COIN)
		{
			_ui->setItemToBag(_vItem[i].name);
			_ui->deleteItemOnMap(i);
			
			break;
		}
	}
	
}

void Player::checkHitEnemy() {
	_attackDelay += TIMEMANAGER->getElapsedTime();

	if (_attackDelay > 1 && (_player.state == PLAYERSTATE_ATTACKING || _player.state == PLAYERSTATE_ATTACKING_JUMP))
	{
		_vEnemyRange = _em->getEnemyVector();

		for (int i = 0; i < _vEnemyRange.size() + 1; i++) {
			
			int damage = RND->getFromIntTo(_equipWeapon.minDmg, _equipWeapon.maxDmg);
			
			MYRECT enemyRect;
			RECT temp;
			if (i < _vEnemyRange.size() && _vEnemyRange[i]->getStat().hp > 0) {
				temp = _vEnemyRange[i]->getRect();
				enemyRect.set(temp.left, temp.top, temp.right, temp.bottom);

				if (isCollision(enemyRect, _equipWeaponRect) && _player.currentFrameX == 2) {
					_vEnemyRange[i]->getDamaged(damage, getAngle(_player.pointx, _player.pointy, _vEnemyRange[i]->getPoint().x, _vEnemyRange[i]->getPoint().y), 3);
					_attackDelay = 0;
				}
			}
			else {
				if (_em->getBoss()->getStat().hp > 0)
				{
					temp = _em->getBoss()->getRect();
					enemyRect.set(temp.left, temp.top, temp.right, temp.bottom);
					if (isCollision(enemyRect, _equipWeaponRect) && _player.currentFrameX == 2) {
						_em->getBoss()->getDamaged(damage, getAngle(_player.pointx, _player.pointy, _em->getBoss()->getPoint().x, _em->getBoss()->getPoint().y), 3);
						_attackDelay = 0;
					}
				}
			}
		}
	}
}


void Player::checkInvincible() {
	if (_player.state != PLAYERSTATE_DEAD) {
		if (_invincible)
			_invincibleTime -= TIMEMANAGER->getElapsedTime();
		if (_invincibleTime < 0) {
			_invincible = false;
			_invincibleTime = 0;
		}
	}
}


void Player::firstSettingStat() {
	for (int i = 0; i < 5; i++)_player.statusEffect[i].type = STATUSEFFECT_NULL;

	_player.stat.hp = 100;
	_player.stat.str = 10;
	_player.stat.dex = 10;
	_player.stat.vit = 5;
	_player.stat.inl = 10;
	_player.stat.lck = 10;
	_player.stat.def = 10;
	_player.stat.fir = 10;
	_player.stat.ice = 10;
	_player.stat.lgt = 10;
	_player.stat.psn = 10;
	_player.stat.mel = 10;
	_player.stat.rng = 10;
	_player.stat.crit = 10;
	_player.stat.aspd = 10;
	_player.stat.spd = 10;
	_canCtrl = true;


	_player.jumppower = 0;
	_player.lookingLeft = true;
	_player.lookingRight = false;
	_player.xspeed = 0;
	_player.yspeed = 0;
	_player.money = 0;
	_player.gravity = 0.3;

	_player.state = PLAYERSTATE_FALLING;
	_player.currentFrameX = 0;
	_player.curretFrameY = 0;

}

void Player::mapcollision()
{
	_player.rc = RectMakeCenter(_player.pointx, _player.pointy, 24, 24);
	RECT temp;
	if ((midL.type == MAPTILE_WALL || midL.type == MAPTILE_WALL2) && IntersectRect(&temp, &midL.rc, &_player.rc))
	{
		_player.pointx += temp.right - temp.left;
		if ((upL.type != MAPTILE_WALL && upL.type != MAPTILE_WALL2)
			&& (_player.rc.top > midL.rc.top - 10 && _player.rc.top < midL.rc.top) && !_invincible)
		{
			_player.pointy = midL.rc.top + (_player.rc.bottom - _player.rc.top) * 0.5 + 2;
			_player.state = PLAYERSTATE_HOLDING_WALL;
			_player.xspeed = 0;
			_player.yspeed = 0;
			setStateImg();
		}
	}
	if ((midR.type == MAPTILE_WALL || midR.type == MAPTILE_WALL2) && IntersectRect(&temp, &midR.rc, &_player.rc))
	{
		_player.pointx -= temp.right - temp.left;
		if ((upR.type != MAPTILE_WALL && upR.type != MAPTILE_WALL2) &&
			(_player.rc.top > midR.rc.top - 10 && _player.rc.top < midR.rc.top) && !_invincible)
		{
			_player.pointy = midR.rc.top + (_player.rc.bottom - _player.rc.top) * 0.5 + 2;
			_player.state = PLAYERSTATE_HOLDING_WALL;
			_player.xspeed = 0;
			_player.yspeed = 0;
			setStateImg();
		}
	}
	if ((upM.type == MAPTILE_WALL || upM.type == MAPTILE_WALL2) && IntersectRect(&temp, &upM.rc, &_player.rc))
	{
		_player.pointy += temp.bottom - temp.top;
	}
	if ((botM.type == MAPTILE_WALL || botM.type == MAPTILE_WALL2) && IntersectRect(&temp, &botM.rc, &_player.rc))
	{
		_player.pointy -= temp.bottom - temp.top;
		_player.yspeed = 0;
	}
	_player.rc = RectMakeCenter(_player.pointx, _player.pointy, 24, 24);
}
