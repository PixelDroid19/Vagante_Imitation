#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Map.h"

UI::UI()
{
}


UI::~UI()
{
}

HRESULT UI::init()
{
	//_player->init(PointMake(TILESIZE*(36 + 5), TILESIZE*(4 + 5)));


	
	
	_active = false;
	
	tagItemInfo _item[5];
	tagCoinInfo _coin[255];

	//========================  A D D   I M G ========================
	addImg();

	//================================================================
	_active = false;
	_menuNum = 0;
	_menuRect = RectMake(20, WINSIZEY - 88, 228, 280);
	_lvlRect = RectMake(_menuRect.left + 18, _menuRect.top + 162, 192, 84);
	_currentMoney = 0;
	_count = 0;
	_income = 0;
	_lvlNum = 0;
	_bagNum = 0;
	_skilNum = 0;
	_statNum = 0;
	_rankNum = 0;
	_move = false;
	_delay.coin = 0;
	_delay.menu = 0;
	_inputAlphaSource = 255;
	_save.position0 = 0;
	_save.position1 = 0;
	_lvlPoint = 1;
	_plusAlphaSource = 0;
	_inputGuide = 0;
	_maxHp = 50 + _player->getStat().vit * 10;
	_currentHp = _player->getStat().hp;

	//====================== H A N D ================================

	ZeroMemory(&_hand, sizeof(tagItem));
	_hand.equip = false;
	_hand.img = IMAGEMANAGER->findImage("hand");
	_hand.minDmg = 1;
	_hand.maxDmg = 1;
	_hand.name = NAME_HAND;
	_hand.type = TYPE_WEAPON;
	_player->setEquipWeapon(_hand);



	//================================================================

	setItemToBag(NAME_HEAL);
	setItemToBag(NAME_SWORD);
	setItemToBag(NAME_HEAL);
	setItemToBag(NAME_SWORD);
	setItemToBag(NAME_HEAL);
	setItemToBag(NAME_SWORD);
	setItemToBag(NAME_HEAL);
	setItemToBag(NAME_SWORD);
	setItemToBag(NAME_HEAL);
	setItemToBag(NAME_SWORD);
	
	addItemOnMap(NAME_SWORD, PointMake(TILESIZE*(36 ), TILESIZE*(4)));
	//addItemOnMap(NAME_COIN, PointMake(TILESIZE*(36 + 5), TILESIZE*(4 + 5)));
	//addItemOnMap(NAME_COIN, PointMake(TILESIZE*(36 + 6), TILESIZE*(4 + 5)));
	//addItemOnMap(NAME_COIN, PointMake(TILESIZE*(36 + 7), TILESIZE*(4 + 5)));
	//addItemOnMap(NAME_COIN, PointMake(TILESIZE*(36 + 8), TILESIZE*(4 + 5)));
	//addItemOnMap(NAME_COIN, PointMake(TILESIZE*(36 + 4), TILESIZE*(4 + 5)));
	//addItemOnMap(NAME_COIN, PointMake(TILESIZE*(36 + 3), TILESIZE*(4 + 5)));
	addItemOnMap(NAME_HEAL, PointMake(TILESIZE*(36 - 5), TILESIZE*(4 + 5)));

	SOUNDMANAGER->play("7_DarkCave_Music",0.5);
	return S_OK;
}
void UI::release()
{

}
void UI::update()
{
	//=========================== T I M E R ===========================
	_count++;
	if (_delay.coin) _delay.coin--;
	if (_delay.menu) _delay.menu--;

	//=================== P L A Y E R   U P D A T E ===================
	_currentHp = _player->getHP();
	_maxHp = 50 + _player->getStat().vit * 10;
	if (_currentHp > _maxHp) _player->setHP(_maxHp);

	//=================== M E N U   R E C T ===========================
	_lvlRect = RectMake(_menuRect.left + 18, _menuRect.top + 157, 192, 224);

	//================= F R A M E   C O N T R O L =====================
	repeatIndex("cursor_idle", 6);
	repeatIndex("cursor_move", 6);

	//================= I N C O M E   U P D A T E =====================
	if (!_delay.coin && _income > 0)
	{
		_income--;
		_currentMoney++;
	}

	//================== A L P H A   S O U R C E ======================
	if (!_delay.menu)
	{
		_inputAlphaSource -= 5;
		if (_inputAlphaSource < 0) _inputAlphaSource = 0;
	}
	else
	{
		_inputAlphaSource = 255;
	}

	if (_lvlPoint)
	{
		if (_count % 180 < 90 && _count % 6 == 0) 
		{ 
			_plusAlphaSource += 15; 
			if (_plusAlphaSource > 180) _plusAlphaSource = 180;
		}
		else if (_count % 180 >= 90 && _count % 6 == 0)
		{
			_plusAlphaSource -= 15;
			if (_plusAlphaSource < 0) _plusAlphaSource = 0;
		}
	}
	//====================== H I T   O U T P U T ======================
	for ( _viHit = _vHit.begin(); _viHit != _vHit.end(); ++_viHit)
	{
		_viHit->y--;
		_viHit->alphaSource += 7;
	}

	for (_viHit = _vHit.begin(); _viHit != _vHit.end(); )
	{
		if (_viHit->alphaSource >= 255)
		{
			_vHit.erase(_viHit);
			break;
		}
		else ++_viHit;
	}

	//===================== I T E M   O N   M A P =====================
	for ( _viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{

		mapInfo bot = _map->getMapInfo(
			((int)_viItem->point.y / TILESIZE)+1,
			(int)_viItem->point.x / TILESIZE);
		mapInfo cen = _map->getMapInfo(
			((int)_viItem->point.y / TILESIZE),
			(int)_viItem->point.x / TILESIZE);
		_viItem->point.y += 3;
		if (isCollision(bot.rc, _viItem->rc))
		{
			if (bot.type == MAPTILE_WALL || bot.type == MAPTILE_WALL2 || bot.type == MAPTILE_GROUND_CAN_GO_DOWN_1)
			{
				_viItem->point.y = bot.rc.top - _viItem->img0->getHeight() / 2;
			}
		}
		if (isCollision(cen.rc, _viItem->rc))
		{
			if (cen.type == MAPTILE_WALL || cen.type == MAPTILE_WALL2 || cen.type == MAPTILE_GROUND_CAN_GO_DOWN_1)
			{
				_viItem->point.y = cen.rc.top - _viItem->img0->getHeight() / 2;
			}
		}


		_viItem->rc = RectMakeCenter(_viItem->point.x, _viItem->point.y,
			_viItem->img0->getWidth(), _viItem->img0->getHeight());


	}
	for ( _viItem = _vItem.begin(); _viItem != _vItem.end(); )
	{
		if (((int)_viItem->point.y / TILESIZE) >= 39)
		{
			_vItem.erase(_viItem);
			break;
		}
		else ++_viItem;
	}

	//======================== F U N C T I O N ========================
	rectMove();
	collision();
	if (_active) keyControl();
}

void UI::render(){}
void UI::draw(){}
void UI::render(POINT camera)
{
	draw(camera);
	itemDraw(camera);
	if(_active) explanation();
	showStatus();

}
void UI::draw(POINT camera)
{
	IMAGEMANAGER->findImage("equip_slot")->render(getMemDC(), _menuRect.left + 52, _menuRect.top);
	for ( _viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
	{
		if (_viBag->equip)
		{
			if (_viBag->type == TYPE_WEAPON) _viBag->img->render(getMemDC(), _menuRect.left + 52, _menuRect.top);
			break;
		}
		if (_viBag == --_vBag.end() && !_viBag->equip) IMAGEMANAGER->findImage("hand")->frameRender(getMemDC(),
			_menuRect.left + 52, _menuRect.top, 0, 0);
		
	}
	IMAGEMANAGER->findImage("hpBarBottom")->render(getMemDC(), _menuRect.left + 2, _menuRect.top + 44);
	hpBottomNumberMacro(_menuRect.left + 90, _menuRect.top + 50,_currentHp);
	IMAGEMANAGER->findImage("hpBarTop")->render(getMemDC(), _menuRect.left +38, _menuRect.top + 50, 0,0,184* _currentHp / _maxHp, 14);
	hpTopNumberMacro(_menuRect.left + 90, _menuRect.top + 50, _currentHp);

	if (!_active)
		IMAGEMANAGER->findImage("selectOff")->render(getMemDC(), _menuRect.left + 2, _menuRect.top + 70);
	else
		IMAGEMANAGER->findImage("selectOn")->render(getMemDC(), _menuRect.left + 2, _menuRect.top + 70);
	switch (_menuNum)
	{
	case 0:
		IMAGEMANAGER->findImage("bag")->render(getMemDC(), _menuRect.left, _menuRect.top + 88);
		for (_viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
		{
			_viBag->img->render(getMemDC(), _menuRect.left + 5 + (_viBag->position % 6) * 36,
				_menuRect.top + 90 + (_viBag->position / 6) * 36);
			if (_viBag->equip == true)
				IMAGEMANAGER->findImage("equip_icon")->frameRender(getMemDC(), _menuRect.left + 32 + (_viBag->position % 6) * 36,
					_menuRect.top + 115 + (_viBag->position / 6) * 36, 1, 0);
		}

		if (!_move)IMAGEMANAGER->findImage("cursor_idle")->frameRender(getMemDC(),
			_menuRect.left - 13 + (_bagNum % 6) * 36, _menuRect.top + 113 + (_bagNum / 6) * 36);
		else
		{
			for ( _viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
			{
				if (_viBag->position == _save.position0)
				{
					_viBag->img->alphaRender(getMemDC(), _menuRect.left + (_bagNum % 6) * 36, _menuRect.top + 133 + (_bagNum / 6) * 36, 150);
				}

			}
			IMAGEMANAGER->findImage("cursor_move")->frameRender(getMemDC(),
				_menuRect.left - 13 + (_bagNum % 6) * 36, _menuRect.top + 113 + (_bagNum / 6) * 36);
		}

		if (!_delay.menu)
		{
			if (!_move)
			{
				switch (_inputGuide)
				{
				case 0:
					IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
						_menuRect.right + 10, _menuRect.top + 100,8,3,_inputAlphaSource);
					letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 110, "Next Tab",_inputAlphaSource);
					break;
				case 1:
					IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
						_menuRect.right + 10, _menuRect.top + 100, 3, 2, _inputAlphaSource);
					letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 110, "UnEquip", _inputAlphaSource);
					IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
						_menuRect.right + 10, _menuRect.top + 140, 5, 2, _inputAlphaSource);
					letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 150, "Move", _inputAlphaSource);
					IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
						_menuRect.right + 10, _menuRect.top + 180, 8, 3, _inputAlphaSource);
					letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 190, "Next Tap", _inputAlphaSource);
					break;
				case 2:
					IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
						_menuRect.right + 10, _menuRect.top + 100, 3, 2, _inputAlphaSource);
					letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 110, "Equip", _inputAlphaSource);
					IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
						_menuRect.right + 10, _menuRect.top + 140, 5, 2, _inputAlphaSource);
					letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 150, "Move", _inputAlphaSource);
					IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
						_menuRect.right + 10, _menuRect.top + 180, 8, 3, _inputAlphaSource);
					letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 190, "Next Tap", _inputAlphaSource);
					break;
				case 3:
					IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
						_menuRect.right + 10, _menuRect.top + 100, 3, 2, _inputAlphaSource);
					letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 110, "quaff", _inputAlphaSource);
					IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
						_menuRect.right + 10, _menuRect.top + 140, 5, 2, _inputAlphaSource);
					letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 150, "Move", _inputAlphaSource);
					IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
						_menuRect.right + 10, _menuRect.top + 180, 8, 3, _inputAlphaSource);
					letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 190, "Next Tap", _inputAlphaSource);
					break;
				}
			}
			else
			{
				IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
					_menuRect.right + 10, _menuRect.top + 100, 5, 2, _inputAlphaSource);
				letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 110, "Place", _inputAlphaSource);
				IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
					_menuRect.right + 10, _menuRect.top + 140, 8, 3, _inputAlphaSource);
				letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 150, "Next Tap", _inputAlphaSource);

			}

		}
		

		break;
	case 1:
		IMAGEMANAGER->findImage("skil")->render(getMemDC(), _menuRect.left, _menuRect.top + 88);
		IMAGEMANAGER->findImage("cursor_idle")->frameRender(getMemDC(),
			_menuRect.left - 13 + (_skilNum % 6) * 36, _menuRect.top + 113 + (_skilNum / 6) * 36);
		if (!_delay.menu)
		{
			IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
				_menuRect.right + 10, _menuRect.top + 100, 0, 0, _inputAlphaSource);
			letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 110, "Prev Tab", _inputAlphaSource);
			IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
				_menuRect.right + 10, _menuRect.top + 140, 8, 3, _inputAlphaSource);
			letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 150, "Next Tab", _inputAlphaSource);

		}

		break;
	case 2:
		IMAGEMANAGER->findImage("stat")->render(getMemDC(), _menuRect.left, _menuRect.top + 88);
		letterMacro(LETTER_WHITE, _menuRect.left + 56, _menuRect.top + 98, itoa(_player->getStat().str, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 126, _menuRect.top + 98, itoa(_player->getStat().def, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 202, _menuRect.top + 98, itoa(_player->getStat().mel, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 56, _menuRect.top + 116, itoa(_player->getStat().dex, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 126, _menuRect.top + 116, itoa(_player->getStat().fir, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 202, _menuRect.top + 116, itoa(_player->getStat().rng, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 56, _menuRect.top + 134, itoa(_player->getStat().vit, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 126, _menuRect.top + 134, itoa(_player->getStat().ice, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 202, _menuRect.top + 134, itoa(_player->getStat().crit, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 56, _menuRect.top + 152, itoa(_player->getStat().inl, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 126, _menuRect.top + 152, itoa(_player->getStat().lgt, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 202, _menuRect.top + 152, itoa(_player->getStat().aspd, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 56, _menuRect.top + 170, itoa(_player->getStat().lck, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 126, _menuRect.top + 170, itoa(_player->getStat().psn, _tmp, 10));
		letterMacro(LETTER_WHITE, _menuRect.left + 202, _menuRect.top + 170, itoa(_player->getStat().spd, _tmp, 10));
		IMAGEMANAGER->findImage("cursor_idle")->frameRender(getMemDC(),
			_menuRect.left - 15 + (_statNum % 3) * 78, _menuRect.top + 96 + (_statNum / 3) * 18);
		if (!_delay.menu)
		{
			IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
				_menuRect.right + 10, _menuRect.top + 100, 0, 0, _inputAlphaSource);
			letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 110, "Prev Tab", _inputAlphaSource);
			IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
				_menuRect.right + 10, _menuRect.top + 140, 8, 3, _inputAlphaSource);
			letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 150, "Next Tab", _inputAlphaSource);
		}

		break;
	case 3:
		IMAGEMANAGER->findImage("lvl")->render(getMemDC(), _menuRect.left, _menuRect.top + 88);
		IMAGEMANAGER->findImage("lvl_icon")->frameRender(getMemDC(), _lvlRect.left, _lvlRect.top - 28, _lvlNum - 1, 0);
		IMAGEMANAGER->findImage("lvl_icon")->frameRender(getMemDC(), _lvlRect.left, _lvlRect.top, _lvlNum, 0);
		IMAGEMANAGER->findImage("lvl_icon")->frameRender(getMemDC(), _lvlRect.left, _lvlRect.top + 28, _lvlNum + 1, 0);
		switch (_lvlNum)
		{
		case 0:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Sword");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Holy");

			break;
		case 1:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Sword");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Holy");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Defense");
			break;
		case 2:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Holy");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Defense");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Strength");
			break;
		case 3:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Defense");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Strength");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Dexterity");
			break;
		case 4:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Strength");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Dexterity");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Vitality");
			break;
		case 5:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Dexterity");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Vitality");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Intelligence");
			break;
		case 6:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Vitality");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Intelligence");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Luck");
			break;
		case 7:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Intelligence");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Luck");
			break;
		}
		if (_lvlPoint)
		{
			char tmp[10];
			itoa(_lvlPoint, tmp, 10);
			IMAGEMANAGER->findImage("plus")->alphaFrameRender(getMemDC(),
				_menuRect.left + 10, _menuRect.bottom - 28, _plusAlphaSource);
			letterMacro(LETTER_WHITE, _menuRect.left + 40, _menuRect.bottom - 26, tmp,_plusAlphaSource);
			letterMacro(LETTER_WHITE, _menuRect.left + 50, _menuRect.bottom - 26, " point available", _plusAlphaSource);
		}
		IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
			_menuRect.right + 10, _menuRect.top + 100, 3, 2, _inputAlphaSource);
		letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 110, "Level Up", _inputAlphaSource);
		IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
			_menuRect.right + 10, _menuRect.top + 140, 1, 7, _inputAlphaSource);
		letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 150, "Prev Rank", _inputAlphaSource);
		IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
			_menuRect.right + 10, _menuRect.top + 180, 2, 7, _inputAlphaSource);
		letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 190, "Next Rank", _inputAlphaSource);
		IMAGEMANAGER->findImage("inputImage")->alphaFrameRender(getMemDC(),
			_menuRect.right + 10, _menuRect.top + 220, 0, 0, _inputAlphaSource);
		letterMacro(LETTER_WHITE, _menuRect.right + 50, _menuRect.top + 230, "Prev Tap", _inputAlphaSource);

		break;
	}
	IMAGEMANAGER->findImage("player_icon")->render(getMemDC(), _menuRect.left + 4, _menuRect.top + 38);

	IMAGEMANAGER->findImage("money_background")->render(getMemDC(), WINSIZEX - 138, 10);
	IMAGEMANAGER->findImage("money_icon")->render(getMemDC(), WINSIZEX - 130, 4);

	coinNumberMacro(NUMBER_COIN_YELLOW, WINSIZEX - 104, 2, _currentMoney);
	if (_income > 0)
	{
		IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), WINSIZEX - 90, 34, 10, 1);
		coinNumberMacro(NUMBER_COIN_GREEN, WINSIZEX - 104, 34, _income);
	}

	for ( _viHit = _vHit.begin(); _viHit != _vHit.end(); ++_viHit)
	{
		char str[10];
		letterMacro2(_viHit->font, _viHit->x + camera.x , _viHit->y + camera.y, itoa(_viHit->damage, str, 10), _viHit->alphaSource);
	}
	//Draw minimap
	_map->drawMinimap();
}

void UI::collision()
{
	for ( _viItem = _vItem.begin(); _viItem != _vItem.end();++_viItem )
	{
		if (isCollision(_player->getRect(), _viItem->rc))
		{
			if (_viItem->name == NAME_COIN)
			{
				if (_viItem->point.x > _player->getPoint().x) _viItem->point.x -= 1;
				if (_viItem->point.x < _player->getPoint().x) _viItem->point.x += 1;
				if (_viItem->point.y > _player->getPoint().y) _viItem->point.y -= 4;
				_viItem->rc = RectMakeCenter(_viItem->point.x, _viItem->point.y, _viItem->img0->getFrameWidth(), _viItem->img0->getFrameHeight());
			}
		}
	}
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); )
	{
		if (isCollision(RectMakeCenter(_player->getPoint().x,_player->getPoint().y,2,2), _viItem->rc) && _viItem->name == NAME_COIN)
		{

			_vItem.erase(_viItem);
			setCoin(1);
			break;
		}
		else ++_viItem;
	}

}

void UI::showStatus()
{
	for (int i = 0; i < 5; i++)
	{
		switch ((*(_player->getStatusEffect()+i)).type)
		{
		case STATUSEFFECT_POISON:
			IMAGEMANAGER->findImage("effect")->frameRender(getMemDC(), _menuRect.left + 30 + 30 * i, _menuRect.top - 25, 2, 0);
			break;
		case STATUSEFFECT_FIRE:
			IMAGEMANAGER->findImage("effect")->frameRender(getMemDC(), _menuRect.left + 30 + 30 * i, _menuRect.top - 25, 2, 1);
			break;
		case STATUSEFFECT_STUN:
			IMAGEMANAGER->findImage("effect")->frameRender(getMemDC(), _menuRect.left + 30 + 30 * i, _menuRect.top - 25, 0, 3);
			break;
		case STATUSEFFECT_HEAL:
			IMAGEMANAGER->findImage("effect")->frameRender(getMemDC(), _menuRect.left + 30 + 30 * i, _menuRect.top - 25, 0, 0);
			break;
		}
	}

}

void UI::itemDraw(POINT camera)
{
	for ( _viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		if (_viItem->type != TYPE_WEAPON)
			_viItem->img0->render(getMemDC(), _viItem->rc.left + camera.x, _viItem->rc.top + camera.y);
		else
			_viItem->img0->frameRender(getMemDC(), _viItem->rc.left + camera.x, _viItem->rc.top + camera.y, 0, 0);
	}
}

void UI::explanation()
{
	switch (_menuNum)
	{
	case 0:
		for ( _viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
		{
			if (_viBag->position == _bagNum)
			{
				switch (_viBag->name)
				{
				case NAME_SWORD:
					letterMacro(LETTER_GREEN, _menuRect.left + 10, _menuRect.top - SPACE * 5,
						"Normal Sword");
					letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 4,
						"2-5 damage");
					letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
						"Normal Attack speed");
					letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE * 2,
						"Not the sharpest edge,");
					letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE * 1,
						" but it does its job.");
					break;
				case NAME_HEAL:					
					letterMacro(LETTER_GREEN, _menuRect.left + 10, _menuRect.top - SPACE * 4,
						"healing potion");
					letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
						"+10 health point");
					letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE * 2,
						"you can use potion");
					letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE * 1,
						" when you are in danger");
					break;
				}
			}

		}
		break;
	case 1:
		break;
	case 2:
		switch (_statNum)
		{
		case 0:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 5,
				"Strength");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 4,
				"Strength increases");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				" your damage,");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				" especially with melee");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE ,
				" weapons.");
			break;
		case 1:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 4,
				"Defense");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				"Defense is how");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				" resistant you are to");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 1,
				" physical damage.");
			break;
		case 2:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				"Melee Damage");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				"Increased damage of");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 1,
				" melee weapons.");
			break;
		case 3:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 5,
				"Dexterity");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 4,
				"Dexterity increases");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				" your attack speed");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				" especially with");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE,
				" ranged weapons.");
			break;
		case 4:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 5,
				"Fire Resist");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 4,
				"Each point of fire");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				" resistance increases");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				" your resistance to");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE,
				" fire damage by 25%");
			break;
		case 5:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 4,
				"Ranged Damage");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				"Increased damage of");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				" thrown or ranged");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 1,
				" weapons.");
			break;
		case 6:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				"Vitality");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				"Vitality increases");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 1,
				" your maximum health.");
			break;
		case 7:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 5,
				"Cold Resist");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 4,
				"Each point of cold");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				" resistance increases");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				" your resistance to");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE,
				" cold damage by 25%");
			break;
		case 8:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				"Critical");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				"% chance of a critical");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 1,
				" hit");
			break;
		case 9:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 6,
				"Intelligence");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 5,
				"Intelligence affects");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 4,
				" spell power as well");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				" as the number of");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE * 2,
				" spell charge each");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE,
				" spell has.");
			break;
		case 10:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 6,
				"Lightning Resist");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 5,
				"Each point of lightning");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 4,
				" resistance increases");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				" your resistance to");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE * 2,
				" lightning damage by");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE,
				" 25%");
			break;
		case 11:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				"Attack Speed");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				"% Increased attack");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 1,
				" speed.");
			break;
		case 12:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 6,
				"Luck");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 5,
				"Luck affects your");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 4,
				" critical strike rate");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				" and evasion, as well");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE * 2,
				" as chance based");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE,
				" effects.");
			break;
		case 13:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 6,
				"Poison Resist");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 5,
				"Each point of poison");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 4,
				" resistance increases");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				" your resistance to");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE * 2,
				" poison damage by");
			letterMacro(LETTER_WHITE, _menuRect.left + 14, _menuRect.top - SPACE,
				" 25%");
			break;
		case 14:
			letterMacro(LETTER_RED, _menuRect.left + 10, _menuRect.top - SPACE * 3,
				"Move Speed");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 2,
				"Increased Movement");
			letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE * 1,
				" speed amount.");
			break;
		}
		break;
	case 3:
		switch (_lvlNum)
		{
		case 0:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*8,
					"Sword");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Sword Dance");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Slide backwards or");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*4,
					"forwards while stabbing.");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Hitting an enemy while");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"sliding deals extra");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"damage.");
				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*8,
					"Sword");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Omnistab");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Hold down and attack");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*4,
					"while falling for a falling");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"attack. Hold up and");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"attack to do a weaker");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"overhead slash.");
				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Sword");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Power Attack");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Hold your attack button");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"to gather strength.");
				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Sword");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Quick Draw");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Hone your dexterity.");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Draw your sword faster");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"than the enemy can");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"react.");
				break;
			}
			break;
		case 1:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Holy");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Angelic Wings");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Take no damage from");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"falling.");
				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Holy");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Divine weapon");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Swinging your weapon");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"after damaging enemies");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"manifests holy light.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Holy");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Holy Shield");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Stored Divine weapon");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Charges give bonus");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"defense against");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"elemental damage.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Holy");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Second Chance");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Attacks cannot kill you");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"unless your HP is zero.");
				break;
			}
			break;
		case 2:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Defense");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Defensive Stance");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Hold down to block");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"physical attacks.");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Defense");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Aspect of Rock");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Become immune to");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"Knockbacks when taking");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"damage.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Defense");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Bulwark");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Gain a recharging");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"defensive shield.");
				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Defense");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Reflex Stance");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Correctly timed blocks will");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"reflect attack damage.");
				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Defense");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Aspect of Steel");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Some attacks will be");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"automatically blocked.");
				break;
			}
			break;
		case 3:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Strength");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"+1 Total Strength");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Strength increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"melee weapons.");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Strength");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"+2 Total Strength");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Strength increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"melee weapons.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Strength");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Heavy Blows (+3 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Strength)");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Gain a small knockback");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"bonus.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Strength");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"+4 Total Strength");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Strength increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"melee weapons.");

				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Strength");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Brute Force(+5 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Strength)");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"your maximum damage is");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"increased.");

				break;
			}
			break;
		case 4:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Dexterity");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+1 Total Dexterity");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Dexterity increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"attack/cast speed and");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"ranged weapons.");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Dexterity");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+2 Total Dexterity");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Dexterity increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"attack/cast speed and");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"ranged weapons.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Dexterity");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Quick Step(+3 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Dexterity) ");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Move faster while");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"attacking.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Dexterity");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+4 Total Dexterity");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Dexterity increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"attack/cast speed and");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"ranged weapons.");

				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Dexterity");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Finesse(+5 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Dexterity) ");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"your mimimum damage is");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"increased.");

				break;
			}
			break;
		case 5:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Vitality");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"+1 Total Vitality");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Vitality increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"maximum health");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Vitality");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"+2 Total Vitality");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Vitality increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"maximum health");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Vitality");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Thick Skin(+3 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Vitality) ");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Gain a permanent +1");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"defense bonus.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Vitality");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"+4 Total Vitality");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Vitality increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"maximum health");

				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Vitality");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Berserker's Blood(+5 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Vitality) ");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Gain +1 damage for each");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"20% of health missing.");

				break;

			}
			break;
		case 6:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*8,
					"Intelligence");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"+1 Total Intelligence");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Intelligence increases ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*4,
					"your maximum spell");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"charge, as well as your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"overall speel casting");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"abilities.");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*8,
					"Intelligence");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"+2 Total Intelligence");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Intelligence increases ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*4,
					"your maximum spell");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"charge, as well as your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"overall speel casting");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"abilities.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Intelligence");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Meditation(+3 Total");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Intelligence");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"your spell charge limit is ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"increased.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*8,
					"Intelligence");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"+4 Total Intelligence");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Intelligence increases ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*4,
					"your maximum spell");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"charge, as well as your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"overall speel casting");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"abilities.");

				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Intelligence");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Preparation(+5 Total");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Intelligence");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"your spell charge limit is ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"greatly increased.");

				break;
			}
			break;
		case 7:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Luck");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+1 Total Luck");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Luck affects your critical ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"strike rate and evasion,");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"as well as chance based");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"effects.");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Luck");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+2 Total Luck");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Luck affects your critical ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"strike rate and evasion,");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"as well as chance based");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"effects.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Luck");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Fortune(+3 Total Luck)");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Chests have a small");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"chance to drop an extra");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"item.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Luck");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+4 Total Luck");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Luck affects your critical ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"strike rate and evasion,");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"as well as chance based");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"effects.");

				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Luck");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Lucky Resist(+5 Total");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Luck");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Gain a chance to resist");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"non-physical damage.");

				break;
			}
			break;
		}
		break;
	}
}

void UI::hitOutput(float x, float y, int damage, LETTERFONT font)
{
	tagHitOutput hit;
	ZeroMemory(&hit, sizeof(tagHitOutput));
	hit.x = x;
	hit.y = y;
	hit.damage = damage;
	hit.alphaSource = 0;
	hit.font = font;

	_vHit.push_back(hit);
}

void UI::setItemToBag(ITEMNAME name)
{
	int count = 0;
	bool end = false;

	tagItem item;
	ZeroMemory(&item, sizeof(item));
	item.name = name;
	switch (item.name)
	{
	case NAME_SWORD:
		item.img = IMAGEMANAGER->findImage("sword");
		item.img0 = IMAGEMANAGER->findImage("sword0");
		item.minDmg = 8;
		item.maxDmg = 15;
		item.type = TYPE_WEAPON;
		break;
	case NAME_HEAL:
		item.img = IMAGEMANAGER->findImage("heal");
		item.img0 = IMAGEMANAGER->findImage("heal0");
		item.itemStat.hp = 10;
		item.type = TYPE_POTION;
		break;
	}
	item.alphaSource = 255;
	item.equip = false;
	while (true)
	{
		if (_vBag.size() == 0)
		{
			item.position = 0;
			end = true;

		}
		else
		{
			for (_viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
			{
				if (_viBag->position == count)
				{
					count++;
					end = false;
					break;
				}
				end = true;
			}

		}
		if (end)
		{
			item.position = count;
			break;
		}

	}
	_vBag.push_back(item);
}
void UI::rectMove()
{
	if (_active)
	{
		_menuRect.top -= 15;
		_menuRect.bottom -= 15;
		if (_menuRect.top < WINSIZEY - 280)
		{
			_menuRect.top = WINSIZEY - 280;
			_menuRect.bottom = WINSIZEY;
		}
	}
	else
	{
		_menuRect.top += 15;
		_menuRect.bottom += 15;
		if (_menuRect.top >WINSIZEY - 88)
		{
			_menuRect.top = WINSIZEY - 88;
			_menuRect.bottom = _menuRect.top + 280;
		}

	}

	_lvlRect = RectMake(_menuRect.left + 18, _menuRect.top + 157, 192, 84);

}

void UI::keyControl()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT) && _active)
	{
		switch (_menuNum)
		{
		case 0:
			if (_bagNum % 6 != 0)_bagNum--;
			if (_bagNum < 0) _bagNum = 0;
			break;
		case 1:
			if (_skilNum % 6 != 0) _skilNum--;
			if (_skilNum < 0) _skilNum = 0;
			break;
		case 2:
			if (_statNum % 3 != 0)_statNum--;
			if (_statNum < 0) _statNum = 0;
			break;
		case 3:
			_rankNum--;
			if (_rankNum < 0) _rankNum = 0;
			break;
		}
		setInputGuide();

	}
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) && _active)
	{
		switch (_menuNum)
		{
		case 0:
			if (_bagNum % 6 != 5) _bagNum++;
			if (_bagNum > 29) _bagNum = 29;
			break;
		case 1:
			if (_skilNum % 6 != 5)_skilNum++;
			if (_skilNum > 29) _skilNum = 29;
			break;
		case 2:
			if (_statNum % 3 != 2) _statNum++;
			if (_statNum > 14) _statNum = 14;
			break;
		case 3:
			_rankNum++;
			if (_rankNum > 3 && _lvlNum <= 1) _rankNum = 3;
			else if (_rankNum > 4 && _lvlNum > 1) _rankNum = 4;
			break;
		}
		setInputGuide();

	}

	if (KEYMANAGER->isOnceKeyDown(VK_UP) && _active)
	{
		switch (_menuNum)
		{
		case 0:
			if (_bagNum / 6 != 0) _bagNum -= 6;
			if (_bagNum < 0) _bagNum = 0;
			break;
		case 1:
			if (_skilNum / 6 != 0)_skilNum -= 6;
			if (_skilNum < 0) _skilNum = 0;
			break;
		case 2:
			if (_statNum / 3 != 0) _statNum -= 3;
			if (_statNum < 0) _statNum = 0;
			break;
		case 3:
			_lvlNum--;
			if (_lvlNum != 0) _rankNum = 0;
			if (_lvlNum < 0) _lvlNum = 0;
			break;
		}
		setInputGuide();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _active)
	{
		switch (_menuNum)
		{
		case 0:
			if (_bagNum / 6 != 4)_bagNum += 6;
			if (_bagNum > 29) _bagNum = 29;
			
			break;
		case 1:
			if (_skilNum / 6 != 4)_skilNum += 6;
			if (_skilNum > 29) _skilNum = 29;
			break;
		case 2:
			if (_statNum / 3 != 4) _statNum += 3;
			if (_statNum > 14) _statNum = 14;
			break;
		case 3:
			if (_lvlNum != 7)
			{
				_lvlNum++;
				_rankNum = 0;
			}
			if (_lvlNum > 7) _lvlNum = 7;
			break;
		}
		setInputGuide();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LSHIFT))
	{
		_menuNum++;
		if (_menuNum > 3) _menuNum = 0;
		setInputGuide();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_A))
	{
		_menuNum--;
		if (_menuNum < 0) _menuNum = 3;
		setInputGuide();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_Z))
	{
		switch (_menuNum)
		{
		case 0:
			if (!_move)
			{
				for (_viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
				{
					if (_viBag->position == _bagNum)
					{
						_move = true;
						_save.position0= _viBag->position;
						break;
					}
				}
			}
			else
			{
				for (_viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
				{
					if (_viBag->position == _bagNum)
					{
						_viBag->position = _save.position0;
						continue;
					}
					if (_viBag->position == _save.position0)
					{
						_viBag->position = _bagNum;
					}
				}
				_move = false;
			}
			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		default:
			break;
		}
		setInputGuide();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_X))
	{
		switch (_menuNum)
		{
		case 0:
			for ( _viBag = _vBag.begin(); _viBag != _vBag.end(); )
			{
				if (_viBag->position == _bagNum)
				{
					if (_viBag->type == TYPE_WEAPON)
					{
						if (_viBag->equip)
						{
							_viBag->equip = false;
							_player->setEquipWeapon(_hand);
						}
						else
						{
							for (int i = 0; i < _vBag.size(); i++)
							{
								if (_vBag[i].type == TYPE_WEAPON && _vBag[i].equip) _vBag[i].equip = false;
							}
							_viBag->equip = true;
							tagItem item;
							ZeroMemory(&item, sizeof(tagItem));
							item.img0 = _viBag->img0;
							item.minDmg = _viBag->minDmg;
							item.maxDmg = _viBag->maxDmg;
							_player->setEquipWeapon(item);

							SOUNDMANAGER->play("6_Instrument_Sound", 0.5);
						}
						break;
					}
					if (_viBag->type == TYPE_POTION)
					{
						tagStatusEffect tse;
						tse.damage = 5;
						tse.leftTime = 20;
						tse.type = STATUSEFFECT_HEAL;
						_vBag.erase(_viBag);
						_player->addStatusEffect(tse);

						break;
					}
				}
				else ++_viBag;
			}
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		}
		setInputGuide();
	}

	//if (KEYMANAGER->isOnceKeyDown('T'))
	//{
	//	setItemToBag(NAME_HEAL);
	//}

	//if (KEYMANAGER->isOnceKeyDown('Y'))
	//{
	//	setCoin(1);
	//}
	//if (KEYMANAGER->isOnceKeyDown('U'))
	//{
	//	hitOutput(WINSIZEX / 2, WINSIZEY / 2, 8, LETTER_RED);
	//}
}

void UI::setInputGuide()
{
	_delay.menu = 30;
	if(_menuNum == 0 && !_move)
	for (_viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
	{
		if (_viBag->position == _bagNum)
		{
			switch (_viBag->type)
			{
			case TYPE_WEAPON:
				switch (_viBag->equip)
				{
				case TRUE:
					_inputGuide = 1;
					break;
				case FALSE:
					_inputGuide = 2;
					break;
				}
				break;
			case TYPE_POTION:
				_inputGuide = 3;
				break;
			}
			break;
		}
		_inputGuide = 0;
	}


}


void UI::coinNumberMacro(NUMBERFONT font, float x, float y, int num)
{
	int a, b, c, d;
	a = num / 1000;
	b = (num % 1000) / 100;
	c = (num % 100) / 10;
	d = num % 10;
	switch (font)
	{
	case NUMBER_COIN_YELLOW:
		if (a != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x, y, a, 0);
		if (a != 0 || b != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 18, y, b, 0);
		if (a != 0 || b != 0 || c != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 36, y, c, 0);
		IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 54, y, d, 0);
		break;
	case NUMBER_COIN_GREEN:
		if (a != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x, y, a, 1);
		if (a != 0 || b != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 18, y, b, 1);
		if (a != 0 || b != 0 || c != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 36, y, c, 1);
		IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 54, y, d, 1);
		break;
	case NUMBER_COIN_RED:
		if (a != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x, y, a, 2);
		if (a != 0 || b != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 18, y, b, 2);
		if (a != 0 || b != 0 || c != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 36, y, c, 2);
		IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 54, y, d, 2);
		break;
	}
}

void UI::hpTopNumberMacro(float x, float y, int num)
{
	int a, b, c, d, e, f;
	a = num / 100;
	b = (num % 100) / 10;
	c = num % 10;

	d = _maxHp / 100;
	e = (_maxHp % 100) / 10;
	f = _maxHp % 10;


	
	if (a != 0)IMAGEMANAGER->findImage("hpBarTopNumber")->frameRender(getMemDC(), x, y, a, 0);
	if (a != 0 || b != 0) IMAGEMANAGER->findImage("hpBarTopNumber")->frameRender(getMemDC(), x + 11, y, b, 0);
	IMAGEMANAGER->findImage("hpBarTopNumber")->frameRender(getMemDC(), x + 22, y, c, 0);
	IMAGEMANAGER->findImage("hpBarTopNumber")->frameRender(getMemDC(), x + 33, y, 10, 0);
	IMAGEMANAGER->findImage("hpBarTopNumber")->frameRender(getMemDC(), x + 44, y, d, 0);
	IMAGEMANAGER->findImage("hpBarTopNumber")->frameRender(getMemDC(), x + 55, y, e, 0);
	IMAGEMANAGER->findImage("hpBarTopNumber")->frameRender(getMemDC(), x + 66, y, f, 0);

}

void UI::hpBottomNumberMacro(float x, float y, int num)
{
	int a, b, c, d, e, f;
	a = num / 100;
	b = (num % 100) / 10;
	c = num % 10;

	d = _maxHp / 100;
	e = (_maxHp % 100) / 10;
	f = _maxHp % 10;



	if (a != 0)IMAGEMANAGER->findImage("hpBarBottomNumber")->frameRender(getMemDC(), x, y, a, 0);
	if (a != 0 || b != 0) IMAGEMANAGER->findImage("hpBarBottomNumber")->frameRender(getMemDC(), x + 11, y, b, 0);
	IMAGEMANAGER->findImage("hpBarBottomNumber")->frameRender(getMemDC(), x + 22, y, c, 0);
	IMAGEMANAGER->findImage("hpBarBottomNumber")->frameRender(getMemDC(), x + 33, y, 10, 0);
	IMAGEMANAGER->findImage("hpBarBottomNumber")->frameRender(getMemDC(), x + 44, y, d, 0);
	IMAGEMANAGER->findImage("hpBarBottomNumber")->frameRender(getMemDC(), x + 55, y, e, 0);
	IMAGEMANAGER->findImage("hpBarBottomNumber")->frameRender(getMemDC(), x + 66, y, f, 0);

}

void UI::letterMacro(LETTERFONT font, float x, float y, const char *str)
{
	for (int i = 0; *(str + i) != '\0'; i++)
	{
		int sour;
		int dest = 0;
		if (*(str + i) >= 65 && *(str + i) <= 90)
		{
			sour = *(str + i) - 65;
			dest = 0;
		}
		else if (*(str + i) >= 97 && *(str + i) <= 122)
		{
			sour = *(str + i) - 97;
			dest = 1;
		}
		else if (*(str + i) >= 33 && *(str + i) <= 57)
		{
			sour = *(str + i) - 33;
			dest = 2;
		}
		else if (*(str + i) == 32)
		{
			sour = 26;
		}
		IMAGEMANAGER->findImage("letter")->alphaFrameRender(getMemDC(), x + i * 8, y, sour, dest + font * 3,0);
	}

}

void UI::letterMacro(LETTERFONT font, float x, float y, const char *str, int alpha)
{
	for (int i = 0; *(str + i) != '\0'; i++)
	{
		int sour;
		int dest = 0;
		if (*(str + i) >= 65 && *(str + i) <= 90)
		{
			sour = *(str + i) - 65;
			dest = 0;
		}
		else if (*(str + i) >= 97 && *(str + i) <= 122)
		{
			sour = *(str + i) - 97;
			dest = 1;
		}
		else if (*(str + i) >= 33 && *(str + i) <= 57)
		{
			sour = *(str + i) - 33;
			dest = 2;
		}
		else if (*(str + i) == 32)
		{
			sour = 26;
		}
		IMAGEMANAGER->findImage("letter")->alphaFrameRender(getMemDC(), x + i * 8, y, sour, dest + font * 3, alpha);
	}

}

void UI::letterMacro2(LETTERFONT font, float x, float y, const char *str, int alpha)
{
	for (int i = 0; *(str + i) != '\0'; i++)
	{
		int sour;
		int dest = 0;
		if (*(str + i) >= 65 && *(str + i) <= 90)
		{
			sour = *(str + i) - 65;
			dest = 0;
		}
		else if (*(str + i) >= 97 && *(str + i) <= 122)
		{
			sour = *(str + i) - 97;
			dest = 1;
		}
		else if (*(str + i) >= 33 && *(str + i) <= 57)
		{
			sour = *(str + i) - 33;
			dest = 2;
		}
		else if (*(str + i) == 32)
		{
			sour = 26;
		}
		IMAGEMANAGER->findImage("letter2")->alphaFrameRender(getMemDC(), x + i * 8, y, sour, dest + font * 3, alpha);
	}

}


void UI::repeatIndex(string keyName, int delay)
{
	if (_count % delay == 0)
	{
		if (IMAGEMANAGER->findImage(keyName)->getFrameX() < IMAGEMANAGER->findImage(keyName)->getMaxFrameX())
			IMAGEMANAGER->findImage(keyName)->setFrameX(IMAGEMANAGER->findImage(keyName)->getFrameX() + 1);
		else IMAGEMANAGER->findImage(keyName)->setFrameX(0);
	}
}

void UI::setCoin(int coin)
{
	_income += coin;
	_delay.coin = 100;
}

void UI::addImg()
{
	//============================ U I ========================================
	IMAGEMANAGER->addImage("bag", "Img/ui/bag.bmp", 228, 192, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("stat", "Img/ui/stat.bmp", 228, 192, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("lvl", "Img/ui/lvl.bmp", 228, 192, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("selectOn", "Img/ui/selectOn.bmp", 224, 18, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("selectOff", "Img/ui/selectOff.bmp", 224, 18, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hpBarBottom", "Img/ui/hpBarBottom.bmp", 224, 26, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("money_icon", "Img/ui/money_icon.bmp", 32, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("money_background", "Img/ui/money_background.bmp", 128, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("equip_slot", "Img/ui/equip_slot.bmp", 176, 44, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("player_icon", "Img/ui/player_icon.bmp", 28, 28, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("skil", "Img/ui/skill.bmp", 228, 192, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("lvl_icon", "Img/ui/lvl_icon.bmp", 224, 28, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("cursor_idle", "Img/ui/cursor_idle.bmp", 144, 36, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("cursor_move", "Img/ui/cursor_move.bmp", 144, 36, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("plus", "Img/ui/plus.bmp", 48, 48, 3, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("equip_icon", "Img/ui/equip_icon.bmp", 48, 16, 3, 1, true, RGB(255, 0, 255)); 
	IMAGEMANAGER->addImage("hpBarTop", "Img/ui/hpBarTop.bmp", 184, 14, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("effect", "Img/ui/status_effect.bmp", 128, 256, 4, 8, true, RGB(255, 0, 255));

	//========================= F O N T ========================================
	IMAGEMANAGER->addFrameImage("coin_number", "Img/ui/font/number.bmp", 416, 96, 13, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("letter", "Img/ui/font/letter_font.bmp", 270, 180, 27, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("letter2", "Img/ui/font/letter_font_2size.bmp", 378, 240, 27, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("inputImage", "Img/ui/font/inputImage.bmp", 320, 320, 10, 10, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("hpBarBottomNumber", "Img/ui/font/hpBarBottomNumber.bmp", 121, 14, 11, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("hpBarTopNumber", "Img/ui/font/hpBarTopNumber.bmp", 121, 14, 11, 1, true, RGB(255, 0, 255));

	//========================= I T E M =========================================
	IMAGEMANAGER->addImage("sword", "Img/ui/item/sword.bmp", 40, 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("heal", "Img/ui/item/heal.bmp", 40, 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("hand", "Img/ui/item/hand.bmp", 80, 40, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("coin", "Img/ui/item/coin.bmp", 6, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("sword0", "Img/ui/item/sword0.bmp", 64, 14, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("heal0", "Img/ui/item/potion0.bmp", 18, 18, true, RGB(255, 0, 255));
}

void UI::addItemOnMap(ITEMNAME name, POINT point)
{
	tagItem item;
	ZeroMemory(&item, sizeof(item));
	item.name = name;
	item.point = point;
	switch (item.name)
	{
	case NAME_SWORD:
		item.img = IMAGEMANAGER->findImage("sword");
		item.img0 = IMAGEMANAGER->findImage("sword0");
		item.minDmg = 2;
		item.maxDmg = 5;
		item.rc = RectMakeCenter(item.point.x, item.point.y, 32, 14);
		item.type = TYPE_WEAPON;
		break;
	case NAME_HEAL:
		item.img = IMAGEMANAGER->findImage("heal");
		item.img0 = IMAGEMANAGER->findImage("heal0");
		item.itemStat.hp = 10;
		item.rc = RectMakeCenter(item.point.x, item.point.y, 18, 18);
		item.type = TYPE_POTION;
		break;
	case NAME_COIN:
		item.img0 = IMAGEMANAGER->findImage("coin");
		item.rc = item.rc = RectMakeCenter(item.point.x, item.point.y, 6, 6);
	}
	item.alphaSource = 255;
	item.equip = false;

	_vItem.push_back(item);


}

void UI::deleteItemOnMap(int arrNum)
{
	_vItem.erase(_vItem.begin() + arrNum);
}

void UI::addCoinOnMap(POINT point)
{

}