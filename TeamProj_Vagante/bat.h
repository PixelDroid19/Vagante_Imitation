#pragma once
#include "Enemy.h"
enum BATSTATE {
	BATSTATE_SLEEP,
	BATSTATE_FLYING,
	BATSTATE_HIT
};
class bat : public Enemy
{
private:
	bool _isPlayerOnTarget;	//Player detection range
	bool _isOnTop;			//Check whether on ceiling
	BATSTATE _batstate;
	int _hittimer;
	float _timerForFrame;
public:
	//Spawn position, min detection range, max detection range
	HRESULT init(POINT point, float minCog, float maxCog);
	void update();				//	Update
	void move();				// Movement function
	void actByState();			//
	void hitPlayer();			//Player hit processing
	void mapCollisionCheck();	//Ceiling, wall collision judgment and processing
	void deadcheck();
	void imgHandleByState();
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; _batstate = BATSTATE_HIT; _hittimer = TIMEMANAGER->getWorldTime(); SOUNDMANAGER->play("5_Enemy_Demage_Sound", 1); }
	void frameUpdate();					// Frame update
	void render(POINT camera);
	void draw(POINT camera);

public:
	bat();
	~bat();
};

