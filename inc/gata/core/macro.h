#ifndef _MACRO_H_
#define _MACRO_H_

#include <stdio.h>
#include <stdlib.h>
#include <cassert>

#define MY_MIN_INT (-2147483647)
#define MY_MAX_INT (2147483647)

#define MY_MAX_FLOAT (1000000.0f)

#define MY_MAX(x, y) ((x) > (y) ? (x) : (y))
#define MY_MIN(x, y) ((x) < (y) ? (x) : (y))

#define MY_SQR(x) ((x) * (x))

#define SAFE_DEL(x) if (x) { delete x; x = 0; }
#define SAFE_DEL_ARRAY(x) if (x) { delete [] x; x = 0; }

#ifdef WIN32
	#define  LOGI(...)  { printf(__VA_ARGS__); printf("\n"); }
	#define  LOGE(...)  { printf(__VA_ARGS__); printf("\n"); }
	#define  LOGD(...)  { printf(__VA_ARGS__); printf("\n"); }
	#define  LOGW(...)  { printf(__VA_ARGS__); printf("\n"); }
#else
	#include <jni.h>
	#include <android/log.h>

	#define  LOG_TAG    "GAME"
	#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,		LOG_TAG,	__VA_ARGS__)
	#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,		LOG_TAG,	__VA_ARGS__)
	#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,		LOG_TAG,	__VA_ARGS__)
	#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARNING,	LOG_TAG,	__VA_ARGS__)
#endif

#endif
