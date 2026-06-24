#pragma once
#include "gameNode.h"
#include "vaganteStructEnum.h"
class EnemyManager;
class Player;
class UI;

#define COINBOXMAX 5
#define ITEMBOXMAX 2

/*
!Reference the detailed type definition in vaganteStructEnum.h!
enum MAPTILETYPE 
{
	MAPTILE_NULL,
	MAPTILE_WALL,
	MAPTILE_WALL2,
	MAPTILE_LADDER,
	MAPTILE_GROUND_CAN_GO_DOWN_1,
	MAPTILE_GROUND_CAN_GO_DOWN_2,
	MAPTILE_TRAP_SPIKE,
	MAPTILE_TRAP_ARROW,
	MAPTILE_TRAP_ROCK
};

struct mapInfo 
{
	//Tile type
	int type;
	//Tile position
	POINT point;
	//Tile rect
	RECT rc;
};
*/

struct tagObj
{
	image* Image;
	RECT rc;
	float X, Y;
	int FrameX, FrameY;
	// Box open state
	bool _openBox;  

	// Flag to check if the event has already occurred
	bool _eventChk;
};
struct tagMiniMap
{
	image* Image;
	RECT playerRc;
	float X, Y;
};

struct tagSight
{
	image* Image;
	float X, Y;
	float alpha;
};
class Map : public gameNode
{
private:
	image* _trapImg;
	image* _mapImg;
	image* _BgImg;
	image* _SightImg;
	image* _SightBackImg;
	tagObj itemBox[ITEMBOXMAX];
	tagObj coinBox[COINBOXMAX];
	tagObj Door;
	tagMiniMap miniMap;



	mapInfo _mapInfo[40][58];
	EnemyManager* _em;
	Player* _player;
	UI* _ui;

public:
	HRESULT init();
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);
	void drawDebug(POINT camera);
	void drawMinimap();
	void setTile();
	void setObject();
	
	tagObj getCoinBox(int i) { return coinBox[i]; }// Return coin box
	tagObj getitemBox(int i) { return itemBox[i]; }// Return item box
	void setCoinBox(int i, bool isOpen) { coinBox[i]._openBox = isOpen; }// Coin box state change
	void setItemBox(int i, bool isOpen) { itemBox[i]._openBox = isOpen; }// Item box state change

	mapInfo getMapInfo(int i, int j) { return _mapInfo[i][j]; }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setUiAddressLink(UI* ui) { _ui = ui; }

	Map();
	~Map();
};
