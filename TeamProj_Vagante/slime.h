#pragma once
#include "Enemy.h"
#include "UI.h"
enum SLIMESTATE {
	SLIMESTATE_IDLE,
	SLIMESTATE_MOVING,
	SLIMESTATE_JUMPING,
	SLIMESTATE_FALLING,
	SLIMESTATE_DEAD
};

class slime : public Enemy
{
private:
	bool _isOnTop;			// Check whether on ceiling
	SLIMESTATE _slimeState;	//
	int _hittimer;			//
	float _jumptimer;		// Jump timer
	float _timerForFrame;
	bool _isOnLand;			// Check if on ground
	mapInfo upL, upM, upR, midL, midM, midR, botL, botM, botR;			// 
public:
	// Spawn position, min detection range, max detection range
	HRESULT init(POINT point, float minCog, float maxCog);
	void update();				//	Update
	void render(POINT camera);
	void draw(POINT camera);
	void move();				// Movement function
	void actByState();			//
	void hitPlayer();			// Player hit processing
	void mapCollisionCheck();	// Wall and ground collision check and processing
	void deadcheck();
	void imgHandleByState();
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; _ui->hitOutput(_pointx, _pointy, damage, LETTER_WHITE); SOUNDMANAGER->play("5_Enemy_Demage_Sound", 1); }
	void frameUpdate();					// Frame update
	
	slime();
	~slime();
};

