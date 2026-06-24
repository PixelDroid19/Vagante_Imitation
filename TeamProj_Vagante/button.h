#pragma once
#include "gameNode.h"

// Callback function pointer
typedef void(*CALLBACK_FUNCTION)(void);

//<Functional>
// Or template	std::Function
//				std::move
//				std::bind
// Use when extending general classes or windows
// Example: externally created object

// Consider using callback functions

enum BUTTONDIRECTION
{
	BUTTONDIRECTION_NULL,
	BUTTONDIRECTION_UP,
	BUTTONDIRECTION_DOWN
};

class button : public gameNode
{
private:
	BUTTONDIRECTION _direction;		// Button direction

	const char* _imageName;			// Button image name
	image* _image;					// Button image
	RECT _rc;
	float _x, _y;

	POINT _btnDownFramePoint;
	POINT _btnUpFramePoint;

	CALLBACK_FUNCTION _callbackFunction;

public:
	HRESULT init(const char* imageName, int x, int y,
		POINT btnDownFramePoint, POINT btnUpFramePoint,
		CALLBACK_FUNCTION cbFunction);
	void release(void);
	void update(void);
	void render(void);



	button();
	~button();
};
