#include "Gles20Image.h"

#include <gata/utils/tga.h>
#include <gata/utils/pvr.h>

#include <gata/utils/MyUtils.h>


#include <cassert>

namespace gata {
	namespace graphic {
//====================================================================================
bool Gles20Image::load(const char* szName)
{
	using namespace gata::utils;

	if (strEndsWith(szName, ".tga"))
	{
		tgaInfo* info = tgaLoad(szName);

		if (!info || info->status != TGA_OK)
		{
			tgaDestroy(info);
			return false;
		}

		GLint format = 0;
		if (info->pixelDepth == 32)
		{
			format = GL_RGBA;
		}
		else if (info->pixelDepth == 24)
		{
			format = GL_RGB;
		}
		else
		{
			assert(0);
		}

		glGenTextures(1, &m_texId);
		glBindTexture(GL_TEXTURE_2D, m_texId);

		glTexImage2D(GL_TEXTURE_2D, 0, format, info->width, info->height, 0, format, GL_UNSIGNED_BYTE, info->imageData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
		glBindTexture(GL_TEXTURE_2D, 0);

		m_width = info->width;
		m_height = info->height;

		tgaDestroy(info);
	}
	else if (strEndsWith(szName, ".pvr"))
	{
		PvrInfo info;
		bool ok = readPvr(szName, &info);
		if (!ok) return false;

		glGenTextures(1, &m_texId);
		glBindTexture(GL_TEXTURE_2D, m_texId);

		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES,
			info.header.intdwWidth, info.header.intdwHeight, 0, info.dataLength, info.pData);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
		glBindTexture(GL_TEXTURE_2D, 0);

		m_width = info.header.intdwWidth;
		m_height = info.header.intdwHeight;
	}
	else
	{
		assert(0);
	}

	return true;
}

void* Gles20Image::getHandle()
{
	return m_texId > 0 ? &m_texId : 0;
}

void Gles20Image::unload()
{
	if (m_texId > 0)
	{
		glDeleteTextures(1, &m_texId);
	}
}
//====================================================================================
	}
}
