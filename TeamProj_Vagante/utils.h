#pragma once
#define PI 3.141592654f
#define PI2 PI * 2

#define PI8 float(PI / 8.0f)
#define PI16 float(PI / 16.0f)
#define PI32 float(PI / 32.0f)
#define PI64 float(PI / 64.0f)
#define PI128 float(PI / 128.0f)

#define FLOAT_TO_INT(f1) static_cast<int>(f1)
#define FLOAT_EPSILON 0.001f	// Maximum difference for real number comparison (graphic error prevention)


namespace IOTA_UTIL
{
	// Distance calculation function
	float getDistance(float startX, float startY, float endX, float endY);

	// Angle calculation function
	float getAngle(float x1, float y1, float x2, float y2);
}
