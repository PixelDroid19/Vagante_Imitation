#pragma once
#include "gameNode.h"
#include <vector>
#include "vaganteStructEnum.h"

class Player;
class Map;
class UI;
struct tagFireBall
{

	RECT rc;						// Projectile collision box
	float x, y;						// Projectile position
	float angle;					// Projectile angle
	float radius;					// Radius
	float speed;					// Speed
	float fireX, fireY;				// Projectile launch position
	bool fire;						// Is fired?
	int currentFrameX, currentFrameY;
	int count;
	float power;
	int type;
};

// Projectile used by enemies
class FireBall : public gameNode
{
private:
	vector<tagFireBall> _vFireBall;
	vector<tagFireBall>::iterator _viFireBall;

	image* _fireBallImage;			// Projectile image
	const char* _imageName;
	float _range;
	int _FireBallMax;
	float _timerForFrameUpdate;
	Player* _player;
	Map* _map;
	UI* _ui;

public:
	HRESULT init(int FireBallMax, float range, const char* imageName);
	void release(void);
	void update(void);
	void render(POINT camera);
	void frameHandle();

	void fire(float x, float y, float angle, float speed);

	void move(void);
	void hitPlayer();
	void removeFireBall(int arrNum);
	vector<tagFireBall> getVFireBall(void) { return _vFireBall; }
	vector<tagFireBall>::iterator getVIFireBall(void) { return _viFireBall; }
	void setMapAddressLink(Map* map) { _map = map; }
	void setPlayerAddressLink(Player* player) { _player = player; }
	void setUIAddressLink(UI* ui) { _ui = ui; }

	// For A* algorithm
	FireBall();
	~FireBall();
};
