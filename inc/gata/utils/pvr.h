#ifndef _PVR_H_
#define _PVR_H_

#include "../core/macro.h"

typedef struct PVR_TEXTURE_HEADER_TAG{ 
	unsigned intdwHeaderSize;  /* size of the structure */ 
	unsigned intdwHeight;  /* height of surface to be created */ 
	unsigned intdwWidth;  /* width of input surface */ 
	unsigned intdwMipMapCount;  /* number of MIP-map levels requested */ 
	unsigned intdwpfFlags;  /* pixel format flags */ 
	unsigned intdwDataSize;  /* Size of the compress data */ 
	unsigned intdwBitCount;  /* number of bits per pixel */ 
	unsigned intdwRBitMask;  /* mask for red bit */ 
	unsigned intdwGBitMask;  /* mask for green bits */ 
	unsigned intdwBBitMask;  /* mask for blue bits */ 
	unsigned intdwAlphaBitMask;  /* mask for alpha channel */ 
	unsigned intdwPVR;  /* should be 'P' 'V' 'R' '!' */ 
	unsigned intdwNumSurfs;  /* number of slices for volume textures or skyboxes */ 
} PVR_TEXTURE_HEADER;

typedef struct _PvrInfo
{
	_PvrInfo() : pData(0) { }
	~_PvrInfo()
	{
		SAFE_DEL(pData);
	}

	PVR_TEXTURE_HEADER	header;
	unsigned char*		pData;
	int					dataLength;
} PvrInfo;

bool readPvr(const char* fileName, PvrInfo* pInfo);

#endif
