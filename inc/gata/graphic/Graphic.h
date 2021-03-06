//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : Graphic.h
//  @ Date : 6/15/2012
//  @ Author : 
//
//


#if !defined(_GRAPHIC_H)
#define _GRAPHIC_H

#include "GraphicMain.h"
#include "Factory.h"

namespace gata {
	namespace graphic {
//====================================================================================================================
class Image;
class Transform;

class Graphic
{
public:
	Graphic() : m_isInitOk(false) { }
	virtual ~Graphic() { }

	bool isInit() { return m_isInitOk; }

	virtual bool init(void* pContext) = 0;
	virtual void resize(int width, int height) = 0;
	virtual void uninit() = 0;

	virtual void setClearColor(int rgba) = 0;

	virtual void clear() = 0;
	virtual void flush() = 0;

	virtual void setClip(int x, int y, int w, int h) = 0;
	void getClip(int &x, int &y, int &w, int &h) { x = m_xClip; y = m_yClip; w = m_wClip; h = m_hClip; }

	void setScale(float xScale, float yScale) { m_xScale = xScale; m_yScale = yScale; }
	void getScale(float& xScale, float& yScale) { xScale = m_xScale; yScale = m_yScale; }
			
	virtual void resetClip() = 0;

	virtual void fillRect(int x, int y, int w, int h) = 0;

	virtual void setColor(int rgba) = 0;

	virtual void drawImage(
		Image* p,
		int xDst, int yDst,
		int xSrc, int ySrc, int wSrc, int hSrc,
		float xScale = 1.0f, float yScale = 1.0f,
		Transform* pTransform = 0) = 0;

	virtual void drawImage(
		Image* p,
		int xDst, int yDst,
		float xScale = 1.0f, float yScale = 1.0f,
		Transform* pTransform = 0) = 0;

	static Graphic* create()
	{
		return GraphicMain::getFactory()->createGraphic();
	}

protected:
	bool m_isInitOk;

	int m_xClip, m_yClip, m_wClip, m_hClip;

	float m_xScale;
	float m_yScale;
};
//====================================================================================================================
	}
}

#endif  //_GRAPHIC_H
