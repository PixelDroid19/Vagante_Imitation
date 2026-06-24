#include "stdafx.h"
#include "mainGame.h"

mainGame::mainGame()
{
}


mainGame::~mainGame()
{

}


HRESULT mainGame::init(void)
{
	gameNode::init(true);
	
	SCENEMANAGER->addScene("GameScene", new GameScene);
	SCENEMANAGER->changeScene("GameScene");
	return S_OK;
}

void mainGame::release(void)
{
	
}


void mainGame::update(void)	
{
	gameNode::update();

		
	SCENEMANAGER->update();
	
	
}


void mainGame::render(void)	
{
	SDL_SetRenderDrawColor(getMemDC(), 255, 255, 255, 255);
	SDL_RenderClear(getMemDC());

	SCENEMANAGER->render();
}
