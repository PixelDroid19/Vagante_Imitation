#include "stdafx.h"
#include "Map.h"
#include "EnemyManager.h"
#include "Player.h"
#include "UI.h"

Map::Map()
{
}


Map::~Map()
{
}
HRESULT Map::init()
{
	setTile();
	setObject();
	// Load background images
	_BgImg = new image;
	_BgImg->init("Img\\etc\\BackGround.bmp", 1980, 1280, true, RGB(255, 0, 255));
	_mapImg = new image;
	_mapImg->init("Img\\etc\\VaganteMap.bmp", 1856, 1280, true, RGB(255, 0, 255));
	_trapImg = new image;
	_trapImg->init("Img\\map\\Trap.bmp", 32, 32, true, RGB(255, 0, 255));
	miniMap.Image = new image;
	miniMap.Image->init("Img\\map\\miniMap.bmp", 232, 160, true, RGB(255, 0, 255));
	miniMap.X = 0;
	miniMap.Y = 0;
	miniMap.playerRc = RectMakeCenter(0,0, 4, 4);
	/*
	_SightImg = new image;
	_SightImg->init("Img\\map\\SightMap.bmp", 1856, 1280, true, RGB(255, 0, 255),true);
	_SightBackImg = new image;
	_SightBackImg->init("Img\\map\\SightMap.bmp", 1856, 1280, true, RGB(255, 0, 255), true);*/

		return S_OK;
}
void Map::release()
{

}
void Map::update()
{
	for (int i = 0; i < 5; i++)
	{
		if (coinBox[i]._openBox && !coinBox[i]._eventChk)
		{
			coinBox[i].FrameX++;

			for (int j = 0; j < RND->getFromIntTo(1, 3); j++)
			{
				_ui->addItemOnMap(NAME_COIN, PointMake(RND->getFromIntTo(coinBox[i].rc.left, coinBox[i].rc.right), coinBox[i].rc.top - 4));
			}
			if (coinBox[i].FrameX >= coinBox[i].Image->getMaxFrameX())
			{
				coinBox[i].FrameX = coinBox[i].Image->getMaxFrameX();
				coinBox[i]._eventChk = true;
			}

		}
	}
	if (itemBox[0]._openBox && !itemBox[0]._eventChk)
	{
		itemBox[0].FrameX++;
		if (itemBox[0].FrameX >= itemBox[0].Image->getMaxFrameX())
		{
			_ui->addItemOnMap(NAME_SWORD, PointMake(itemBox[0].rc.left + (itemBox[0].rc.right - itemBox[0].rc.left)/2, itemBox[0].rc.top - 4));
			itemBox[0].FrameX = itemBox[0].Image->getMaxFrameX();
			itemBox[0]._eventChk = true;
		}
	}

	if (itemBox[1]._openBox && !itemBox[1]._eventChk)
	{
		itemBox[1].FrameX++;
		if (itemBox[1].FrameX >= itemBox[1].Image->getMaxFrameX())
		{
			_ui->addItemOnMap(NAME_HEAL, PointMake(itemBox[1].rc.left + (itemBox[1].rc.right - itemBox[1].rc.left) / 2, itemBox[1].rc.top - 4));
			itemBox[1].FrameX = itemBox[1].Image->getMaxFrameX();
			itemBox[1]._eventChk = true;
		}
	}





	/*
	Register item info example

	tagItemInfo item;
	//item
	item.bigImg = IMAGEMANAGER->findImage(); // Big image (image displayed in inventory)
	item.smallImg = IMAGEMANAGER->findImage(); //Small image (image displayed on map)
	//stats
	item.itemstat.hp = 0;
	item.itemstat.str = 1;
	item.itemstat.dex = 0;
	item.itemstat.vit = 0;
	item.itemstat.inl = 0;
	item.itemstat.lck = 0;
	item.itemstat.def = 0;
	item.itemstat.fir = 0;
	item.itemstat.ice = 0;
	item.itemstat.lgt = 0;
	item.itemstat.psn = 0;
	item.itemstat.mel = 0;
	item.itemstat.rng = 0;
	item.itemstat.crit = 0;
	item.itemstat.aspd = 0;
	item.itemstat.spd = 0;
	item.itemstate = ITEMSTATE_ONMAP;
	//item position index
	item.point = ;
	//item rect
	item.rc = ;
	//Created item is saved to the map!
	_ui->addItemOnMap(item);
	*/

}
void Map::render()
{
}

//Camera render	x = camera.x offset
//				y = camera.y offset adjusted!!!!
void Map::render(POINT camera)
{
	draw(camera);
}
void Map::draw(POINT camera)
{
	//test~
//	IMAGEMANAGER->render("bg", getMemDC(), camera.x, camera.y);

	_BgImg->render(getMemDC(), camera.x, camera.y);


	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 58; j++)
		{
			if (_mapInfo[i][j].type == MAPTILE_WALL || _mapInfo[i][j].type == MAPTILE_WALL2)
			{
				/*Rectangle(getMemDC(), _mapInfo[i][j].rc.left + camera.x,
					_mapInfo[i][j].rc.top + camera.y,
					_mapInfo[i][j].rc.right + camera.x,
					_mapInfo[i][j].rc.bottom + camera.y);*/
			}
			else if (_mapInfo[i][j].type == MAPTILE_LADDER)
			{
				/*Rectangle(getMemDC(), _mapInfo[i][j].rc.left + camera.x,
					_mapInfo[i][j].rc.top + camera.y,
					_mapInfo[i][j].rc.right + camera.x,
					_mapInfo[i][j].rc.bottom + camera.y);*/
			}
			else if (_mapInfo[i][j].type == MAPTILE_GROUND_CAN_GO_DOWN_1)
			{
				/*Rectangle(getMemDC(), _mapInfo[i][j].rc.left + camera.x,
					_mapInfo[i][j].rc.top + camera.y,
					_mapInfo[i][j].rc.right + camera.x,
					_mapInfo[i][j].rc.bottom + camera.y);*/
			}
			else if (_mapInfo[i][j].type == MAPTILE_SPIKE_TRAP)
			{
				/*Rectangle(getMemDC(), _mapInfo[i][j].rc.left + camera.x,
					_mapInfo[i][j].rc.top + camera.y,
					_mapInfo[i][j].rc.right + camera.x,
					_mapInfo[i][j].rc.bottom + camera.y);*/
				_trapImg->render(getMemDC(), _mapInfo[i][j].rc.left +camera.x, _mapInfo[i][j].rc.top + camera.y);
			}
		}
	}
	_mapImg->render(getMemDC(), camera.x, camera.y);
	for(int i = 0; i < ITEMBOXMAX; i++)
	{
		//Rectangle(getMemDC(), itemBox[i].rc.left + camera.x, itemBox[i].rc.top + camera.y, itemBox[i].rc.right + camera.x, itemBox[i].rc.bottom + camera.y);
		itemBox[i].Image->frameRender(getMemDC(), itemBox[i].X+camera.x, itemBox[i].Y + camera.y, itemBox[i].FrameX, itemBox[i].FrameY);
	}
	for (int i = 0; i < COINBOXMAX; i++)
	{
		
		//Rectangle(getMemDC(), coinBox[i].rc.left + camera.x, coinBox[i].rc.top + camera.y, coinBox[i].rc.right + camera.x, coinBox[i].rc.bottom + camera.y);
		coinBox[i].Image->frameRender(getMemDC(), coinBox[i].X + camera.x, coinBox[i].Y + camera.y, coinBox[i].FrameX, coinBox[i].FrameY);
		
	}


	miniMap.Image->render(getMemDC(), miniMap.X, miniMap.Y);
	if (isDebugDrawEnabled())
	{
		int mx = miniMap.playerRc.left + _player->getPoint().x / 8;
		int my = miniMap.playerRc.top + _player->getPoint().y / 8;
		debugDrawPoint(getMemDC(), mx, my, 2, DebugPalette::Player);
	}
	else
		Rectangle(getMemDC(), miniMap.playerRc.left+_player->getPoint().x/8, miniMap.playerRc.top + _player->getPoint().y / 8, miniMap.playerRc.right+ _player->getPoint().x / 8, miniMap.playerRc.bottom + _player->getPoint().y / 8);
}
void Map::drawDebug(POINT camera)
{
	if (!isDebugDrawEnabled())
		return;

	SDL_Renderer* dc = getMemDC();
	const int viewLeft = -camera.x;
	const int viewTop = -camera.y;
	const int viewRight = viewLeft + WINSIZEX;
	const int viewBottom = viewTop + WINSIZEY;

	const int rowStart = std::max(0, viewTop / TILESIZE - 1);
	const int rowEnd = std::min(39, viewBottom / TILESIZE + 1);
	const int colStart = std::max(0, viewLeft / TILESIZE - 1);
	const int colEnd = std::min(57, viewRight / TILESIZE + 1);

	if (isDebugLayerEnabled(DEBUG_LAYER_GRID) || isDebugLayerEnabled(DEBUG_LAYER_TILES))
	{
		for (int i = rowStart; i <= rowEnd; i++)
		{
			for (int j = colStart; j <= colEnd; j++)
			{
				const RECT& tile = _mapInfo[i][j].rc;
				const int type = _mapInfo[i][j].type;

				if (isDebugLayerEnabled(DEBUG_LAYER_GRID))
					debugDrawRect(dc, tile, camera, DebugPalette::Grid);

				if (!isDebugLayerEnabled(DEBUG_LAYER_TILES))
					continue;

				if (type == MAPTILE_WALL || type == MAPTILE_WALL2)
				{
					debugDrawFilledRect(dc, tile, camera, DebugPalette::WallFill);
					debugDrawRect(dc, tile, camera, DebugPalette::Wall);
				}
				else if (type == MAPTILE_LADDER)
					debugDrawRect(dc, tile, camera, DebugPalette::Ladder);
				else if (type == MAPTILE_SPIKE_TRAP)
					debugDrawRect(dc, tile, camera, DebugPalette::Trap);
			}
		}
	}

	if (isDebugLayerEnabled(DEBUG_LAYER_HITBOX))
	{
		for (int i = 0; i < ITEMBOXMAX; i++)
			debugDrawRect(dc, itemBox[i].rc, camera, DebugPalette::ItemBox);

		for (int i = 0; i < COINBOXMAX; i++)
			debugDrawRect(dc, coinBox[i].rc, camera, DebugPalette::CoinBox);
	}
}
void Map::drawMinimap()
{
	miniMap.Image->render(getMemDC(), miniMap.X, miniMap.Y);
	Rectangle(getMemDC(), miniMap.playerRc.left + _player->getPoint().x / 8, miniMap.playerRc.top + _player->getPoint().y / 8, miniMap.playerRc.right + _player->getPoint().x / 8, miniMap.playerRc.bottom + _player->getPoint().y / 8);

}

void Map::setTile()
{

	/* 
		0 : MAPTILE_NULL
		1 : MAPTILE_WALL,
		2 : MAPTILE_WALL2,
		3 : MAPTILE_LADDER,
		4 : MAPTILE_GROUND_CAN_GO_DOWN_1,
		5 : MAPTILE_SPIKE_TRAP
	*/

	//Map data
	int temp[40][58] = 
	{
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 } ,
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },

		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,3,0,0,0, 0,0,1,1,1, 1,1,0,0,0, 1,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,1,1, 0,0,0, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,3,1,1,4, 4,4,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,1,1, 1,0,0, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,3,1,1,0, 0,0,0,1,0, 1,4,4,4,4, 4,4,1,0,1, 4,4,4,4,4, 4,4,4,1,0, 0,0,0,1,1, 0,0,0, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,3,1,1,1, 1,0,0,0,0, 1,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 0,0,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,3,0,0,1, 1,0,0,0,4, 0,0,0,0,0, 0,0,0,4,0, 1,0,0,0,0, 0,0,1,0,1, 1,0,0,0,0, 0,0,1, 1,1,1,1,1 },
											 
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,3,0,1,1, 1,1,1,0,0, 0,5,5,0,0, 5,0,5,0,1, 1,0,5,0,0, 0,0,1,1,1, 1,0,0,0,0, 0,0,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,3,0,1,1, 1,1,1,1,1, 1,1,1,4,4, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,0,0,0, 0,1,1,1,1, 1,1,1,4,4, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,0,4,4,4, 4,0,0,1,1, 1,1,1,3,0, 1,1,1,1,0, 0,0,0,0,0, 0,0,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, 1,1,1,3,0, 0,1,1,1,0, 0,0,0,0,0, 0,0,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },

		{ 1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 1,1,0,0,0, 1,1,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,4,0, 0,0,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 0,0,0,0,0, 1,1,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,0,0,0, 0,0,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,1,4,0,0, 4,1,1,1,1, 1,1,0,0,4, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 5,0,1,1,1, 0,0,0,0,0, 0,0,0,1,1, 1,1,5,0,5, 5,1,1,1,1, 0,0,0,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,4,0,0, 0,4,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,4,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },

		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,0,3,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 4,4,3,1,1, 0,0,0,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,0,0,4,4, 0,0,1,1,1, 0,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,0,0,1,1, 0,0,0,1,1, 1,1,1,1,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,0,1,0, 0,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,3,0,0,0, 0,0,0,1,1, 1,0,0,0,0, 4,1,0,1,1, 0,0,1,4,0, 0,1,0,0,0, 0,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,3,0,1,1, 1,1,1,1,1, 1,0,0,1,5, 5,1,0,1,1, 0,0,1,0,0, 0,0,0,0,0, 0,1,1,1,1, 1,1,1, 1,1,1,1,1 },
																//24,26
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,0,0,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,0,1,1, 0,0,1,4,0, 0,0,0,0,0, 0,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 3,0,0,1,1, 1,1,1,1,1, 1,1,0,0,0, 0,0,0,1,1, 0,0,1,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 3,0,0,1,1, 1,1,1,1,1, 1,1,1,0,0, 1,1,1,1,1, 1,1,1,4,4, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,1, 1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,3,0, 0,0,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 1,1,0,1,1, 0,0,0,1,1, 1,1,1,0,0, 0,0,0,0,0, 1,1,0,3,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
														//21							//26,35		 
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,0,0,1,0, 0,1,0,1,1, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,3,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,0,1,1,0, 0,0,0,1,1, 0,4,1,4,0, 1,1,1,0,0, 0,0,0,3,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 1,1,1,0,0, 0,0,0,3,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,1,0,0,0, 0,0,1,1,1, 4,0,0,0,4, 1,1,1,4,0, 0,0,0,0,0, 0,0,0,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,1,5,1,5, 1,5,1,1,1, 0,0,0,0,0, 1,1,1,0,0, 0,0,0,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },

		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 } ,
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 }

	};

	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 58; j++)
		{
			_mapInfo[i][j].rc = RectMake(_mapInfo[i][j].point.x, _mapInfo[i][j].point.y, TILESIZE, TILESIZE);
		}
	}


	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 58; j++)
		{
			_mapInfo[i][j].type = temp[i][j];
			_mapInfo[i][j].point = PointMake(j*TILESIZE, i*TILESIZE);
			_mapInfo[i][j].rc = RectMake(_mapInfo[i][j].point.x, _mapInfo[i][j].point.y, TILESIZE, TILESIZE);
		}
	}

}
void Map::setObject()
{
	for (int i = 0; i < ITEMBOXMAX; i++)
	{
		itemBox[i].Image = new image;
		itemBox[i].Image->init("Img\\map\\Box.bmp", 192, 32,6,1, true, RGB(255, 0, 255));
		itemBox[i].FrameX = 0;
		itemBox[i].FrameY = 0;
		itemBox[i]._openBox = false;
		itemBox[i]._eventChk = false;
	}
	for (int i = 0; i < COINBOXMAX; i++)
	{
		coinBox[i].Image = new image;
		coinBox[i].Image->init("Img\\map\\Box.bmp", 192, 32, 6, 1, true, RGB(255, 0, 255));
		coinBox[i].FrameX = 0;
		coinBox[i].FrameY = 0;
		coinBox[i]._openBox = false;
		coinBox[i]._eventChk = false;
	}
	itemBox[0].X = _mapInfo[15][7].rc.left;
	itemBox[0].Y = _mapInfo[15][7].rc.top;
	itemBox[0].rc = RectMake(itemBox[0].X, itemBox[0].Y, itemBox[0].Image->getFrameWidth(), itemBox[0].Image->getFrameHeight());

	itemBox[1].X = _mapInfo[33][41].rc.left;
	itemBox[1].Y = _mapInfo[33][41].rc.top;
	itemBox[1].rc = RectMake(itemBox[1].X, itemBox[1].Y, itemBox[1].Image->getFrameWidth(), itemBox[1].Image->getFrameHeight());


	coinBox[0].X = _mapInfo[8][45].rc.left;
	coinBox[0].Y = _mapInfo[8][45].rc.top;
	coinBox[0].rc = RectMake(coinBox[0].X, coinBox[0].Y, coinBox[0].Image->getFrameWidth(), coinBox[0].Image->getFrameHeight());

	coinBox[1].X = _mapInfo[26][35].rc.left;
	coinBox[1].Y = _mapInfo[26][35].rc.top;
	coinBox[1].rc = RectMake(coinBox[1].X, coinBox[1].Y, coinBox[1].Image->getFrameWidth(), coinBox[1].Image->getFrameHeight());

	coinBox[2].X = _mapInfo[29][21].rc.left;
	coinBox[2].Y = _mapInfo[29][21].rc.top;
	coinBox[2].rc = RectMake(coinBox[2].X, coinBox[2].Y, coinBox[2].Image->getFrameWidth(), coinBox[2].Image->getFrameHeight());

	coinBox[3].X = _mapInfo[23][21].rc.left;
	coinBox[3].Y = _mapInfo[23][21].rc.top;
	coinBox[3].rc = RectMake(coinBox[3].X, coinBox[3].Y, coinBox[3].Image->getFrameWidth(), coinBox[3].Image->getFrameHeight());

	coinBox[4].X = _mapInfo[25][26].rc.left;
	coinBox[4].Y = _mapInfo[25][26].rc.top;
	coinBox[4].rc = RectMake(coinBox[4].X, coinBox[4].Y, coinBox[4].Image->getFrameWidth(), coinBox[4].Image->getFrameHeight());
	//test

}

