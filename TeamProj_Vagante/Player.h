#pragma once
#include "gameNode.h"
#include "vaganteStructEnum.h"
#include "UI.h"

class EnemyManager;
class Map;
class UI;
class Enemy;

#define JUMPPOWERSTART 5
#define JUMPPOWERMAX 10
#define FALLPOWERMAX 8
#define RUNPOWERSTART 1
#define RUNPOWERMAX 2.6
#define DASHPOWER 8
#define DASHTIME 2.5
#define LADDERUPSPEED 2
#define LADDERDOWNSPEED -3

/*
!You can check the detailed values in vaganteStructEnum.h!
enum PLAYERSTATE {
	PLAYERSTATE_ILDE,				//Normal state
	PLAYERSTATE_LOOKING_UP,			//Looking up with up arrow
	PLAYERSTATE_LOOKING_DOWN,		//Looking down with down arrow
	PLAYERSTATE_MOVING,				//Moving
	PLAYERSTATE_JUMPING,			//Jumping
	PLAYERSTATE_FALLING,			//Falling
	PLAYERSTATE_ATTACKING_1,		//Attack 1
	PLAYERSTATE_ATTACKING_2,		//Attack 2
	PLAYERSTATE_ATTACKING_3,		//Attack 3
	PLAYERSTATE_HIT,				//Hit state
	PLAYERSTATE_HOLDING_WALL,		//Holding wall
	PLAYERSTATE_HOLDING_LADDER,		//Holding ladder
	PLAYERSTATE_CHEKINGINVENTORY	//Checking inventory
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

struct tagPlayerInfo {
	image* image;
	PLAYERSTATE state;
	tagStatusEffect statusEffect[5];	//Status effects
	tagStat stat;						//Stats
	float pointx, pointy;				//Position coordinates
	float xspeed;						//X-axis horizontal speed
	float yspeed;						//Y-axis vertical speed
	bool lookingRight;					//Looking right
	bool lookingLeft;					//Looking left
	float jumppower;					//Jump power
	RECT rc;							//Rect
	int money;							//Money
	float gravity;						//Gravity
	int currentFrameX, curretFrameY;	//Current frame
};
*/
class Player : public gameNode
{

private:
	typedef vector<Enemy*> vEnemyRange;
	typedef vector<Enemy*>::iterator viEnemyRange;
	typedef vector<tagItem> vItem;
	typedef vector<tagItem>::iterator viItem;


	tagPlayerInfo _player;
	EnemyManager* _em;
	Map* _map;
	UI* _ui;
	bool _canCtrl;		//Determines whether input can be received
	

	mapInfo upL, upM, upR, midL, midM, midR, botL, botM, botR;// Position info
	int _curTileX, _curTileY; // Current tile position
	int _prevTileX, _prevTileY; // Previous tile position

	tagItem _equipWeapon;
	MYRECT _equipWeaponRect;  //Rect for drawing weapon
	float _attackDelay;
	
	vEnemyRange _vEnemyRange;	
	vItem _vItem;
	
	
	animation* _playerMotion[2];


	int _animSpeed;
	int _animCount;
	int _frameX;
	int _frameY;
	int _frameYOffset;
	int _animDelay;

	int _statusEffectTimer;


	int _test;

	bool _invincible;
	float _invincibleTime;

public:
	HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);
	void drawDebug(POINT camera);

	//Initial settings
	void firstSettingStat();


	void move();			// Movement function
	void keyintput();		// Key input function
	void setmaptileInfo();	// Get tile info
	void setStateImg(void); // Set image according to state	
	void frameUpdate();

	void jump();			// Jump
	void attack();			// Attack
	void attackjump();		// Attack while jumping
	void attackingNow();	// Currently attacking
	void holdLadder();		// Hold ladder
	void canDown();			// Check if can go down from ground

	void mapcollision();
	void attackCollision();

	void checkStatusEffect(); //Check status effects
	void checkHitEnemy(); //Check enemy hit
	void checkInvincible(); //Check invincibility
	void checkItemBox(); //Check item box position
	void checkDropItem(); //Check dropped items

	//When hit (knockback)
	void getDamaged(int damage);
	//When hit (knockback angle knockback)
	void getDamaged(int damage, float angle, float knockbackpower);
	//Add status effect
	void addStatusEffect(tagStatusEffect statuseffect);

	void enemyCollision(void);



	//Getter&Setter
	inline int getHP() { return _player.stat.hp; }
	inline void setHP(int hp) { _player.stat.hp = hp; }
	inline POINT getPoint() { return PointMake(int(_player.pointx), int(_player.pointy)); }
	inline void setPoint(POINT point) { _player.pointx = point.x; _player.pointy = point.y; }
	inline RECT getRect() { return _player.rc; }
	inline PLAYERSTATE getState() { return _player.state; }
	inline void setState(PLAYERSTATE state) { _player.state = state; }
	inline int getMoney() { return _player.money; }
	inline void setMoney(int money) { _player.money = money; }
	inline tagStat getStat() { return _player.stat; }
	inline void setStat(tagStat stat) { _player.stat = stat; }
	inline bool getCtrl() { return _canCtrl; }
	inline void setCtrl(bool ctrl) { _canCtrl = ctrl; }
	inline float getXSpeed() { return _player.xspeed; }
	inline void setXSpeed(float xspeed) { _player.xspeed = xspeed; }
	inline float getYSpeed() { return _player.yspeed; }
	inline void setYSpeed(float yspeed) { _player.yspeed = yspeed; }
	inline tagStatusEffect* getStatusEffect() { return &_player.statusEffect[0]; }
	inline void setEquipWeapon(tagItem weapon) { _equipWeapon = weapon; }


	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setMapAddressLink(Map* map) { _map = map; }
	void setUiAddressLink(UI* ui) { _ui = ui; }

	Player();
	~Player();
};
