#include <gata/utils/pvr.h>
#include <gata/utils/MyUtils.h>

#include <stdio.h>

bool readPvr(const char* fileName, PvrInfo* pInfo)
{
	int length;
	FILE* f = my_fopen(fileName, &length);
	if (!f)
	{
		return false;
	}

	const int kPvrHeaderSize = sizeof(PVR_TEXTURE_HEADER);

	fread(&pInfo->header, 1, kPvrHeaderSize, f);
	
	pInfo->dataLength = length - kPvrHeaderSize;
	
	SAFE_DEL_ARRAY(pInfo->pData);
	pInfo->pData = new unsigned char[pInfo->dataLength];
	
	fread(pInfo->pData, 1, pInfo->dataLength, f);

	fclose(f);

	return true;
}
