#pragma once

namespace IOTA_UTIL
{
	typedef struct tagPoint
	{
		float x;
		float y;

		tagPoint() : x(0.0f), y(0.0f) {}

		void move(float dx, float dy)
		{
			x += dx;
			y += dy;
		}
	}MYPOINT, *LPMYPOINT;

	typedef struct tagCircle
	{
		float x;
		float y;
		float r;

		tagCircle() : x(0.0f), y(0.0f), r(0.0f) {}
		tagCircle(float _x, float _y, float _r) : x(_x), y(_y), r(_r) {}

		void move(float dx, float dy)
		{
			x += dx;
			y += dy;
		}

		void set(float centerX, float centerY, float radius)
		{
			x = centerX;
			y = centerY;
			r = radius;
		}

		void setCenterPos(float posX, float posY)
		{
			x = posX;
			y = posY;
		}

	}MYCIRCLE, *LPMYCIRCLE;

	typedef struct tagMYRECT
	{
		float left;
		float top;
		float right;
		float bottom;

		tagMYRECT() : left(0.0f), top(0.0f), right(0.0f), bottom(0.0f) {}
		tagMYRECT(float _left, float _top, float _right, float _bottom)
			: left(_left), top(_top), right(_right), bottom(_bottom) {}

		void move(float dx, float dy)
		{
			left += dx;
			top += dy;
			right += dx;
			bottom += dy;
		}

		void set(float _left, float _top, float _right, float _bottom)
		{
			left = _left;
			top = _top;
			right = _right;
			bottom = _bottom;
		}

		void setLeftTopPos(float _left, float _top)
		{
			float width = getWidth();
			float height = getHeight();

			left = _left;
			top = _top;
			right = left + width;
			bottom = top + height;
		}

		void setCenterPos(float centerX, float centerY)
		{
			float halfW = getWidth() * 0.5f;
			float halfH = getHeight() * 0.5f;

			left = centerX - halfW;
			top = centerY - halfH;
			right = centerX + halfW;
			bottom = centerY + halfH;
		}

		float getWidth(void) { return right - left; }
		float getHeight(void) { return bottom - top; }
	}MYRECT, *LPMYRECT;

	float getAngle(float x1, float y1, float x2, float y2);

	bool checkPointInRect(const RECT& rc, const POINT& pt);
	bool checkPointInRect(const RECT& rc, int x, int y);
	bool checkPointInRect(const MYRECT& rc, float x, float y);
	bool checkPointInRect(const MYRECT& rc, const MYPOINT& pt);

	bool checkPointInCircle(float cX, float cY, float cR, const MYPOINT& pt);
	bool checkPointInCircle(float cX, float cY, float cR, float x, float y);
	bool checkPointInCircle(const MYCIRCLE& rc, float x, float y);
	bool checkPointInCircle(const MYCIRCLE& rc, const MYPOINT& pt);

	bool isCollision(const MYRECT& rc1, const MYRECT& rc2);
	bool isCollision(const RECT& rc1, const RECT& rc2);

	bool isCollision(const MYCIRCLE& cir1, const MYCIRCLE& cir2);

	bool isCollision(const MYCIRCLE& cir1, const RECT& rc);
	bool isCollision(const MYCIRCLE& cir, const MYRECT& rc);

	bool isCollisionReaction(const RECT& rcHold, RECT& rcMove);
	bool isCollisionReaction(const MYRECT& mrcHold, MYRECT& mrcMove);
	bool isCollisionReaction(const MYCIRCLE& cirHold, MYCIRCLE& cirMove);
}
