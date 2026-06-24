#pragma once

#include "platform_compat.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdarg>

#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>

#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "utils.h"
#include "imageManager.h"
#include "timeManager.h"
#include "soundManager.h"
#include "effectManager.h"
#include "iniDataManager.h"
#include "sceneManager.h"
#include "keyAniManager.h"
#include "txtData.h"
#include "collision.h"
#include "database.h"
#include "debugDraw.h"

using namespace std;
using namespace IOTA_UTIL;

#define WINNAME "Iota Team"
#define WINSTARTX 50
#define WINSTARTY 50
#define WINSIZEX 800
#define WINSIZEY 600
#define WINSTYLE 0

#define RND randomFunction::getSingleton()
#define KEYMANAGER keyManager::getSingleton()
#define IMAGEMANAGER imageManager::getSingleton()
#define TXTDATA txtData::getSingleton()
#define TIMEMANAGER timeManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define EFFECTMANAGER effectManager::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define KEYANIMANAGER keyAniManager::getSingleton()
#define INIDATA iniDataManager::getSingleton()
#define DATABASE database::getSingleton()

#define SAFE_DELETE(p) {if(p) {delete(p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[](p); (p) = NULL;}}
#define SAFE_RELEASE(p) {if(p) {(p)->release(); (p)=NULL;}}

extern SDL_Window* _hWnd;
extern SDL_Renderer* _renderer;
extern POINT _ptMouse;
