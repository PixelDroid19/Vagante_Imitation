#pragma once
#include "Enemy.h"
#include "UI.h"
class worm :
	public Enemy
{
private:
	image* _moveLeft;
	image* _moveRight;
	image* _moveUp;
	image* _moveDown;
	image* _hit;

	bool _isLeft;
	int _whereIsWorm;
	float _moveSpeed;
	bool _reverseFrame;

public:
	//Initial position, minimum perception range, maximum perception range
	HRESULT init(POINT point, float minCog, float maxCog);
	void falling();			// Falling handling
	void rectResize();		// In case rect resize is needed for any reason
	void update();
	void render(POINT camera);
	void draw(POINT camera);
	void move();			// Movement function
	void attack();			// Attack
	void frameUpdate();	// Frame update
	void playerCog() {	_isFindPlayer = true;}	// In immediate player perception, set to true unconditionally

	//Since damage and death are processed in the base class, handle it here
	void getDamaged(int damage) { 
		_statistics.hp -= damage; _ui->hitOutput(_pointx, _pointy, damage, LETTER_RED);
		SOUNDMANAGER->play("5_Enemy_Demage_Sound", 1);
	}
	void getDamaged(int damage, float angle, float knockbackpower) 
	{ 
		_statistics.hp -= damage; _xspeed += knockbackpower; _yspeed += knockbackpower; _angle = angle; _gravity = 0; _state = ENEMYSTATE_HIT; 
		_ui->hitOutput(_pointx, _pointy, damage, LETTER_RED);	
		SOUNDMANAGER->play("5_Enemy_Demage_Sound", 1);
	}

	void isThereWall();									//Check if there's a wall to change direction?
	void isDig();										//Check if stuck in ground (dig)?

	worm();
	~worm();
};
