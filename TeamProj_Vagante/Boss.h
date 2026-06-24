#pragma once
#include "gameNode.h"
#include <list>
#include <algorithm>
#include "Map.h"
#include "vaganteStructEnum.h"
#include "FireBall.h"
#include "UI.h"

/*
!You can check the struct in vaganteStructEnum.h!
enum BossSTATE {
BossSTATE_IDLE,
BossSTATE_MOVING,
BossSTATE_ATTACKING,
BossSTATE_HIT,
BossSTATE_JUMPING,
BossSTATE_FALLING,
BossSTATE_DEAD
};

struct tagStat {
int hp;		//HP
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
int crit;	//Critical chance
int aspd;	//Attack speed
int spd;	//Movement speed
};
*/



enum BOSSSTATE {
	BOSSSTATE_SLEEP,
	BOSSSTATE_ACTIVATE,
	BOSSSTATE_FLYING,
	BOSSSTATE_FIREING,
	BOSSSTATE_STAMPING,
	BOSSSTATE_DEAD
};

//For A* algorithm!!
typedef struct vertex {
	int f;		//Total cost
	int h;		//Movement cost
	int g;		// Movement distance calculation
	int vx, vy;	// Position
	int px, py;	// Parent position
	//vertex *p;	// Parent vertex
	bool operator<(const vertex &v) const {
		return (f > v.f);
	}
};
class Player;
class UI;
class Boss : public gameNode
{
private:
	image* _image;														// Image
	BOSSSTATE _state;													// State
	Player* _player;													// Player link
	UI* _ui;															//ui
	tagStatusEffect _statusEffect[5];									// Status effect
	tagStat _statistics;												// Stats
	int _currentFrameX, _currentFrameY;									// Current frame
	RECT _rc;															// Movement collision rect
	RECT _rcHit;														// Attack collision rect (add by JW)
	float _pointx, _pointy;												// Coordinates
	float _xspeed, _yspeed;												// x,y movement speed
	int _money;															// Gold dropped when killed
	Map* _map;															// Map link
	bool _isFindPlayer;													// Whether player was found
	bool _stampHitLand;													// Bool to check stamp hit land	
	bool _totallydead;													// Bool for confirming complete death
	int _fireballCount;													// Fireball count
	FireBall* _fireball;
	mapInfo upL, upM, upR, midL, midM, midR, botL, botM, botR;			// Position info
	bool _canfire;
	bool _lookleft;
	float _actTimer;													// Action timer
	int _frameTime, _frameFPS;											// Frame change amount
	float _minCog, _maxCog;												// Minimum cognition range, maximum cognition range
	float _timerForFrameUpdate;
	float _timerForAstar;
	// For A* algorithm!!
	int _curTileX, _curTileY;		// Current position
	int _goalTileX, _goalTileY;		// Goal position
	vertex _startpoint;			// Start point
	vertex* _currentvertex;			// Current search position
	vertex _npcurrentvertex;			// Next search position
	vector<vertex> _wayToPlayer;		// Path list to player
	int _tileinfo[40][58];			// Tile info
	// Open list, close list
	vector<vertex> _openlist;
	vector<vertex> _closelist;
	void add_openlist(vertex v);					// Add to open list
	void add_closelist(vertex v);					// Add to close list
	vertex getcloselist(int x, int y);				// Get from close list
	vertex getopenlist(int x, int y);				// Get from open list
	vertex pop_openlist();							// Pop minimum from open list
	vertex pop_closelist();						// Pop from close list
	vertex pop_closelist(int vx, int vy);			// Return vertex at matching coordinates
	vertex calc_vertex(vertex v, vertex p);					// Calculate vertex value
	void add_opelistEightWay(vertex* v);	// Add surrounding 8 tiles to open list
	bool checkGoal();
	void makeWay();
	void astar();

public:
	HRESULT init();
	virtual HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);
	void drawDebug(POINT camera);

	void stateHandle();
	void speedAdjust();
	void fireFireBall();
	void stamping();
	void imageChange();
	void deadCheck();

	virtual void move();			// Movement function
	void mapCollisionHandle();
	virtual void frameUpdate();		// Frame update


									// When damaged (without knockback)
	void getDamaged(int damage) { _statistics.hp -= damage; SOUNDMANAGER->play("5_Enemy_Demage_Sound", 1);}
	// When damaged (knockback & angle)
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _ui->hitOutput(_pointx, _pointy, damage, LETTER_WHITE);  SOUNDMANAGER->play("5_Enemy_Demage_Sound", 1); }
	// Status effect
	void addStatusEffect(tagStatusEffect statuseffect);


	// Getter & setter
	int getHP() { return _statistics.hp; }
	void setHP(int hp) { _statistics.hp = hp; }
	tagStat getStat() { return _statistics; }
	void setStat(tagStat stat) { _statistics = stat; }
	RECT getRect() { return _rcHit; }
	POINT getPoint() { return PointMake(_pointx, _pointy); }
	void setPoint(POINT point) { _pointx = point.x; _pointy = point.y; }
	int getMoney() { return _money; }
	float getXSpeed() { return _xspeed; }
	void setXSpeed(float xspeed) { _xspeed = xspeed; }
	float getYSpeed() { return _yspeed; }
	void setYSpeed(float yspeed) { _yspeed = yspeed; }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setUiAddressLink(UI* ui) { _ui = ui; }
	void setMapAddressLink(Map* map) { _map = map; }


	void statusEffect();

	Boss();
	~Boss();
};

