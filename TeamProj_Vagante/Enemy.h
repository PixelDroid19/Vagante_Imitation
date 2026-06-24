#pragma once
#include "gameNode.h"
#include "Map.h"
#include "vaganteStructEnum.h"
/*
!You can check the detailed values in vaganteStructEnum.h!
enum ENEMYSTATE {
ENEMYSTATE_IDLE,
ENEMYSTATE_MOVING,
ENEMYSTATE_ATTACKING,
ENEMYSTATE_HIT,
ENEMYSTATE_JUMPING,
ENEMYSTATE_FALLING,
ENEMYSTATE_DEAD
};

struct tagStat {
int hp;		//Health
int str;	//Strength
int dex;	//Dexterity
int vit;	//Vitality
int inl;	//Intelligence
int lck;	//Luck
int def;	//Defense
int fir;	//Fire resistance
int ice;	//Ice resistance
int lgt;	//Lightning resistance
int psn;	//Poison resistance
int mel;	//Melee attack damage
int rng;	//Ranged attack damage
int crit;	//Critical hit rate
int aspd;	//Attack speed
int spd;	//Movement speed
};
*/

class Player;
class UI;
class Enemy : public gameNode
{
protected:
	image* _image;														//Image
	ENEMYSTATE _state;													//State
	Player* _player;													//Player pointer
	UI* _ui;															//UI
	Map* _map;															//Map recognition pointer
	tagStatusEffect _statusEffect[5];									//Status effects
	tagStat _statistics;												//Stats
	int _currentFrameX, _currentFrameY;									//Current frame
	RECT _rc;															//Collision rect
	float _pointx, _pointy;												//Coordinates
	float _xspeed, _yspeed, _angle, _gravity;							//Knockback x,y movement speed, angle, gravity
	float _minCog, _maxCog;												//Minimum recognition range, maximum recognition range
	int _money;															//Gold the enemy drops when defeated
	mapInfo _upL, _upM, _upR, _midL, _midM, _midR, _botL, _botM, _botR;	//9 tiles around the coordinate
	bool _isFindPlayer;													//Whether player was found
	int _frameTime, _frameFPS;											//Frame change timer
	POINT _lastPlayerPoint;
	bool _dead;															//Death check
	int _deadAlpha;														//Death alpha
	RECT _attackRect;													//Attack rect for player collision
	bool _deleteForeEm;													//If true, delete from EM

public:
	virtual HRESULT init();
	virtual HRESULT init(POINT point, float minCog, float maxCog);
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void render(POINT camera);
	virtual void draw(POINT camera);
	virtual void drawDebug(POINT camera);

	virtual void move();			// Movement function
	virtual void jump();			// Jump
	virtual void attack();			// Attack
	virtual void frameUpdate() {}	// Frame update
	virtual void falling();			// Fall handling
	virtual void rectResize();		// In case rect size adjustment is needed
	virtual void playerCog();		// Determine if player is in line of sight
	//When hit (knockback)
	virtual void getDamaged(int damage) { _statistics.hp -= damage; }
	//When hit (knockback angle knockback)
	virtual void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; _angle = angle; _gravity = 0; _state = ENEMYSTATE_HIT; }

	//Status effects
	virtual void addStatusEffect(tagStatusEffect statuseffect);

	//Getter&Setter
	//Health
	int getHP() { return _statistics.hp; }
	void setHP(int hp) { _statistics.hp = hp; }
	//Status value
	tagStat getStat() { return _statistics; }
	//Set status value
	void setStat(tagStat stat) { _statistics = stat; }
	//Collision rect
	RECT getRect() { return _rc; }
	//Enemy current position
	POINT getPoint() { return PointMake(_pointx, _pointy); }
	//Set enemy position
	void setPoint(POINT point) { _pointx = point.x; _pointy = point.y; }
	//Gold to drop
	int getMoney() { return _money; }
	//Knockback speed
	float getXSpeed() { return _xspeed; }
	void setXSpeed(float xspeed) { _xspeed = xspeed; }
	float getYSpeed() { return _yspeed; }
	void setYSpeed(float yspeed) { _yspeed = yspeed; }
	//Set nearby tile info
	void setTileInfo(mapInfo ul, mapInfo um, mapInfo ur, mapInfo ml, mapInfo mm, mapInfo mr, mapInfo bl, mapInfo bm, mapInfo br) { _upL = ul; _upM = um; _upR = ur; _midL = ml; _midM = mm; _midR = mr; _botL = bl; _botM = bm; _botR = br; }
	//Set map object tile info
	void setMap(Map* map) { _map = map; }
	//Whether dead and deleted
	bool getdead() { return _dead; }
	//Set state
	void setState(ENEMYSTATE es) { _state = es; }
	//Clear attack rect
	virtual void attRectClear() { _attackRect = RectMake(_pointx, _pointy, 1, 1); }
	//Apply status effect
	virtual void statusEffect();

	ENEMYSTATE getState() { return _state; }

	BYTE getSpriteAlpha() const
	{
		if (_state == ENEMYSTATE_DEAD)
			return (BYTE)(_deadAlpha > 255 ? 255 : _deadAlpha);
		return 255;
	}

	//Link player and UI
	void setPlayerAddressLink(Player* player) { _player = player; }
	void setUiAddressLink(UI* ui) { _ui = ui; }

	Enemy();
	~Enemy();
};
