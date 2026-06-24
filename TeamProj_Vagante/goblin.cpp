#include "stdafx.h"
#include "goblin.h"
#include "Player.h"

goblin::goblin()
{
}

goblin::~goblin()
{
}

HRESULT goblin::init(POINT point, float minCog, float maxCog)
{
	_state = ENEMYSTATE_IDLE; // Initialize state
	_pointx = point.x;		  // Initialize point
	_pointy = point.y;		  // Initialize point
	_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE); // Initialize collision rect
	_money = RND->getFromIntTo(5, 1);

	_isFindPlayer = false;			 // Initial state: player not found
	_attack = false;			     // Initial state: not attacking

	idleImg = new image;			 // Initialize image 
	moveImg = new image;			 // Initialize image 
	attackImg = new image;			 // Initialize image 
	attackImgLeft = new image;		 // Initialize image 
	attackImgRight = new image;		 // Initialize image 
	jumpImg = new image;			 // Initialize image 
	hitImg = new image;				 // Initialize image 
	deadImg = new image;			 // Initialize image 

	idleImg->init("Img\\enemy\\goblin_idle.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));		// Default image
	moveImg->init("Img\\enemy\\goblin_moving.bmp", 256, 64, 8, 2, true, RGB(255, 0, 255));	// Moving image
	jumpImg->init("Img\\enemy\\goblin_jump.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));	// Jumping image

	attackImg->init("Img\\enemy\\goblin_attack.bmp", 256, 64, 4, 2, true, RGB(255, 0, 255));// Attack image
	hitImg->init("Img\\enemy\\goblin_hit.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));		// Hit image
	deadImg->init("Img\\enemy\\goblin_dead.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));		// Attack image

	_findRange = minCog;		// Player search range
	_findRangeMax = maxCog;		// Player search range

	_statistics.hp = 16; // Initialize HP
	//_statistics.str = 1;
	_xspeed = _yspeed = 0; // Initialize speed

	_image = idleImg;	// Initialize starting image
	_currentFrameX = 0;	// Initialize frame
	_currentFrameY = 0;	// Initialize frame
	_frameFPS = 10;		// Initialize frame interval
	_frameTime = 0;		// Initialize frame interval

	_dead = false;		// Initialize death state	

	_deadAlpha = 0;		// For decreasing alpha on death

	_isOnLand = false;	// Landing check
	_isLive = true;

	_jumptimer = TIMEMANAGER->getWorldTime();	// Initialize jump timer
	_gravity = 0;		// Initialize gravity

	return S_OK;
}

void goblin::release()
{
}

void goblin::update()
{
	if (getDistance(_pointx, _pointy, 
		_player->getPoint().x, _player->getPoint().y) < 300) _isFindPlayer = true; // If distance to player is less than 300, recognize as player
	else _isFindPlayer = false; // If 300 or more, cancel recognition

	// Speed decrease
	if (_xspeed > 0) _xspeed -= 0.1;
	else if (_xspeed < 0) _xspeed += 0.1;
	if (_yspeed > 0) _yspeed -= 0.1;
	else if (_yspeed < 0) _yspeed += 0.1;

	// Speed limit
	if (_xspeed > 2) _xspeed = 2;
	else if (_xspeed < -2) _xspeed = -2;

	mapCollisionCheck(); // Map check 
	attRectClear();	// Function to clear and initialize attack rect
	frameUpdate(); // Process frame update. Renders when executed.
	
	//if (KEYMANAGER->isOnceKeyDown('N')) // Receives test key input and draws 
	//{
	//	getDamaged(1);
	//	_image = hitImg;
	//}
	
	//if (PtInRect(&_findPlayerRect, _player->getPoint()) && _isLive == true); // When player is found and HP is above 0
	//{
	//	if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 30) attack(); // Attack if distance to player is less than 30
	//	else move(); // Otherwise move. // Move when distance to player is 10
	//	//jump();
	//}

	if (_isFindPlayer == true && _isLive == true) // When player is found and HP is above 0
	{
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 30) attack(); // Attack if distance to player is less than 30
		else move();
	}

	//if (_statistics.hp > 0) // When player is found and HP is above 0
	//{
	//	if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 30) attack(); // Attack if distance to player is less than 30
	//	else move();
	//}

	if (_statistics.hp <= 0)// Death check
	{
		_isLive = false;
		_image = deadImg; // Attack image
		_state = ENEMYSTATE_DEAD; // Change state to dead
	}

	if (_state == ENEMYSTATE_DEAD)// When dead
	{
		_isLive = false;

		_deadAlpha += 5; // Increase alpha value
		if (_deadAlpha > 255)
		{
			_dead = true; // If alpha is 255 or more, mark as dead and remove
		}
	}

	if (_state == ENEMYSTATE_HIT)
	{
		_image = hitImg;
		_state = ENEMYSTATE_IDLE;
	}
}

void goblin::render(POINT camera)
{
	draw(camera);
}

void goblin::draw(POINT camera)
{
	_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y,
		_currentFrameX, _currentFrameY, getSpriteAlpha());
}

void goblin::drawDebug(POINT camera)
{
	Enemy::drawDebug(camera);
	if (!isDebugLayerEnabled(DEBUG_LAYER_HITBOX))
		return;
	debugDrawRect(getMemDC(), _findPlayerRect, camera, DebugPalette::DetectLine);
}

void goblin::move()
{
	_pointx += _xspeed; // Apply speed and add to coordinates
	_pointy += _yspeed;	// Apply speed and add to coordinates

	// Gravity processing
	_yspeed += 0.3;

	_state = ENEMYSTATE_MOVING; // Set to moving state

	_image = moveImg; // Change image to moving

	if (_pointx / TILESIZE > _player->getPoint().x / TILESIZE) // If left of player, face left
	{
		_right = false; // Check if facing right
		//_xspeed -= _statistics.spd; // AI speed not applied here
		_xspeed -= 0.5; // Apply value to x speed
	}
	else if ((_pointx / TILESIZE < _player->getPoint().x / TILESIZE)) // If right of player, face right
	{
		_right = true; // Facing right
		//_xspeed += _statistics.spd;
		_xspeed += 0.5; // Apply value to speed

	} 
}

void goblin::jump()					
{
	_image = jumpImg; // Change image to jump

	if (TIMEMANAGER->getWorldTime() - _jumptimer > 1) // Jump timer
	{
		_yspeed -= 8; // Jump
		_state = ENEMYSTATE_JUMPING;// This state is jumping
		_jumptimer = TIMEMANAGER->getWorldTime();// Reset jump timer
	}

	if (_state == ENEMYSTATE_JUMPING || _state == ENEMYSTATE_FALLING)
	{
		if (_yspeed > 0) _state = ENEMYSTATE_FALLING; // If Yspeed > 0, falling
	
		if (_pointx > _player->getPoint().x) // If left of player, face left
		{
			_right = false; // Left flag?
			_xspeed -= 2;
			_currentFrameY = 0;
		}
		else // If right of player, face right
		{
			_right = true;
		//	_xspeed += _statistics.spd;
			_currentFrameY = 1;
		}
	}
	if (_isOnLand)  _state = ENEMYSTATE_IDLE;
}

void goblin::attack()
{

	_state = ENEMYSTATE_ATTACKING;

	_statistics.mel = RND->getFromIntTo(6, 7);

	if (_isFindPlayer && _statistics.hp >= 0)
	{
		_image = attackImg;

		if (_right == true && _player->getPoint().x <= _pointx + 32)
		{
			//_image = attackImgRight;
			_attackRect = RectMake(_pointx + 32, _pointy + 8, 32, 16);
		}
		
		if (_right == false && _player->getPoint().x >= _pointx - 32)
		{
			//_image = attackImgLeft;
			_attackRect = RectMake(_pointx, _pointy + 8, 32, 16);
		}
	}

	RECT temp;
	RECT playerRect = _player->getRect();
	if (IntersectRect(&temp, &playerRect, &_attackRect)) // Hit the player
	{
		_player->getDamaged(_statistics.mel, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 3);
		// Knock player back in opposite direction
		_xspeed = cosf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;
		_yspeed = -sinf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;
	}

}

void goblin::falling()
{

}

void goblin::frameUpdate()
{
	_frameFPS = 10;
	_frameTime++;
	if (_frameFPS <= _frameTime)
	{
		_currentFrameX++;
		_frameTime = 0;
	}

	if (_currentFrameX > _image->getMaxFrameX())
	{
		_currentFrameX = 0;
	}

	if (_right == true && _state != ENEMYSTATE_ATTACKING) _currentFrameY =  0;
	else if (_right == false && _state != ENEMYSTATE_ATTACKING) _currentFrameY = 1;
}	

void goblin::playerCog()
{

}

void goblin::mapCollisionCheck()
{
	_upL = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE - 1);
	_upM = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE);
	_upR = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE + 1);
	_midL = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE - 1);
	_midM = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE);
	_midR = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE + 1);
	_botL = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE - 1);
	_botM = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE);
	_botR = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE + 1);

	_rc = RectMakeCenter(_pointx, _pointy, TILESIZE, TILESIZE);
	RECT temp;
	if ((_midL.type == MAPTILE_WALL || _midL.type == MAPTILE_WALL2) && IntersectRect(&temp, &_midL.rc, &_rc))
	{
		_pointx += temp.right - temp.left;
		jump();
	}
	if ((_midR.type == MAPTILE_WALL || _midR.type == MAPTILE_WALL2) && IntersectRect(&temp, &_midR.rc, &_rc))
	{
		_pointx -= temp.right - temp.left;
		jump();
	}
	if ((_upM.type == MAPTILE_WALL || _upM.type == MAPTILE_WALL2) && IntersectRect(&temp, &_upM.rc, &_rc))
	{
		_pointy += temp.bottom - temp.top;
	}

	if (_player->getPoint().y >= _pointy)
	{
		if ((_botM.type == MAPTILE_WALL || _botM.type == MAPTILE_WALL2) && IntersectRect(&temp, &_botM.rc, &_rc))
		{
			_pointy -= temp.bottom - temp.top;
			_yspeed = 0;
			_isOnLand = false;
		}
	}
	else
	{
		if ((_botM.type == MAPTILE_WALL || _botM.type == MAPTILE_WALL2 || _botM.type == MAPTILE_GROUND_CAN_GO_DOWN_1) && IntersectRect(&temp, &_botM.rc, &_rc))
		{
			_pointy -= temp.bottom - temp.top;
			_yspeed = 0;
			_isOnLand = true;
		}
	}
	_rc = RectMakeCenter(_pointx, _pointy, TILESIZE, TILESIZE);

}