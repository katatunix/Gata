//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : Sprite.h
//  @ Date : 6/15/2012
//  @ Author : 
//
//

#if !defined(_SPRITE_H)
#define _SPRITE_H

#include "../core/macro.h"
#include "../core/Resource.h"
#include "../core/ResManager.h"

#include "../graphic/Transform.h"
#include "../graphic/Graphic.h"
#include "../graphic/Image.h"

namespace gata {
	namespace sprite {
//================================================================================================================
using namespace gata::core;
using namespace gata::graphic;

typedef struct _Module
{
	_Module(): x(0), y(0), w(0), h(0) { }
	_Module(int _x, int _y, int _w, int _h): x(_x), y(_y), w(_w), h(_h) { }

	int x, y, w, h;
} Module;

typedef struct _FModule
{
	_FModule() : m_moduleIndex(-1), m_transform(Transform()), m_xInFrame(0), m_yInFrame(0) { }
	_FModule(int moduleIndex, Transform transform, int xInFrame, int yInFrame) :
			m_moduleIndex(moduleIndex),
			m_transform(transform),
			m_xInFrame(xInFrame),
			m_yInFrame(yInFrame) { }

	int m_moduleIndex;
	Transform m_transform;
	int m_xInFrame;
	int m_yInFrame;
} FModule;

typedef struct _Frame
{
	_Frame() : m_pFModulesList(0), m_fmodulesNumber(0) { }
	_Frame(FModule* pFModulesList, int fmodulesNumber) : m_pFModulesList(pFModulesList), m_fmodulesNumber(fmodulesNumber) { }
	~_Frame()
	{
		SAFE_DEL_ARRAY(m_pFModulesList);
	}

	FModule* m_pFModulesList;
	int m_fmodulesNumber;
} Frame;

typedef struct _AFrame
{
	_AFrame() : m_frameIndex(-1), m_xInAnim(0), m_yInAnim(0) { }
	_AFrame(int frameIndex, Transform transform, int xInAnim, int yInAnim) :
			m_frameIndex(frameIndex),
			m_xInAnim(xInAnim),
			m_yInAnim(yInAnim) { }

	int m_frameIndex;
	int m_xInAnim;
	int m_yInAnim;
} AFrame;

enum AnimLoopType
{
	NORMAL = 0,
	CIRCULAR = 1,
	PINGPONG = 2
};

typedef struct _Anim
{
	_Anim() : m_pAFramesList(0), m_aframesNumber(0), m_loopType(NORMAL), m_delayNumber(0) { }

	~_Anim()
	{
		SAFE_DEL_ARRAY(m_pAFramesList);
	}

	AFrame* m_pAFramesList;
	int m_aframesNumber;
	AnimLoopType m_loopType;
	int m_delayNumber;
} Anim;

class MAnim
{
public:
	MAnim() : m_pAnim(0), m_delayCount(0), m_currentAFrameIndex(0), m_isForward(true), m_isRunning(false) { }

	void setAnim(Anim* p);
	void reset();
	bool isRunning();
	void update();

	Anim* m_pAnim;
	int m_delayCount;
	int m_currentAFrameIndex;
	bool m_isForward;

	bool m_isRunning;
};

//=======================================================================================================
// Sprite
#define MAX_MANIM_NUMBER 256
#define MAX_WRAP_BUFFER_LENGTH 2048

#define ANCHOR_LEFT			1
#define ANCHOR_RIGHT		2
#define ANCHOR_HCENTER		4
#define ANCHOR_VCENTER		8
#define ANCHOR_TOP			16
#define ANCHOR_BOTTOM		32

enum TextAlignStyle
{
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_CENTER,
	ALIGN_JUSTIFY
};

class Sprite : public Resource
{
public:
	Sprite();
	virtual ~Sprite();

	// From Resource
	bool load(const char* szName);
	void unload();

	void* getHandle();

	//
	void paintFrame(Graphic* g, int frameIndex, int x, int y, Transform* pTransform = 0);
	void paintScaleFrame(Graphic* g, int frameIndex, int x, int y,
		float xScale = 1.0f, float yScale = 1.0f, Transform* pTransform = 0);

	//
	int mapAnim(int animIndex);
	void unmapMAnim(int manimIndex);
	void resetMAnim(int manimIndex);

	void paintAndUpdateMAnim(Graphic* g, int manimIndex, int x, int y);
	void paintScaleAndUpdateMAnim(Graphic* g, int manimIndex, int x, int y, float xScale = 1.0f, float yScale = 1.0f);

	void paintMAnim(Graphic* g, int manimIndex, int x, int y);
	void paintScaleMAnim(Graphic* g, int manimIndex, int x, int y, float xScale = 1.0f, float yScale = 1.0f);

	void updateMAnim(int manimIndex);

	//
	void paintModule(Graphic* g, int moduleIndex, int x, int y, Transform* pTransform = 0);
	void paintScaleModule(Graphic* g, int moduleIndex, int x, int y,
		float xScale = 1.0f, float yScale = 1.0f, Transform* pTransform = 0);

	// Font
	void drawString(Graphic* g, const char* pString, int x, int y,
		int anchors = ANCHOR_LEFT | ANCHOR_TOP, int justifyWidth = 0, float xScale = 1.0f, float yScale = 1.0f);
	void drawString(Graphic* g, const unsigned short* pString, int x, int y,
		int anchors = ANCHOR_LEFT | ANCHOR_TOP, int justifyWidth = 0, float xScale = 1.0f, float yScale = 1.0f);

	void drawWrapString(Graphic* g, const char* pString, int x, int y, int width,
		int anchors = ANCHOR_LEFT | ANCHOR_TOP, TextAlignStyle align = ALIGN_LEFT, float xScale = 1.0f, float yScale = 1.0f);
	void drawWrapString(Graphic* g, const unsigned short* pString, int x, int y, int width,
		int anchors = ANCHOR_LEFT | ANCHOR_TOP, TextAlignStyle align = ALIGN_LEFT, float xScale = 1.0f, float yScale = 1.0f);

	//
	int getFrameWidth(int frameIndex);
	int getFrameHeight(int frameIndex);

	//
	void setImage(Image* i) { m_pImage = i; }

	//
	static Sprite* create() { return new Sprite(); }

private:
	//
	void transformRect(int& x, int& y, int w, int h, Transform* pTransform);
	TransformType convertTransformType(const char* szTrans);

	// Font
	int searchCharIndex(char ch);
	int searchCharIndex(unsigned short ch);
	
	void calcuStringRect(const char* pString, int left, int right, int& x, int& y, int& width, int& height);
	void calcuStringRect(const unsigned short* pString, int left, int right, int& x, int& y, int& width, int& height);

	void wrapString(char* sz, int width, float xScale);
	void wrapString(unsigned short* sz, int width, float xScale);

	//
	// Variable members:
	//

	Module* m_pModulesList;
	int m_modulesNumber;

	Frame* m_pFramesList;
	int m_framesNumber;

	Anim* m_pAnimsList;
	int m_animsNumber;

	MAnim m_pMAnimsList[MAX_MANIM_NUMBER];

	//
	Image* m_pImage;

	bool m_isLoaded;

	// Font
	unsigned short* m_pCharMap;
	int m_charMapLength;

	int m_charHorizonBetween;
	int m_lineHeight;
	int m_spaceWidth;

	char m_szWrapBuffer8bit[MAX_WRAP_BUFFER_LENGTH + 1];
	unsigned short m_szWrapBuffer16bit[MAX_WRAP_BUFFER_LENGTH + 1];
};
//================================================================================================================
	}
}

#endif  //_SPRITE_H