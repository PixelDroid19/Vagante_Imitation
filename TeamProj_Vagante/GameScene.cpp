#include "stdafx.h"
#include "GameScene.h"


GameScene::GameScene()
{
}


GameScene::~GameScene()
{

}
HRESULT GameScene::init()
{

	addImage();
	_em = new EnemyManager;
	_player = new Player;
	_map = new Map;
	_ui = new UI;

	//??????? ????
	_map->setEnemyManagerAddressLink(_em);
	_map->setPlayerAddressLink(_player);
	_map->setUiAddressLink(_ui);
	_player->setEnemyManagerAddressLink(_em);
	_player->setMapAddressLink(_map);
	_player->setUiAddressLink(_ui);
	_em->setPlayerAddressLink(_player);
	_em->setMapAddressLink(_map);
	_em->setUiAddressLink(_ui);
	_ui->setPlayerAddressLink(_player);
	_ui->setMapAddressLink(_map);
	_ui->setEnemyManagerAddressLink(_em);

	_ui->init();
	_map->init();
	_player->init(PointMake(TILESIZE*(50), TILESIZE*(5)));
	_em->init();

	_UsingStatusWindow = false;
	_camera = PointMake(WINSIZEX, WINSIZEY);

	return S_OK;
}
void GameScene::release()
{

}
void GameScene::update()
{
	_camera = _player->getPoint();
	_em->update();
	_player->update();
	_ui->update();
	_map->update();
	keyInput();

	SOUNDMANAGER->update();

	// Clamp camera to map bounds (skip if background image failed to load)
	if (_camera.x < WINSIZEX / 2)
	{
		_camera.x = WINSIZEX / 2;
	}
	else
	{
		image* bg = IMAGEMANAGER->findImage("bg");
		if (bg && _camera.x > bg->getWidth() - WINSIZEX / 2)
			_camera.x = bg->getWidth() - WINSIZEX / 2;
	}
	if (_camera.y < WINSIZEY / 2)
	{
		_camera.y = WINSIZEY / 2;
	}
	else
	{
		image* bg = IMAGEMANAGER->findImage("bg");
		if (bg && _camera.y > bg->getHeight() - WINSIZEY / 2)
			_camera.y = bg->getHeight() - WINSIZEY / 2;
	}
}
void GameScene::render()
{
	POINT camera = PointMake(WINSIZEX / 2 - _camera.x, WINSIZEY / 2 - _camera.y);
	_map->render(camera);
	_em->render(camera);
	_player->render(camera);
	_ui->render(camera);

	if (!isDebugDrawEnabled())
		return;

	_map->drawDebug(camera);
	_em->drawDebug(camera);
	_player->drawDebug(camera);
}

void GameScene::addImage()
{
	//????? ????? ????!!
	IMAGEMANAGER->addImage("bg", "Img/etc/temp_bg.bmp", TILESIZE * 58, TILESIZE * 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("????", "Img/etc/explosion_1.bmp", 448, 64, 7, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("test", "Img/etc/tes.bmp", 40, 224, 1, 7, true, RGB(255, 0, 255));


	IMAGEMANAGER->addFrameImage("boss_dragon_cry", "Img/enemy/dragon_cry.bmp", 304, 164, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("boss_dragon_dead", "Img/enemy/dragon_dead.bmp", 39 * 2, 30 * 2, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("boss_dragon_stamp", "Img/enemy/dragon_drive.bmp", 152 * 2, 80 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("boss_dragon_fire", "Img/enemy/dragon_fire.bmp", 175 * 2, 76 * 2, 5, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("fireball", "Img/enemy/dragon_fireball.bmp", 640, 160, 16, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("boss_dragon_fly", "Img/enemy/dragon_fly.bmp", 186 * 2, 76 * 2, 6, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("boss_dragon_sleep", "Img/enemy/dragon_sleep.bmp", 37 * 2, 30 * 2, 1, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("wormMoveUp", "Img\\enemy\\crawler_move_up.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveDown", "Img\\enemy\\crawler_move_down.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveLeft", "Img\\enemy\\crawler_move_left.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveRight", "Img\\enemy\\crawler_move_right.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("bathit", "Img\\enemy\\bat_hit.bmp", 32, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("batsleep", "Img\\enemy\\bat_idle.bmp", 32, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("batflying", "Img\\enemy\\bat_moving.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("slime_Idle", "Img\\enemy\\slime_idle.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("slime_Move", "Img\\enemy\\slime_dash.bmp", 192, 64, 6, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("slime_Dash_Cast", "Img\\enemy\\slime_dashCast.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("slime_Dash", "Img\\enemy\\slime_dash.bmp", 192, 64, 6, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("slime_Jump", "Img\\enemy\\slime_jump.bmp", 96, 64, 3, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("slime_Falling", "Img\\enemy\\slime_fall.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("minimap", "Img\\map\\miniMap.bmp", 232, 160, true, RGB(255, 0, 255));
	//============================================================== Music ====================================================================

	SOUNDMANAGER->addSound("0_boss_Backgound_Music", "Sound/BossSound.ogg", true, true);
	SOUNDMANAGER->addSound("1_Player_Attack_Sound", "Sound/PlayerAttack.wav", false, false);
	SOUNDMANAGER->addSound("2_Player_Jump_Sound", "Sound/PlayerJump.wav", false, false);
	SOUNDMANAGER->addSound("3_Player_Demage_Sound", "Sound/PlayerDamage.wav", false, false);
	SOUNDMANAGER->addSound("4_Player_Death_Music", "Sound/PlayerDeath.ogg", true, true);
	SOUNDMANAGER->addSound("5_Enemy_Demage_Sound", "Sound/EnemyDamage.wav", false, false);
	SOUNDMANAGER->addSound("6_Instrument_Sound", "Sound/Instrument.wav", false, false);
	SOUNDMANAGER->addSound("7_DarkCave_Music", "Sound/DarkCave.ogg", true, true);
	SOUNDMANAGER->addSound("8_BossFireball", "Sound/BossFireball.wav", false, false);
	SOUNDMANAGER->addSound("9_BossStamp", "Sound/BossStamp.wav", false, false);

	//============================================================== ManEater ====================================================================

	//IMAGEMANAGER->addImage("maneater_Dead", "image/enemy/manEater_dead.bmp", 32, 32, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("maneataer_Attack", "image/enemy/manEater_attack.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("maneater_Attack_After", "image/enemy/manEater_after_attack.bmp", 224, 32, 7, 1, true, RGB(255, 0, 255));

	//IMAGEMANAGER->addImage("1", "image\\enemy\\manEater_dead.bmp", 32, 32, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("2", "image\\enemy\\manEater_attack.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("3", "image\\enemy\\manEater_after_attack.bmp", 224, 32, 7, 1, true, RGB(255, 0, 255));

	//IMAGEMANAGER->addImage("maneater_Dead", "manEater_dead.bmp", 32, 32, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("maneater_Attack", "manEater_attack.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("maneater_Attack_After", "manEater_after_attack.bmp", 224, 32, 7, 1, true, RGB(255, 0, 255));


	//============================================================== Goblin ====================================================================

	//IMAGEMANAGER->addFrameImage("goblin_Idle", "image/enemy/goblin_idle.bmp", 32, 68, 1, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("doblin_Hit", "image/enemy/goblin_hit.bmp", 32, 68, 1, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("goblin_Dead", "image/enemy/goblin_dead.bmp", 32, 68, 1, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("goblin_Attack", "image/enemy/goblin_attack.bmp,", 256, 68, 4, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("goblin_Jump", "image/enemy/goblin_jump.bmp", 128, 68, 4, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("goblin_Move", "image/enemy/goblin_move.bmp,", 256, 68, 8, 2, true, RGB(255, 0, 255));
	//
	////============================================================== Bat ====================================================================
	//
	//IMAGEMANAGER->addImage("bat_Idle", "image/enemy/bat_idle.bmp", 32, 32, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addImage("bat_Hit", "image/enemy/bat_hit.bmp", 32, 32, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("bat_Move", "image/enemy/bat_moving.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));
	//
	////============================================================== Slime ====================================================================
	//
	//IMAGEMANAGER->addFrameImage("slime_Idle", "image/enemy/slime_idle.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("slime_Move", "image/enemy/slime_move.bmp", 192, 64, 6, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("slime_Dash_Cast", "image/enemy/slime_dashCast.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("slime_Dash", "image/enemy/slime_dash.bmp", 192, 64, 6, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("slime_Jump", "image/enemy/slime_jump.bmp", 96, 64, 3, 2, true, RGB(255, 0, 255));

}
void GameScene::keyInput()
{
	if (KEYMANAGER->isOnceKeyDown(VK_F3))
		toggleDebugDraw();

	if (KEYMANAGER->isOnceKeyDown(VK_F4) && isDebugDrawEnabled())
	{
		static const int layerCycle[] = {
			DEBUG_LAYER_GRID | DEBUG_LAYER_TILES,
			DEBUG_LAYER_PLAYER_9,
			DEBUG_LAYER_RAYS,
			DEBUG_LAYER_PATH,
			DEBUG_LAYER_HITBOX,
			DEBUG_LAYER_ALL
		};
		static int layerIndex = 5;
		layerIndex = (layerIndex + 1) % 6;
		setDebugDrawLayers(layerCycle[layerIndex]);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		if (_UsingStatusWindow)
		{
			_ui->setActive(false);
			_player->setCtrl(true);
			_UsingStatusWindow = false;
		}
		else
		{
			_ui->setActive(true);
			_player->setCtrl(false);
			_UsingStatusWindow = true;
		}
	}
}