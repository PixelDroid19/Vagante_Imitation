#include "stdafx.h"
#include "mainGame.h"

//If compiled as Unicode
HINSTANCE _hInstance;		//Application instance handle
HWND _hWnd;					//Window handle
POINT _ptMouse;

LPTSTR _lpszClass = TEXT("Iota Team");


mainGame _mg;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void setWindowsSize(int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	MSG message;			//Message
	WNDCLASS wndClass;		//Window class registration structure

	//Salt Bae 
	_hInstance = hInstance;

	wndClass.cbClsExtra = 0;										//Extra class memory
	wndClass.cbWndExtra = 0;										//Extra window memory
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//Window background brush
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//Window cursor
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//Window icon
	wndClass.hInstance = hInstance;									//Instance handle
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//Window message processor
	wndClass.lpszClassName = _lpszClass;							//Window class name
	wndClass.lpszMenuName = NULL;									//Window menu name
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//Window style

	//Register window class
	RegisterClass(&wndClass);

	//Create window
	_hWnd = CreateWindow(
		WINNAME,
		WINNAME,
		WS_OVERLAPPEDWINDOW,		//Window style
		WINSTARTX,					//Window start X position
		WINSTARTY,					//Window start Y position
		WINSIZEX,					//Window width
		WINSIZEY,					//Window height
		NULL,						//Parent window handle
		(HMENU)NULL,				//Menu handle
		hInstance,					//Instance
		NULL);						//Child window handle

	setWindowsSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	//Show window on screen
	ShowWindow(_hWnd, cmdShow);

	if (FAILED(_mg.init()))
	{
		return 0;
	}

	//GetMessage: blocks when no messages pending
	//PeekMessage: returns immediately if no pending messages (non-blocking game loop)

	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);	//Translate keyboard input to character messages
			DispatchMessage(&message);	//Dispatch message to window procedure
		}
		else
		{
			TIMEMANAGER->update(60.0f);
			_mg.update();
			_mg.render();
		}
	}

	/* Original GetMessage-based message loop
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);	//Translate keyboard input to character messages
		DispatchMessage(&message);	//Dispatch message to window procedure
	}
	*/
	_mg.release();

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _mg.MainProc(hWnd, iMessage, wParam, lParam);
}

void setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;
	
	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	AdjustWindowRect(&winRect, WINSTYLE, false);

	SetWindowPos(_hWnd, NULL, x, y, (winRect.right - winRect.left),
		(winRect.bottom - winRect.top), SWP_NOZORDER | SWP_NOMOVE);

}
