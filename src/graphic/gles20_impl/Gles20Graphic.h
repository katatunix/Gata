#ifndef _GLES20_GRAPHIC_H_
#define _GLES20_GRAPHIC_H_

#include <gata/graphic/Graphic.h>

#ifdef WIN32
	#include <gles20/EGL/egl.h>
	#include <gles20/GLES2/gl2.h>
#else
	#include <GLES2/gl2.h>
#endif

namespace gata {
	namespace graphic {
//=========================================================================================

#define MAX_TRIANGLES_NUMBER 10240

class Tranform;

class Gles20Graphic : public Graphic
{
public:
	~Gles20Graphic()
	{
		uninit();
	}

	bool init(void* pContext);
	void resize(int width, int height);
	void uninit();
	
	void setClearColor(int rgba);

	void clear();
	void flush();

	void sendToGpu();

	void setClip(int x, int y, int w, int h);
	void resetClip();

	void fillRect(int x, int y, int w, int h);

	void setColor(int rgba);

	void drawImage(
		Image* p,
		int xDst, int yDst,
		int xSrc, int ySrc, int wSrc, int hSrc,
		float xScale = 1.0f, float yScale = 1.0f,
		Transform* pTransform = 0);

	void drawImage(
		Image* p,
		int xDst, int yDst,
		float xScale = 1.0f, float yScale = 1.0f,
		Transform* pTransform = 0);

private:
	void printGLString(const char* name, GLenum s);
	// GLES2 stuff
	GLuint createShader(const char* szSource, GLenum type);
	GLuint createProg(GLuint vertexShaderId, GLuint fragmentShaderId);
	bool linkProg(GLuint progId);

	//
	void applyTransform(GLfloat* pVertex, GLfloat* pTexCoord, Transform* p);
	void convertPos(int x, int y, GLfloat& xEs, GLfloat& yEs);
	void convertSize(int w, int h, GLfloat& wEs, GLfloat& hEs);

	//
	GLfloat m_r, m_g, m_b, m_a;
	int m_width;
	int m_height;

	//
#ifdef WIN32
	EGLDisplay m_eglDisplay;
	EGLSurface m_eglSurface;
	EGLContext m_eglContext;
#endif

	GLuint m_progObjId;
	GLuint m_vsId;
	GLuint m_fsId;

	int m_positionSlot;
	int m_texCoordSlot;
	int m_colorSlot;
	int m_samplerSlot;

	//
	GLfloat m_pVerticesList[3 * 2 * MAX_TRIANGLES_NUMBER];
	GLfloat m_pTexCoordsList[3 * 2 * MAX_TRIANGLES_NUMBER];
	GLfloat m_pColorsList[3 * 4 * MAX_TRIANGLES_NUMBER];

	int m_trianglesCount;

	GLuint m_currentTexId;
	GLuint m_dummyTexId;
};
//=========================================================================================
	}
}

#endif
