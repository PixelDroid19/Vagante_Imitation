#include "stdafx.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Map.h"
#include "UI.h"

EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

HRESULT EnemyManager::init()
{

	//?????? ???~
	Enemy* temp = new worm;
	POINT pt = PointMake(TILESIZE * 26, TILESIZE * 10);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 9999, 9999);
	_vEnemy.push_back(temp);
	temp = new worm;
	 pt = PointMake(TILESIZE * 34, TILESIZE * 16);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 9999, 9999);
	_vEnemy.push_back(temp);
	temp = new worm;
	 pt = PointMake(TILESIZE * 16, TILESIZE * 14);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 9999, 9999);
	_vEnemy.push_back(temp);
	temp = new worm;
	 pt = PointMake(TILESIZE * 22, TILESIZE * 32);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 9999, 9999);
	_vEnemy.push_back(temp);
	temp = new worm;
	 pt = PointMake(TILESIZE * 27, TILESIZE * 28);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 9999, 9999);
	_vEnemy.push_back(temp);
	temp = new worm;
	 pt = PointMake(TILESIZE * 36, TILESIZE * 28);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 9999, 9999);
	_vEnemy.push_back(temp);
	temp = new worm;
	 pt = PointMake(TILESIZE * 36, TILESIZE * 26);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 9999, 9999);
	_vEnemy.push_back(temp);
	temp = new worm;
	 pt = PointMake(TILESIZE * 40, TILESIZE * 25);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 9999, 9999);
	_vEnemy.push_back(temp);

	


	//???????~
	temp = new bat;
	 pt = PointMake(TILESIZE * 16, TILESIZE * 6);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 0, 0);
	_vEnemy.push_back(temp);
	temp = new bat;
	 pt = PointMake(TILESIZE * 35, TILESIZE * 6);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 0, 0);
	_vEnemy.push_back(temp);
	temp = new bat;
	 pt = PointMake(TILESIZE * 18, TILESIZE * 13);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 0, 0);
	_vEnemy.push_back(temp);
	temp = new bat;
	 pt = PointMake(TILESIZE * 27, TILESIZE * 16);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 0, 0);
	_vEnemy.push_back(temp);
	temp = new bat;
	 pt = PointMake(TILESIZE * 40, TILESIZE * 15);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 0, 0);
	_vEnemy.push_back(temp);
	temp = new bat;
	 pt = PointMake(TILESIZE * 15, TILESIZE * 22);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 0, 0);
	_vEnemy.push_back(temp);
	temp = new bat;
	 pt = PointMake(TILESIZE * 36, TILESIZE * 22);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 0, 0);
	_vEnemy.push_back(temp);
	temp = new bat;
	 pt = PointMake(TILESIZE * 40, TILESIZE * 32);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 0, 0);
	_vEnemy.push_back(temp);

	//?????? ???~
	temp = new goblin;
	pt = PointMake(TILESIZE * 23, TILESIZE * 10);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new goblin;
	pt = PointMake(TILESIZE * 12, TILESIZE * 17);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new goblin;
	pt = PointMake(TILESIZE * 22, TILESIZE * 23);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new goblin;
	pt = PointMake(TILESIZE * 26, TILESIZE * 34);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new goblin;
	pt = PointMake(TILESIZE * 32, TILESIZE * 26);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);

	//?????? ???
	temp = new manEater;
	pt = PointMake(TILESIZE * 17, TILESIZE * 5);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new manEater;
	pt = PointMake(TILESIZE * 23, TILESIZE * 16);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new manEater;
	pt = PointMake(TILESIZE * 19, TILESIZE * 23);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new manEater;
	pt = PointMake(TILESIZE * 15, TILESIZE * 28);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new manEater;
	pt = PointMake(TILESIZE * 36, TILESIZE * 14);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new manEater;
	pt = PointMake(TILESIZE * 41, TILESIZE * 25);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new manEater;
	pt = PointMake(TILESIZE * 35, TILESIZE * 28);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	//??????
	temp = new slime;
	pt = PointMake(TILESIZE * 16, TILESIZE * 32);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new slime;
	pt = PointMake(TILESIZE * 28, TILESIZE * 18);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new slime;
	pt = PointMake(TILESIZE * 37, TILESIZE * 33);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new slime;
	pt = PointMake(TILESIZE * 36, TILESIZE * 25);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	temp = new slime;
	pt = PointMake(TILESIZE * 38, TILESIZE * 10);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);
	

	_boss = new Boss();
	////????????? ???? address???
	_boss->setPlayerAddressLink(_player);
	_boss->setUiAddressLink(_ui);
	_boss->setMapAddressLink(_map);
	_boss->init(PointMake(TILESIZE * 35, TILESIZE * 34));
	return S_OK;
}

void EnemyManager::release()
{

}

void EnemyManager::update()
{

	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		//???????
		(*_viEnemy)->update();
	}

	//???? ??

	//???????
	_boss->update();

	//enemy?? ??????? ??
	deadEnemyCheck();
}
void EnemyManager::render()
{

}

//??? ??	x????? camera.x ???
//			y????? camera.y ??? ???????!!!!
void EnemyManager::render(POINT camera)
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		(*_viEnemy)->render(camera);
	}
	_boss->render(camera);

	draw(camera);
}
void EnemyManager::drawDebug(POINT camera)
{
	if (!isDebugDrawEnabled())
		return;

	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
		(*_viEnemy)->drawDebug(camera);
	_boss->drawDebug(camera);
}
void EnemyManager::draw(POINT camera)
{

}
void EnemyManager::deadEnemyCheck()
{
	for (int i = _vEnemy.size() - 1; i >= 0; i--)
	{
		if (_vEnemy[i]->getHP() <= 0 && !_vEnemy[i]->getdead())
		{
			if (_vEnemy[i]->getState() != ENEMYSTATE_DEAD)
			{
				for (int j = 0; j < _vEnemy[i]->getMoney(); j++)
				{
					_ui->addItemOnMap(NAME_COIN, PointMake(RND->getFromIntTo(_vEnemy[i]->getRect().left, _vEnemy[i]->getRect().right), RND->getFromIntTo(_vEnemy[i]->getRect().top, _vEnemy[i]->getRect().bottom)));
				}
			}
			_vEnemy[i]->setState(ENEMYSTATE_DEAD);
		}
		else if (_vEnemy[i]->getdead())
		{
			SAFE_DELETE(_vEnemy[i]);
			_vEnemy.erase(_vEnemy.begin() + i);
		}
	}
}