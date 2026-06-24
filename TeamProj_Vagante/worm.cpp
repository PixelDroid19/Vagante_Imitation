#include "stdafx.h"
#include "worm.h"
#include "Player.h"

worm::worm()
{
}


worm::~worm()
{
}
/*
struct tagStat {
	int hp;		//HP
	int str;	//Strength
	int dex;	//Dexterity
	int vit;	//Vitality
	int inl;	//Intelligence
	int lck;	//Luck
	int def;	//Defense
	int fir;	//Fire Resistance
	int ice;	//Ice Resistance
	int lgt;	//Lightning Resistance
	int psn;	//Poison Resistance
	int mel;	//Melee Damage
	int rng;	//Ranged Damage
	int crit;	//Critical Rate
	int aspd;	//Attack Speed
	int spd;	//Move Speed
};
*/

HRESULT worm::init(POINT point, float minCog, float maxCog)
{
	//_image = IMAGEMANAGER->findImage("wormMoveUp");
	//Initialize the crawler object
	//Recognition, Aggro
	_minCog = minCog;
	_maxCog = maxCog;
	
	//Set initial position
	_pointx = point.x;
	_pointy = point.y;
	
	//Frame-related frame rate, frame time, current frame
	_frameFPS = 10;
	_frameTime = 0;
	_currentFrameX = _currentFrameY = 0;
	//Falling power
	_xspeed = _yspeed = _angle = _gravity = 0;
	//Coin money
	_money = RND->getFromIntTo(5, 1);

	//When adding images, because the resolution was too large, display at *2
	_moveLeft = new image;
	_moveLeft->init("Img\\enemy\\crawler_move_left.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	_moveRight = new image;
	_moveRight->init("Img\\enemy\\crawler_move_right.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	_moveUp = new image;
	_moveUp->init("Img\\enemy\\crawler_move_up.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	_moveDown = new image;
	_moveDown->init("Img\\enemy\\crawler_move_down.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	_hit = new image;
	_hit->init("Img\\enemy\\crawler_dead.bmp", 0, 0, 16 * 2, 24 * 2, 1, 2, true, RGB(255, 0, 255));

	//Initial 
	_image = _moveUp;

	/*
	IMAGEMANAGER->addFrameImage("wormMoveUp", "Img\\enemy\\crawler_move_up.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveDown", "Img\\enemy\\crawler_move_down.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveLeft", "Img\\enemy\\crawler_move_left.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveRight", "Img\\enemy\\crawler_move_right.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	*/
	//Initially, it does not detect the player
	//Since the crawler is in non-recognition state, immediately change it to true
	_isFindPlayer = false;
	//_statusEffect[5]
	//Status effect initialization x
	for (int i = 0; i < 5; i++)
	{
		_statusEffect[i].damage = 0;
		_statusEffect[i].leftTime = 0;
		_statusEffect[i].type = STATUSEFFECT_NULL;
	}
	//Statistics initialization
	memset(&_statistics, 0, sizeof(tagStat));
	//State initialization
	_state = ENEMYSTATE_IDLE;
	//Direction decision
	int a = RND->getInt(2);
	//Randomly set direction to avoid always going the same way
	if (a == 0) _isLeft = true;
	else _isLeft = false;
	//_isLeft = false;
	//When changing direction, it comes back, so reverse it
	_reverseFrame = false;
	//Move speed
	_moveSpeed = 0.5;
	//Rect, Attack rect
	//Crawler doesn't attack by default
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	_attackRect = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	//Set stats for test purposes
	_statistics.hp = 10;
	_statistics.str = 2;
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

	//Check which wall the crawler is on
	//0 = ground, 1 = right wall, 2 = ceiling, 3 = left wall
	_whereIsWorm = 0;


	//Check death
	_dead = false;
	_deadAlpha = 0;
	
	return S_OK;
}
void worm::update()
{

	//Function to enter attack rect, since it doesn't attack normally, process as is
	//If you need it for future convenience, use it later
	attRectClear();
	//Status effect handling
	statusEffect();
	//Falling handling
	falling();

	//When dying, state change
	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		//When completely faded out, set dead to true so the manager can remove it
		if (_deadAlpha >= 255)
		{
			_dead = true;
		}
	}

	
	if (_isFindPlayer && _state != ENEMYSTATE_DEAD)
	{
		//Crawler is different from a character, so recognize and process
		_isFindPlayer = true;
		move();
		jump();
		attack();

		//If distance exceeds maxCog beyond the perception range, it won't track
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > _maxCog)
			_isFindPlayer = false;

	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//Due to framework issues, replaced _PlayerPoint with _Player->getPoint()~~//
		///////////////////////////////////////////////////////////////////////////////////////

		//In non-perception state, if distance between worm and player is within basic perception range
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < _minCog)
		{
			if (static_cast<int>(_pointx / TILESIZE) == static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) == static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//If worm and player are on the same tile, perform hidden perception
				_isFindPlayer = true;
			}
			else
			{
				playerCog();
			}
		}
	}
	//Frame update and rect resize are processed separately
	frameUpdate();
	rectResize();
}
void worm::render(POINT camera) {
	draw(camera);
}
void worm::draw(POINT camera) {
	//Rectangle(getMemDC(), _pointx - _minCog / 2 + camera.x, _pointy - _minCog / 2 + camera.y, _pointx + _minCog / 2 + camera.x, _pointy + _minCog / 2 + camera.y);
	//Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	if (_state != ENEMYSTATE_DEAD)
		_image->frameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y);
	else
		_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, getSpriteAlpha());
	//EllipseMakeCenter(getMemDC(), _pointx + camera.x, _pointy + camera.y, 5, 5);



	/*
	char string[128];
	sprintf(string, "x : %d, y : %d", _rc.left / TILESIZE, _rc.top / TILESIZE);
	TextOut(getMemDC(), WINSIZEX / 2, WINSIZEY / 2, string, strlen(string));
	if (_isFindPlayer)
	{
	sprintf(string, "Cognizance");
	TextOut(getMemDC(), 300, 300, string, strlen(string));
	}
	else
	{
	sprintf(string, "Non-cognizance");
	TextOut(getMemDC(), 300, 300, string, strlen(string));
	}
	*/
}
void worm::move()
{
	//Movement handling in normal state, also judgment
	if (_state == ENEMYSTATE_IDLE || _state == ENEMYSTATE_MOVING)
	{
		//Since worm is forced to track once it finds the player, change all to moving
		_state = ENEMYSTATE_MOVING;
		
		//Check if stuck in ground or floating in air
		isDig();
		
		//Determines which way the worm is crawling
		//0 = ground (image wormMoveUp), 1 = right wall (image wormMoveRight), 2 = ceiling (image wormMoveDown), 3 = left wall (image wormMoveLeft)
		isThereWall();
		
		switch (_whereIsWorm)
		{
		case 0:
			if (_isLeft)
				_pointx -= _moveSpeed;
			else
				_pointx += _moveSpeed;
			break;
		case 1:
			if (_isLeft)
				_pointy -= _moveSpeed;
			else
				_pointy += _moveSpeed;
			break;
		case 2:
			if (_isLeft)
				_pointx += _moveSpeed;
			else
				_pointx -= _moveSpeed;
			break;
		case 3:
			if (_isLeft)
				_pointy += _moveSpeed;
			else
				_pointy -= _moveSpeed;
			break;
		}
	}
}
void worm::attack()
{
	//If state is not idle or moving, attack rect is not created
	if (_state == ENEMYSTATE_IDLE || _state == ENEMYSTATE_MOVING)
		_attackRect = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	else
		_attackRect = RectMakeCenter(_pointx, _pointy, 0, 0);
	//Collision check with player
	RECT temp;
	RECT playerRect = _player->getRect();
	if (IntersectRect(&temp, &playerRect, &_attackRect))
	{
		_player->getDamaged(_statistics.str, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 1);
		tagStatusEffect tse; 
		tse.type = STATUSEFFECT_POISON;
		tse.damage = 1;
		tse.leftTime = 5;
		_player->addStatusEffect(tse);
	}
}
void worm::frameUpdate()
{
	//It's fine to adjust _frameFPS as needed if you don't understand
	_frameFPS = 10;
	//When moving, set image according to direction and play frame animation
	if (_state == ENEMYSTATE_MOVING)
	{
		//0 = crawling on ground, 1 = right wall, 2 = ceiling, 3 = left wall
		switch (_whereIsWorm)
		{
		case 0:
			//_image = IMAGEMANAGER->findImage("wormMoveUp");
			_image = _moveUp;
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 1:
			//_image = IMAGEMANAGER->findImage("wormMoveRight");
			_image = _moveRight;
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 2:
			//_image = IMAGEMANAGER->findImage("wormMoveDown");
			_image = _moveDown;
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 3:
			//_image = IMAGEMANAGER->findImage("wormMoveLeft");
			_image = _moveLeft;
			//Since it's left-side wall crawling, the direction is opposite
			if (_isLeft)
				_image->setFrameY(1);
			else _image->setFrameY(0);
			break;
		}
		//if (_isLeft)
		//	_image->setFrameY(0);
		//else _image->setFrameY(1);
		_frameTime++;
		if (_frameTime >= _frameFPS)
		{
			_frameTime = 0;
			if (_reverseFrame)
				_currentFrameX--;
			else
				_currentFrameX++;
			if (_currentFrameX > _image->getMaxFrameX())
			{
				_reverseFrame = true;
				_currentFrameX = _image->getMaxFrameX();
			}
			else if (_currentFrameX < 0)
			{
				_reverseFrame = false;
				_currentFrameX = 1;
			}
			_image->setFrameX(_currentFrameX);
		}
	}
	else
	{
		//When hit, display hit image as is
		_image = _hit;
		_currentFrameX = 0;
		_frameFPS = 0;
		_image->setFrameX(0);
		if (_isLeft)
			_image->setFrameY(0);
		else
			_image->setFrameY(1);
	}
	/*
	ENEMYSTATE_IDLE,
		ENEMYSTATE_MOVING,
		ENEMYSTATE_ATTACKING,
		ENEMYSTATE_HIT,
		ENEMYSTATE_JUMPING,
		ENEMYSTATE_FALLING,
		ENEMYSTATE_DEAD
		*/
}

void worm::falling()	
{
	if (_state == ENEMYSTATE_HIT || _state == ENEMYSTATE_DEAD)
	{
		//Check if the falling direction collides with a wall, and the wall's position
		//Is this supposed to be this difficult?
		//The crawler normally never gets knockbacked
		_gravity -= 0.1f;
		_yspeed += _gravity;

		if (_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed)/TILESIZE, (_pointx - cosf(_angle)*_xspeed)/TILESIZE).type == MAPTILE_WALL)
		{
			//If it's a wall, bounce off
			//Check the top direction
			if (_yspeed < 0 && _map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.y > _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.y)
			{
				//The falling direction tile's y coordinate is larger than current, so land on the ground
				_pointx = _pointx - cosf(_angle)*_xspeed;
				_pointy = _map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx) / TILESIZE).rc.top - _moveUp->getFrameHeight() / 2;
				if(_state == ENEMYSTATE_HIT)	_state = ENEMYSTATE_IDLE;
				_whereIsWorm = 0;
				_xspeed = 0;
				_yspeed = 0;
				_angle = 0;
				_gravity = 0;
			}
			else if (_yspeed < 0 && _map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).type == MAPTILE_SPIKE_TRAP)
			{
				//If trap is triggered while falling, unless already dead, take damage
				if(_state != ENEMYSTATE_DEAD)
				{
					getDamaged(999);
					_whereIsWorm = 0;
					_xspeed = 0;
					_yspeed = 0;
					_angle = 0;
					_gravity = 0;
				}
			}
			else if (_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.y < _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.y)
			{
				//The falling direction y is smaller, meaning hitting the ceiling -> reverse speed
				_yspeed = -_yspeed;
			}
			else if (_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.x < _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.x ||
				_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.x > _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.x)
			{
				//The falling direction tile's x is different, so it eventually hits it -> reverse speed
				_xspeed = -_xspeed;
			}
			//else 
		}
		else
		{
			_pointx -= cosf(_angle)*_xspeed;
			_pointy += -sinf(_angle)*_yspeed;
		}
	}
}

void worm::rectResize()	
{
	//Resize the rect, but why..
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}

void worm::isThereWall()
{
	//Determines which way the worm is crawling
	//0 = ground (image wormMoveUp), 1 = right wall (image wormMoveRight), 2 = ceiling (image wormMoveDown), 3 = left wall (image wormMoveLeft)
	//Check if there is a wall in front, or if there is no wall in front and no floor below
	//Please check if the existing tile rect was correctly referenced
	switch (_whereIsWorm)
	{
	case 0:
		if (_isLeft)
		{
			//Check if current tile boundary is crossed when image moves left
			if ((_pointx - _moveSpeed)/TILESIZE < (_pointx)/TILESIZE)
			{
				//When crossing the boundary, check for direction change
				//Check if there is a wall on the left
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//If a wall exists, climb up that wall and change direction
					//Change to 1 (wormMoveRight)
					_whereIsWorm = 1;

					//Adjust x, y positions
					//x = previous tile(x-1) rect right edge + image width/2, y = rect bottom edge - moveSpeed
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.bottom - _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//If there is no wall on the left and no floor below, change to hanging from the tile below
					//x stays same, check tile at y+1 (below self), then the tile right below self
					//If that tile is empty
					//3 (wormMoveLeft)
					_whereIsWorm = 3;
					//Adjust positions
					//x = ground tile(y+1) rect left edge - image width/2, y = ground tile rect top edge + moveSpeed
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
			}
		}
		else
		{
			//Check if current tile boundary is crossed when moving right, different from _isLeft true
			if ((_pointx + _moveSpeed) > (_pointx)/TILESIZE)
			{
				//Crossing tile boundary, prepare for direction change
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//If there is a wall on the right, climb that wall
					//3 (wormMoveLeft)
					_whereIsWorm = 3;
					//Adjust positions
					//x = next tile(x+1) rect left edge - image width/2, y = next tile rect bottom edge - moveSpeed
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.bottom - _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy)/TILESIZE + 1, (_pointx + _moveSpeed)/TILESIZE).type != MAPTILE_WALL)
				{
					//If no wall on the right and check below too
					//Change to 1 (wormMoveRight)
					_whereIsWorm = 1;
					//Adjust positions
					//x = ground tile(y+1) rect right edge + image width/2, y = ground tile rect top edge + moveSpeed
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
			}
		}
		break;
	case 1:
		
		if (_isLeft)
		{
			// Check tile boundary crossing, also check image Y direction
			if ((_pointy - _moveSpeed)/TILESIZE < (_pointy)/TILESIZE)
			{
				// Prepare for direction change at boundary
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					// Wall exists on the upper side, climb up
					// Ceiling type, so use 2 (wormMoveDown)
					_whereIsWorm = 2;
					// Position adjustment
					// x = previous tile(y-1) rect left edge + moveSpeed, y = previous row rect bottom + image height/2
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight()/2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					// No wall above, and wall below doesn't exist either, so fall to ground
					// Ground type, use 0 (wormMoveUp)
					_whereIsWorm = 0;
					// Position adjustment
					// x = ground(x-1) rect right edge - moveSpeed, y = ground rect top edge - image height/2
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
			}
		}
		else
		{
			// Check below the current tile first
			if ((_pointy + _moveSpeed) / TILESIZE > (_pointy)/TILESIZE)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					// Wall exists below, land on ground - type 0 (wormMoveUp)
					_whereIsWorm = 0;
					// Position adjustment
					// x = below(y+1) rect left edge + moveSpeed, y = that rect top - image height/2
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					// No wall below, and no wall to the left below, so ceiling
					// Type 2 (wormMoveDown)
					_whereIsWorm = 2;
					// Position adjustment
					// x = self's current tile(x-1) rect right edge - moveSpeed, y = self current tile rect bottom + image height/2
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
			}
		}
		
		break;
	case 2:
		// Default is type 0
		if (_isLeft)
		{
			// Check when moving to the right, the tile boundary is crossed, _isLeft true is different
			if ((_pointx + _moveSpeed) / TILESIZE > (_pointx) / TILESIZE)
			{
				// Prepare for direction transition at boundary
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					// Wall on the right, climb down the wall
					// Type 3 (wormMoveLeft)
					_whereIsWorm = 3;
					// Position adjustment
					// x = right neighbor(x+1) rect left - image width/2, y = current tile rect top + moveSpeed
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth()/2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					// Check right side and also check below
					// Change to type 1 (wormMoveRight)
					_whereIsWorm = 1;
					// Position adjustment
					// x = tile above self(y-1) rect right + image width/2, y = tile above self rect bottom - moveSpeed
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom - _moveSpeed;
				}
			}
		}
		else
		{
			// When moving left, check if the center tile boundary is crossed
			if ((_pointx - _moveSpeed) / TILESIZE < (_pointx) / TILESIZE)
			{
				// Prepare for boundary transition
				// Check if there is a wall on the left
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					// Wall exists, climb down that wall and change direction
					// Change to type 1 (wormMoveRight)
					_whereIsWorm = 1;

					// Adjust x, y position
					// x = left neighbor(x-1) rect right + image width/2, y = left neighbor rect top + moveSpeed
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top + _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					// No wall on the left, and nothing below either, so check self's below tile
					// x stays, y checks +1 below self's tile, then self's tile straight below
					// If that tile is empty
					// Type 3 (wormMoveLeft)
					_whereIsWorm = 3;
					// Position adjustment
					// x = above(y-1) rect left - image width/2, y = above rect bottom + moveSpeed
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + _moveSpeed;
				}
			}
		}
		break;
	case 3:
		if (_isLeft)
		{
			// Check below the current tile first
			if ((_pointy + _moveSpeed) / TILESIZE > (_pointy)/TILESIZE)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					// Wall below, land on ground type 0 (wormMoveUp)
					_whereIsWorm = 0;
					// Position adjustment
					// x = below(y+1) rect right - moveSpeed, y = that rect top - image height/2
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					// No wall below, and no wall to the right below, so ceiling
					// Type 2 (wormMoveDown)
					_whereIsWorm = 2;
					// Position adjustment
					// x = neighbor(x + 1) rect left + moveSpeed, y = from bottom + image height/2
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
			}
		}
		else
		{
			// Check tile boundary, also check image Y direction
			if ((_pointy - _moveSpeed)/TILESIZE < (_pointy)/TILESIZE)
			{
				// Direction change at boundary
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					// Wall above, climb up
					// Ceiling type, so use 2 (wormMoveDown)
					_whereIsWorm = 2;
					// Position adjustment
					// x = above(y-1) rect right - moveSpeed, y = from bottom + image height/2
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					// No wall above, and no wall below, so fall to ground
					// Ground type, use 0 (wormMoveUp)
					_whereIsWorm = 0;
					// Position adjustment
					// x = neighbor(x+1) rect left + moveSpeed, y = neighbor rect top - image height/2
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
			}
		}
		break;
	}
}

void worm::isDig()
{
	// Different handling depending on situation
	switch (_whereIsWorm)
	{
	case 0:
		// Check if stuck on ground, or floating in the air
		// Check the current tile (_moveUp)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			// If current tile is a wall, the worm is considered stuck, so place it above the tile
			// Just change x same, y above
			_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top - _moveUp->getFrameHeight() / 2;
		}
		else if (_map->getMapInfo((_pointy + _moveUp->getFrameHeight()/2 + 1)/TILESIZE, (_pointx)/TILESIZE).type != MAPTILE_WALL)
		{
			// If current tile is not a wall and the tile below is also not a wall, floating in air so correct position
			_pointy += 5;
		}
		break;
	case 1:
		// Check if stuck on right wall, this is for right-side crawling (_moveRight)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			// Adjust x
			_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.right + _moveRight->getFrameWidth() / 2;
		}
		else if (_map->getMapInfo((_pointy)/TILESIZE, (_pointx - _moveRight->getFrameWidth()/2 - 1)/TILESIZE).type != MAPTILE_WALL)
		{
			// Not touching wall, adjust x
			_pointx -= 5;
		}
		break;
	case 2:
		// Ceiling (_moveDown)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			// Adjust y, opposite of ground case
			_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + _moveDown->getFrameHeight() / 2;
		}
		else if (_map->getMapInfo((_pointy - _moveDown->getFrameHeight()/2 - 1)/TILESIZE, (_pointx)/TILESIZE).type != MAPTILE_WALL)
		{
			// Adjust y, opposite of ground case
			_pointy -= 5;
		}
		break;
	case 3:
		// Left wall (_moveLeft)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			// Adjust x, opposite of right wall
			_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.left - _moveLeft->getFrameWidth() / 2;
		}
		else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveLeft->getFrameWidth() / 2 + 1) / TILESIZE).type != MAPTILE_WALL)
		{
			// Adjust x, opposite of right wall
			_pointx += 5;
		}
		break;
	}
}