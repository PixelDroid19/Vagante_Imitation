#pragma once
#define TILESIZE 32
#define STATUSEFFECTMAX 5
struct tagStat {
	int hp;		// health
	int str;	// strength
	int dex;	// dexterity
	int vit;	// vitality
	int inl;	// intelligence
	int lck;	// luck
	int def;	// defense
	int fir;	// fire resistance
	int ice;	// ice resistance
	int lgt;	// lightning resistance
	int psn;	// poison resistance
	int mel;	// melee attack damage
	int rng;	// ranged attack damage
	int crit;	// critical hit chance
	int aspd;	// attack speed
	int spd;	// movement speed
};

// Status Effect Enum
enum STATUSEFFECT {
	STATUSEFFECT_NULL,		// No status effect
	STATUSEFFECT_POISON,	// Status effect: Poison
	STATUSEFFECT_FIRE,		// Status effect: Fire
	STATUSEFFECT_STUN,		// Status effect: Stun
	STATUSEFFECT_HEAL,		// Status effect: Heal
};
// Status Effect Struct
struct tagStatusEffect {
	// Status effect type
	STATUSEFFECT type;
	// Remaining effect time
	float leftTime;
	// Status effect damage (heal amount if healing)
	int damage;
};
enum ITEMSTATE {
	ITEMSTATE_ONMAP,		// On map
	ITEMSTATE_INVENTORY,	// In inventory
	ITEMSTATE_EQUIP,		// Equipped
	ITEMSTATE_NONEXSIST		// Non-existent
};
struct tagItemInfo {
	image* bigImg;			// Item image (large)
	image* smallImg;		// Item image (small)
	POINT point;			// Item position (when on map)
	RECT rc;				// Item rect (collision when on map)
	tagStat itemstat;		// Item stat value
	int itemstate;			// Check item state (on map, inventory, equipped)
};

enum ENEMYSTATE {
	ENEMYSTATE_IDLE,
	ENEMYSTATE_MOVING,
	ENEMYSTATE_ATTACKING,
	ENEMYSTATE_HIT,
	ENEMYSTATE_JUMPING,
	ENEMYSTATE_FALLING,
	ENEMYSTATE_DEAD
};
enum MAPTILETYPE {
	MAPTILE_NULL,
	MAPTILE_WALL,
	MAPTILE_WALL2,
	MAPTILE_LADDER,
	MAPTILE_GROUND_CAN_GO_DOWN_1,
	MAPTILE_SPIKE_TRAP
};
struct mapInfo {
	// Tile type
	int type;
	// Tile position
	POINT point;
	// Tile rect
	RECT rc;
};
enum PLAYERSTATE {
	PLAYERSTATE_IDLE,				// Normal idle
	PLAYERSTATE_MOVING,				// Moving
	PLAYERSTATE_LOOKING_DOWN,		// Looking down while on ladder
	PLAYERSTATE_LOOKING_DOWN_MOVING,// Looking down while moving
	PLAYERSTATE_JUMPING,			// Jumping
	PLAYERSTATE_FALLING,			// Falling
	PLAYERSTATE_ATTACKING,			// Normal attack
	PLAYERSTATE_ATTACKING_JUMP,		// Jump attack
	PLAYERSTATE_ATTACKING_3,		// Attack 3

	PLAYERSTATE_HIT,				// Was hit
	PLAYERSTATE_HOLDING_WALL,		// Grabbing wall
	PLAYERSTATE_HOLDING_LADDERUP,		// Climbing ladder up
	PLAYERSTATE_HOLDING_LADDERDOWN,		// Climbing ladder down
	PLAYERSTATE_CHEKINGINVENTORY,	// Checking inventory

	PLAYERSTATE_DEAD
};
struct tagPlayerInfo {
	image* img;
	PLAYERSTATE state;
	tagStatusEffect statusEffect[5];	// Status effects
	tagStat stat;						// Stats
	float pointx, pointy;				// Player position
	float xspeed;						// Left-right movement speed
	float yspeed;						// Up-down movement speed
	bool lookingRight;					// Looking right
	bool lookingLeft;					// Looking left
	float jumppower;					// Jump power
	RECT rc;							// Rect~!
	int money;							// Money
	float gravity;						// Gravity
	int currentFrameX, curretFrameY;	// Current frame
	
};
