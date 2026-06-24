#pragma once
#include "gameNode.h"
#include "Enemy.h"
#include "manEater.h"
#include "Boss.h"
#include "worm.h"
#include "bat.h"
#include "slime.h"
#include "goblin.h"
#include "vaganteStructEnum.h"

class Player;
class Map;
class UI;
class EnemyManager : public gameNode
{
private:
	vector<Enemy*> _vEnemy;
	vector<Enemy*>::iterator _viEnemy;


	Player* _player;
	Boss* _boss;
	Map* _map;
	UI* _ui;

public:
	HRESULT init();
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);
	void drawDebug(POINT camera);

	void deadEnemyCheck();

	vector<Enemy*> getEnemyVector() { return _vEnemy; }
	Boss* getBoss() { return _boss; }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setMapAddressLink(Map* map) { _map = map; }
	void setUiAddressLink(UI* ui) { _ui = ui; }

	EnemyManager();
	~EnemyManager();
};

