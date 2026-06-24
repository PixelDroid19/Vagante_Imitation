#include "singletonBase.h"
#include <chrono>

//====================================
// ## 17.08.07 ## randomFunction ##
//====================================

//                            singletonBase<T>
class randomFunction : public singletonBase < randomFunction >
{
public:

	randomFunction(void)
	{
		srand(static_cast<unsigned int>(
			std::chrono::system_clock::now().time_since_epoch().count()));
	}
	~randomFunction(void) {}

	//Get random integer between 0 and number-1
	inline int getInt(int num) { return rand() % num; }
	//Get random integer between fromNum and toNum
	inline int getFromIntTo(int fromNum, int toNum) { return rand() % (toNum - fromNum + 1) + fromNum; }

	//Get random float
	inline float getFloat() { return (float)rand() / (float)RAND_MAX; }
	inline float getFloat(float num) { return ((float)rand() / (float)RAND_MAX) * num; }
	inline float getFromFloatTo(float fromFloat, float toFloat)
	{
		float rnd = (float)rand() / (float)RAND_MAX;

		return (rnd * (toFloat - fromFloat) + fromFloat);
	}

};