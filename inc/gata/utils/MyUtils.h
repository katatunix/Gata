#ifndef _MY_UTILS_H_
#define _MY_UTILS_H_

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
FILE* my_fopen(const char* szFilePath, int* pLength);
}
#endif

namespace gata {
	namespace utils {
//================================================================================================

void freeStringsList(int number, char** psz);

bool isRectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

float mydiv(int a, int b);

int getCommonLength(int k1, int len1, int k2, int len2);

unsigned long long getCurrentMillis();

bool strEndsWith(const char* szString, const char* szKey);
//================================================================================================
	}
}
#endif
