#include "Gles20Graphic.h"

#include <gata/graphic/Image.h>
#include <gata/graphic/Transform.h>
#include <gata/core/macro.h>

namespace gata {
	namespace graphic {
//===============================================================================================================

bool Gles20Graphic::init(void* pContext)
{
	assert(!m_isInitOk);

#ifdef WIN32
	// Init EGL

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
	
	EGLint attribList[] =
	{
		EGL_RED_SIZE,       8,
		EGL_GREEN_SIZE,     8,
		EGL_BLUE_SIZE,      8,
		EGL_ALPHA_SIZE,     EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_NONE
	};

	// Get Display
	m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (m_eglDisplay == EGL_NO_DISPLAY) return false;

	// Initialize EGL
	if (!eglInitialize(m_eglDisplay, &majorVersion, &minorVersion)) return false;

	eglBindAPI(EGL_OPENGL_ES_API);

	// Get configs
	if (!eglGetConfigs(m_eglDisplay, NULL, 0, &numConfigs)) return false;

	// Choose config
	if (!eglChooseConfig(m_eglDisplay, attribList, &config, 1, &numConfigs)) return false;

	// Create a surface
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, (NativeWindowType)pContext, 0);
	if (m_eglSurface == EGL_NO_SURFACE) return false;

	// Create a GL context
	EGLContext m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, contextAttribs);
	if (m_eglContext == EGL_NO_CONTEXT) return false;

	// Make the context current
	if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)) return false;
#endif

	// Printf some info
	printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

	// Init OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(107/255.0f, 140/255.0f, 1.0f, 1.0f);

	glEnable(GL_SCISSOR_TEST);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Shaders
	const char* k_szVsSource =
		"attribute vec4 a_position;								\n"
		"attribute vec2 a_texCoord;								\n"
		"attribute vec4 a_color;								\n"
		"varying vec2 v_texCoord;								\n"
		"varying vec4 v_color;									\n"
		"void main()											\n"
		"{														\n"
		"	v_texCoord = a_texCoord;							\n"
		"	v_color = a_color;									\n"
		"	gl_Position = a_position;							\n"
		"}														\n";
	const char* k_szFsSource =
		"precision mediump float;								\n"
		"varying vec2 v_texCoord;								\n"
		"varying vec4 v_color;									\n"
		"uniform sampler2D u_sampler;							\n"
		
		"void main()														\n"
		"{																	\n"
		"	gl_FragColor = texture2D(u_sampler, v_texCoord) + v_color;		\n"
		"}																	\n";

	m_vsId = createShader(k_szVsSource, GL_VERTEX_SHADER); assert(m_vsId);
	m_fsId = createShader(k_szFsSource, GL_FRAGMENT_SHADER); assert(m_fsId);
	m_progObjId = createProg(m_vsId, m_fsId); assert(m_progObjId);

	linkProg(m_progObjId);

	m_positionSlot = glGetAttribLocation(m_progObjId, "a_position");
	m_texCoordSlot = glGetAttribLocation(m_progObjId, "a_texCoord");
	m_colorSlot = glGetAttribLocation(m_progObjId, "a_color");
	m_samplerSlot = glGetUniformLocation(m_progObjId, "u_sampler");

	glUseProgram(m_progObjId);

	glEnableVertexAttribArray(m_positionSlot);
	glEnableVertexAttribArray(m_texCoordSlot);
	glEnableVertexAttribArray(m_colorSlot);

	//
	m_isInitOk = true;

	m_trianglesCount = 0;
	m_currentTexId = 0;

	setColor(0x000000FF);

	m_xScale = 1.0f;
	m_yScale = 1.0f;

	// Create dummy texture
	glGenTextures(1, &m_dummyTexId);
	glBindTexture(GL_TEXTURE_2D, m_dummyTexId);

	const int k_size = 8;
	unsigned char pDummyBuffer[k_size * k_size * 4];
	memset(pDummyBuffer, 0, sizeof(pDummyBuffer));

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, k_size, k_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, pDummyBuffer);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
	glBindTexture(GL_TEXTURE_2D, 0);

	//
	return true;
}

void Gles20Graphic::resize(int width, int height)
{
	assert(m_isInitOk);
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
	resetClip();
}

void Gles20Graphic::uninit()
{
	assert(m_isInitOk);

	glDeleteTextures(1, &m_dummyTexId);

	glDeleteShader(m_vsId);
	glDeleteShader(m_fsId);
	glDeleteProgram(m_progObjId);

#ifdef WIN32
	eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface(m_eglDisplay, m_eglSurface);
	eglDestroyContext(m_eglDisplay, m_eglContext);
	eglTerminate(m_eglDisplay);
	eglReleaseThread();
#endif

	m_isInitOk = false;
}

void Gles20Graphic::setClearColor(int rgba)
{
	glClearColor(
		( (rgba >>	24)	& 0xFF ) / 255.0f,
		( (rgba >>	16)	& 0xFF ) / 255.0f,
		( (rgba >>	8)	& 0xFF ) / 255.0f,
		( rgba & 0xFF ) / 255.0f
	);
}

void Gles20Graphic::clear()
{
	assert(m_isInitOk);

	glClear(GL_COLOR_BUFFER_BIT);
}

void Gles20Graphic::sendToGpu()
{
	assert(m_isInitOk);

	if (m_trianglesCount > 0)
	{
		glVertexAttribPointer(m_positionSlot, 2, GL_FLOAT, GL_FALSE, 0, m_pVerticesList);
		glVertexAttribPointer(m_texCoordSlot, 2, GL_FLOAT, GL_FALSE, 0, m_pTexCoordsList);
		glVertexAttribPointer(m_colorSlot, 4, GL_FLOAT, GL_FALSE, 0, m_pColorsList);

		glBindTexture(GL_TEXTURE_2D, m_currentTexId);

		glUniform1i(m_samplerSlot, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3 * m_trianglesCount);
	}

	m_trianglesCount = 0;
	m_currentTexId = 0;
}

void Gles20Graphic::flush()
{
	assert(m_isInitOk);
	sendToGpu();
#ifdef WIN32
	eglSwapBuffers(m_eglDisplay, m_eglSurface);
#endif
}

void Gles20Graphic::setClip(int x, int y, int w, int h)
{
	assert(m_isInitOk);

	glScissor(x, m_height - y - h, w, h);
	m_xClip = x; m_yClip = y; m_wClip = w; m_hClip = h;
}

void Gles20Graphic::resetClip()
{
	assert(m_isInitOk);

	setClip(0, 0, m_width, m_height);
}

void Gles20Graphic::fillRect(int x, int y, int w, int h)
{
	assert(m_isInitOk);

	x = (int)(x * m_xScale);
	y = (int)(y * m_yScale);
	w = (int)(w * m_xScale);
	h = (int)(h * m_yScale);

	assert(m_isInitOk);
	assert(m_trianglesCount <= MAX_TRIANGLES_NUMBER - 2);

	if (m_trianglesCount > 0 && m_currentTexId != m_dummyTexId)
	{
		sendToGpu();
	}

	m_currentTexId = m_dummyTexId;

	GLfloat xEs, yEs, wEs, hEs;
	convertPos(x, y, xEs, yEs);
	convertSize(w, h, wEs, hEs);

	//
	int i = 3 * 2 * m_trianglesCount;

	m_pVerticesList[i++] = xEs;				m_pVerticesList[i++] = yEs;
	m_pVerticesList[i++] = xEs;				m_pVerticesList[i++] = yEs - hEs;
	m_pVerticesList[i++] = xEs + wEs;		m_pVerticesList[i++] = yEs - hEs;

	m_pVerticesList[i++] = xEs;				m_pVerticesList[i++] = yEs;
	m_pVerticesList[i++] = xEs + wEs;		m_pVerticesList[i++] = yEs - hEs;
	m_pVerticesList[i++] = xEs + wEs;		m_pVerticesList[i++] = yEs;

	//
	i = 3 * 2 * m_trianglesCount;

	m_pTexCoordsList[i++] = 0.0f;			m_pTexCoordsList[i++] = 1.0f;
	m_pTexCoordsList[i++] = 0.0f;			m_pTexCoordsList[i++] = 0.0f;
	m_pTexCoordsList[i++] = 1.0f;			m_pTexCoordsList[i++] = 0.0f;

	m_pTexCoordsList[i++] = 0.0f;			m_pTexCoordsList[i++] = 1.0f;
	m_pTexCoordsList[i++] = 1.0f;			m_pTexCoordsList[i++] = 0.0f;
	m_pTexCoordsList[i++] = 1.0f;			m_pTexCoordsList[i++] = 1.0f;

	//
	i = 3 * 4 * m_trianglesCount;

	m_pColorsList[i++] = m_r; m_pColorsList[i++] = m_g; m_pColorsList[i++] = m_b; m_pColorsList[i++] = m_a;
	m_pColorsList[i++] = m_r; m_pColorsList[i++] = m_g; m_pColorsList[i++] = m_b; m_pColorsList[i++] = m_a;
	m_pColorsList[i++] = m_r; m_pColorsList[i++] = m_g; m_pColorsList[i++] = m_b; m_pColorsList[i++] = m_a;
	m_pColorsList[i++] = m_r; m_pColorsList[i++] = m_g; m_pColorsList[i++] = m_b; m_pColorsList[i++] = m_a;
	m_pColorsList[i++] = m_r; m_pColorsList[i++] = m_g; m_pColorsList[i++] = m_b; m_pColorsList[i++] = m_a;
	m_pColorsList[i++] = m_r; m_pColorsList[i++] = m_g; m_pColorsList[i++] = m_b; m_pColorsList[i++] = m_a;

	//
	m_trianglesCount += 2;
}

void Gles20Graphic::setColor(int rgba)
{
	assert(m_isInitOk);
	m_r = ( (rgba >>	24)	& 0xFF ) / 255.0f;
	m_g = ( (rgba >>	16)	& 0xFF ) / 255.0f;
	m_b = ( (rgba >>	8)	& 0xFF ) / 255.0f;
	m_a = ( rgba & 0xFF ) / 255.0f;
}

void Gles20Graphic::drawImage(
	Image* p,
	int xDst, int yDst,
	int xSrc, int ySrc, int wSrc, int hSrc,
	float xScale, float yScale,
	Transform* pTransform)
{
	assert(m_isInitOk);

	float xDst_float = (xDst * m_xScale);
	float yDst_float = (yDst * m_yScale);

	xScale *= m_xScale;
	yScale *= m_yScale;

	assert(m_isInitOk);
	assert(m_trianglesCount <= MAX_TRIANGLES_NUMBER - 2);
	assert(p->isLoaded());

	void* v = p->getHandle();
	GLuint* pTexId = (GLuint*)v;
	GLuint texId = *pTexId;
	assert(texId > 0);

	if (m_trianglesCount > 0 && m_currentTexId != texId)
	{
		sendToGpu();
	}

	m_currentTexId = texId;

	GLfloat pVertexData[] =
	{
		(GLfloat)xDst_float,			(GLfloat)(m_height - yDst_float),
		(GLfloat)xDst_float,			(GLfloat)(m_height - yDst_float - hSrc * yScale),

		(GLfloat)(xDst_float + wSrc * xScale),	(GLfloat)(m_height - yDst_float),
		(GLfloat)(xDst_float + wSrc * xScale),	(GLfloat)(m_height - yDst_float - hSrc * yScale),
	};

	const GLfloat imageWidth = (GLfloat)p->width();
	const GLfloat imageHeight = (GLfloat)p->height();

	const GLfloat k_pTexCoordBase[] =
	{
		(GLfloat)(xSrc)							/ imageWidth,		// 0
		(GLfloat)(imageHeight - ySrc)			/ imageHeight,		// 1
		(GLfloat)(xSrc + wSrc)					/ imageWidth,		// 2
		(GLfloat)(imageHeight - ySrc - hSrc)	/ imageHeight,		// 3
	};

	GLfloat pTexCoordData[] =
	{
		k_pTexCoordBase[0],	k_pTexCoordBase[1],
		k_pTexCoordBase[0],	k_pTexCoordBase[3],
		k_pTexCoordBase[2],	k_pTexCoordBase[1],
		k_pTexCoordBase[2],	k_pTexCoordBase[3],
	};

	applyTransform(pVertexData, pTexCoordData, pTransform);

	pVertexData[0] = 2.0f * pVertexData[0] / (GLfloat)m_width - 1.0f;
	pVertexData[1] = 2.0f * pVertexData[1] / (GLfloat)m_height - 1.0f;

	pVertexData[2] = 2.0f * pVertexData[2] / (GLfloat)m_width - 1.0f;
	pVertexData[3] = 2.0f * pVertexData[3] / (GLfloat)m_height - 1.0f;

	pVertexData[4] = 2.0f * pVertexData[4] / (GLfloat)m_width - 1.0f;
	pVertexData[5] = 2.0f * pVertexData[5] / (GLfloat)m_height - 1.0f;

	pVertexData[6] = 2.0f * pVertexData[6] / (GLfloat)m_width - 1.0f;
	pVertexData[7] = 2.0f * pVertexData[7] / (GLfloat)m_height - 1.0f;

	//
	int i = 3 * 2 * m_trianglesCount;

	m_pVerticesList[i++] = pVertexData[0 * 2];		m_pVerticesList[i++] = pVertexData[0 * 2 + 1];
	m_pVerticesList[i++] = pVertexData[1 * 2];		m_pVerticesList[i++] = pVertexData[1 * 2 + 1];
	m_pVerticesList[i++] = pVertexData[3 * 2];		m_pVerticesList[i++] = pVertexData[3 * 2 + 1];

	m_pVerticesList[i++] = pVertexData[0 * 2];		m_pVerticesList[i++] = pVertexData[0 * 2 + 1];
	m_pVerticesList[i++] = pVertexData[3 * 2];		m_pVerticesList[i++] = pVertexData[3 * 2 + 1];
	m_pVerticesList[i++] = pVertexData[2 * 2];		m_pVerticesList[i++] = pVertexData[2 * 2 + 1];

	//
	i = 3 * 2 * m_trianglesCount;

	m_pTexCoordsList[i++] = pTexCoordData[0 * 2];			m_pTexCoordsList[i++] = pTexCoordData[0 * 2 + 1];
	m_pTexCoordsList[i++] = pTexCoordData[1 * 2];			m_pTexCoordsList[i++] = pTexCoordData[1 * 2 + 1];
	m_pTexCoordsList[i++] = pTexCoordData[3 * 2];			m_pTexCoordsList[i++] = pTexCoordData[3 * 2 + 1];

	m_pTexCoordsList[i++] = pTexCoordData[0 * 2];			m_pTexCoordsList[i++] = pTexCoordData[0 * 2 + 1];
	m_pTexCoordsList[i++] = pTexCoordData[3 * 2];			m_pTexCoordsList[i++] = pTexCoordData[3 * 2 + 1];
	m_pTexCoordsList[i++] = pTexCoordData[2 * 2];			m_pTexCoordsList[i++] = pTexCoordData[2 * 2 + 1];

	i = 3 * 4 * m_trianglesCount;
	for (int j = i; j < i + 24; j++)
	{
		m_pColorsList[j] = 0.0f;
	}
	
	//
	m_trianglesCount += 2;
}

void Gles20Graphic::drawImage(
		Image* p,
		int xDst, int yDst,
		float xScale, float yScale,
		Transform* pTransform)
{
	assert(m_isInitOk);
	drawImage(p, xDst, yDst, 0, 0, p->width(), p->height(), xScale, yScale, pTransform);
}

void Gles20Graphic::applyTransform(GLfloat* pVertex, GLfloat* pTexCoord, Transform* p)
{
	/*************

	0|    /|2
	 |   / |
	 |  /  |
	 | /   |
	1|/    |3

	*************/
	if (!p) return;
	int count = p->getCount();
	TransformType* list = p->getList();

	for (int i = 0; i < count; i++)
	{
		switch (list[i])
		{
			case ROTATE_90:
			{
				GLfloat backup0u = pTexCoord[0];
				GLfloat backup0v = pTexCoord[1];

				pTexCoord[0] = pTexCoord[1 * 2]; pTexCoord[1] = pTexCoord[1 * 2 + 1];
				pTexCoord[1 * 2] = pTexCoord[3 * 2]; pTexCoord[1 * 2 + 1] = pTexCoord[3 * 2 + 1];
				pTexCoord[3 * 2] = pTexCoord[2 * 2]; pTexCoord[3 * 2 + 1] = pTexCoord[2 * 2 + 1];

				pTexCoord[2 * 2] = backup0u;
				pTexCoord[2 * 2 + 1] = backup0v;
						
				GLfloat width = pVertex[2 * 2] - pVertex[0];
				GLfloat height = pVertex[1] - pVertex[1 * 2 + 1];
				GLfloat diff = height - width;
						
				pVertex[1 * 2 + 1] += diff;
				pVertex[3 * 2] += diff;
				pVertex[2 * 2] = pVertex[3 * 2];
				pVertex[3 * 2 + 1] = pVertex[1 * 2 + 1];
			}
			break;

			case ROTATE_180:
			{
				GLfloat tmp;

				tmp = pTexCoord[0]; pTexCoord[0] = pTexCoord[3 * 2]; pTexCoord[3 * 2] = tmp;
				tmp = pTexCoord[1]; pTexCoord[1] = pTexCoord[3 * 2 + 1]; pTexCoord[3 * 2 + 1] = tmp;

				tmp = pTexCoord[1 * 2]; pTexCoord[1 * 2] = pTexCoord[2 * 2]; pTexCoord[2 * 2] = tmp;
				tmp = pTexCoord[1 * 2 + 1]; pTexCoord[1 * 2 + 1] = pTexCoord[2 * 2 + 1]; pTexCoord[2 * 2 + 1] = tmp;
			}
			break;

			case ROTATE_270:
			{
				GLfloat backup0u = pTexCoord[0];
				GLfloat backup0v = pTexCoord[1];

				pTexCoord[0] = pTexCoord[2 * 2]; pTexCoord[1] = pTexCoord[2 * 2 + 1];
				pTexCoord[2 * 2] = pTexCoord[3 * 2]; pTexCoord[2 * 2 + 1] = pTexCoord[3 * 2 + 1];
				pTexCoord[3 * 2] = pTexCoord[1 * 2]; pTexCoord[3 * 2 + 1] = pTexCoord[1 * 2 + 1];

				pTexCoord[1 * 2] = backup0u;
				pTexCoord[1 * 2 + 1] = backup0v;
						
				GLfloat width = pVertex[2 * 2] - pVertex[0];
				GLfloat height = pVertex[1] - pVertex[1 * 2 + 1];
				GLfloat diff = height - width;
						
				pVertex[1 * 2 + 1] += diff;
				pVertex[3 * 2] += diff;
				pVertex[2 * 2] = pVertex[3 * 2];
				pVertex[3 * 2 + 1] = pVertex[1 * 2 + 1];
			}
			break;

			case FLIP_X:
			{
				GLfloat tmp;

				tmp = pTexCoord[0]; pTexCoord[0] = pTexCoord[2 * 2]; pTexCoord[2 * 2] = tmp;
				tmp = pTexCoord[1]; pTexCoord[1] = pTexCoord[2 * 2 + 1]; pTexCoord[2 * 2 + 1] = tmp;

				tmp = pTexCoord[1 * 2]; pTexCoord[1 * 2] = pTexCoord[3 * 2]; pTexCoord[3 * 2] = tmp;
				tmp = pTexCoord[1 * 2 + 1]; pTexCoord[1 * 2 + 1] = pTexCoord[3 * 2 + 1]; pTexCoord[3 * 2 + 1] = tmp;
			}
			break;

			case FLIP_Y:
			{
				GLfloat tmp;

				tmp = pTexCoord[0]; pTexCoord[0] = pTexCoord[1 * 2]; pTexCoord[1 * 2] = tmp;
				tmp = pTexCoord[1]; pTexCoord[1] = pTexCoord[1 * 2 + 1]; pTexCoord[1 * 2 + 1] = tmp;

				tmp = pTexCoord[2 * 2]; pTexCoord[2 * 2] = pTexCoord[3 * 2]; pTexCoord[3 * 2] = tmp;
				tmp = pTexCoord[2 * 2 + 1]; pTexCoord[2 * 2 + 1] = pTexCoord[3 * 2 + 1]; pTexCoord[3 * 2 + 1] = tmp;
			}
			break;
		}
	}

}

GLuint Gles20Graphic::createShader(const char* szSource, GLenum type)
{
	GLuint shaderId = glCreateShader(type);
	if (!shaderId) return 0;

	glShaderSource(shaderId, 1, &szSource, NULL);

	glCompileShader(shaderId);

	GLint compileStatus = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus)
	{
		LOGE("Error compile %s shader [id = %d]: ", type == GL_VERTEX_SHADER ? "vertex" : "fragment", shaderId);
		
		GLint infoLen = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 0)
		{
			char* buffer = new char[infoLen + 1];
			glGetShaderInfoLog(shaderId, infoLen, &infoLen, buffer);
			buffer[infoLen] = 0;
			LOGE("%s", buffer);
			SAFE_DEL_ARRAY(buffer);
		}
		else
		{
			LOGE("Unkown error");
		}

		return 0;
	}

	return shaderId;
}

GLuint Gles20Graphic::createProg(GLuint vertexShaderId, GLuint fragmentShaderId)
{
	int progId = glCreateProgram();
	if (!progId) return 0;

	glAttachShader(progId, vertexShaderId);
	glAttachShader(progId, fragmentShaderId);

	return progId;
}

bool Gles20Graphic::linkProg(GLuint progId)
{
	glLinkProgram(progId);

	GLint linkStatus;
	glGetProgramiv(progId, GL_LINK_STATUS, &linkStatus);

	if (!linkStatus)
	{
		GLint infoLen = 0;
		glGetProgramiv(progId, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 0)
		{
			char* buffer = new char[infoLen + 1];
			glGetProgramInfoLog(progId, infoLen, NULL, buffer);
			buffer[infoLen] = 0;
			LOGE("Link program [id = %d] error: %s", progId, buffer);
			SAFE_DEL_ARRAY(buffer);
		}
		return false;
	}

	return true;
}

void Gles20Graphic::convertPos(int x, int y, GLfloat& xEs, GLfloat& yEs)
{
	y = m_height - y;

	xEs = 2.0f * (GLfloat)x / (GLfloat)m_width - 1.0f;
	yEs = 2.0f * (GLfloat)y / (GLfloat)m_height - 1.0f;
}

void Gles20Graphic::convertSize(int w, int h, GLfloat& wEs, GLfloat& hEs)
{
	wEs = 2.0f * (GLfloat)w / (GLfloat)m_width;
	hEs = 2.0f * (GLfloat)h / (GLfloat)m_height;
}

void Gles20Graphic::printGLString(const char* name, GLenum s)
{
    const char* v = (const char*)glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}
//===============================================================================================================
	}
}
