#ifndef _GLES20_IMAGE_H_
#define _GLES20_IMAGE_H_

#include <gata/graphic/Image.h>

#ifdef WIN32
	#include <gles20/EGL/egl.h>
	#include <gles20/GLES2/gl2.h>
	#include <gles20/GLES2/gl2ext.h>
#else
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif

namespace gata {
	namespace graphic {
//===========================================================
class Gles20Image : public Image
{
public:
	Gles20Image() : m_texId(0) { }

	~Gles20Image()
	{
		unload();
	}

	bool load(const char* szName);
	void* getHandle();
	void unload();

private:
	GLuint m_texId;
};
//===========================================================
	}
}

#endif
