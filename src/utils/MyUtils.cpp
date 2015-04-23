#include <gata/utils/MyUtils.h>

#include <gata/core/macro.h>

#include <string>
#include <cassert>

#include <math.h>

namespace gata {
	namespace utils {

//=======================================================================================

#ifdef WIN32
#include <Windows.h>
#endif

unsigned long long getCurrentMillis()
{
#ifdef WIN32
	return (unsigned long long)GetTickCount();
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return (unsigned long long)(
		(tv.tv_sec * 1000) + (tv.tv_usec / 1000)
	);
#endif
}

bool isRectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	if (x1 + w1 <= x2) return false;
	if (x2 + w2 <= x1) return false;

	if (y1 + h1 <= y2) return false;
	if (y2 + h2 <= y1) return false;

	return true;
}

float mydiv(int a, int b)
{
	if (a == 0) return 0.0f;
	if (b == 0) return MY_MAX_FLOAT;
	return (float)a / (float)b;
}

int getCommonLength(int k1, int len1, int k2, int len2)
{
	return MY_MIN(k1 + len1, k2 + len2) - MY_MAX(k1, k2);
}

bool strEndsWith(const char* szString, const char* szKey)
{
	int lenKey = strlen(szKey);
	int lenString = strlen(szString);

	if (lenKey > lenString) return false;

	return !strcmp(szString + lenString - lenKey, szKey);
}

//=======================================================================================
	}
}
