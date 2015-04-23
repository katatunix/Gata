//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : Sprite.cpp
//  @ Date : 6/15/2012
//  @ Author : 
//
//

#include <stdio.h>

#include <gata/sprite/Sprite.h>
#include <gata/utils/UtfString.h>
#include <gata/utils/MyUtils.h>

#include <gata/3rdparty/tinyxml/tinyxml.h>

namespace gata {
	namespace sprite {
//==============================================================================================================
using namespace gata::graphic;
using namespace gata::utils;

// MAnim
void MAnim::setAnim(Anim* p)
{
	reset();
	m_pAnim = p;
}

void MAnim::reset()
{
	m_delayCount = 0;
	m_currentAFrameIndex = 0;
	m_isForward = true;
	m_isRunning = true;
}

bool MAnim::isRunning()
{
	return m_isRunning;
}

void MAnim::update()
{
	assert(m_pAnim);
	if (!isRunning()) return;

	m_delayCount++;
	if (m_delayCount < m_pAnim->m_delayNumber) return;

	m_delayCount = 0;

	switch (m_pAnim->m_loopType)
	{
		case NORMAL:
		{
			if (m_currentAFrameIndex >= m_pAnim->m_aframesNumber - 1)
			{
				m_isRunning = false;
			}
			else
			{
				m_currentAFrameIndex++;
			}
		}
		break;
		case PINGPONG:
		{
			if (m_pAnim->m_aframesNumber >= 2)
			{
				if (m_isForward)
				{
					if (m_currentAFrameIndex >= m_pAnim->m_aframesNumber - 1)
					{
						m_currentAFrameIndex = m_pAnim->m_aframesNumber - 2;
						m_isForward = false;
					}
					else
					{
						m_currentAFrameIndex++;
					}
				}
				else
				{
					if (m_currentAFrameIndex <= 0)
					{
						m_currentAFrameIndex = 1;
						m_isForward = true;
					}
					else
					{
						m_currentAFrameIndex--;
					}
				}
			}
		}
		break;
		case CIRCULAR:
		{
			if (m_currentAFrameIndex >= m_pAnim->m_aframesNumber - 1)
			{
				m_currentAFrameIndex = 0;
			}
			else
			{
				m_currentAFrameIndex++;
			}
		}
		break;
	}
}

// =======================================================================
// Sprite
Sprite::Sprite() :
		m_pModulesList(0),
		m_pFramesList(0),
		m_pAnimsList(0),

		m_modulesNumber(0),
		m_framesNumber(0),
		m_animsNumber(0),

		m_pImage(0),
		m_isLoaded(false),

		m_pCharMap(0)
{

}

Sprite::~Sprite()
{
	unload();
}

bool Sprite::load(const char* szName)
{
	unload();

	int length;
	FILE* f = my_fopen(szName, &length);
	
	assert(f);
			
	TiXmlDocument doc;
	if (!doc.LoadFile(f))
	{
		LOGI("ERROR: fail to load: [%s]", szName);
		assert(0);
		return false;
	}
	TiXmlElement* root = doc.FirstChildElement("sprite");
			
	// Load Module -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------------------
	TiXmlElement* module = root->FirstChildElement("module");
	m_modulesNumber = 0;
	while (module)
	{
		if (module->Attribute("isset"))
		{
			m_modulesNumber += atoi(module->Attribute("cols")) * atoi(module->Attribute("rows"));
		}
		else
		{
			m_modulesNumber++;
		}
		module = module->NextSiblingElement("module");
	}
	m_pModulesList = new Module[m_modulesNumber];

	module = root->FirstChildElement("module");
	int i = 0;
	while (module)
	{
		if (module->Attribute("isset"))
		{
			int x = atoi(module->Attribute("x"));
			int y = atoi(module->Attribute("y"));
			int w = atoi(module->Attribute("w"));
			int h = atoi(module->Attribute("h"));
			int cols = atoi(module->Attribute("cols"));
			int rows = atoi(module->Attribute("rows"));
			int spacex = atoi(module->Attribute("spacex"));
			int spacey = atoi(module->Attribute("spacey"));

			for (int u = 0; u < rows; u++)
			{
				for (int v = 0; v < cols; v++)
				{
					m_pModulesList[i].x = x + (w + spacex) * v;
					m_pModulesList[i].y = y + (h + spacey) * u;
					m_pModulesList[i].w = w;
					m_pModulesList[i].h = h;
					i++;
				}
			}
		}
		else
		{
			m_pModulesList[i].x = atoi(module->Attribute("x"));
			m_pModulesList[i].y = atoi(module->Attribute("y"));
			m_pModulesList[i].w = atoi(module->Attribute("w"));
			m_pModulesList[i].h = atoi(module->Attribute("h"));
			i++;
		}
		module = module->NextSiblingElement("module");
	}

	// Load Frame --------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------------------
	TiXmlElement* frames = root->FirstChildElement("frames");
	TiXmlElement* frame = frames->FirstChildElement("frame");
	m_framesNumber = 0;
	while (frame)
	{
		if (frame->Attribute("isset"))
		{
			m_framesNumber += atoi(frame->Attribute("count"));
		}
		else
		{
			m_framesNumber++;
		}
		frame = frame->NextSiblingElement("frame");
	}

	m_pFramesList = new Frame[m_framesNumber];
	frame = frames->FirstChildElement("frame");
	i = 0;
	while (frame)
	{
		if (frame->Attribute("isset"))
		{
			int count = atoi(frame->Attribute("count"));
			int moduleIndexStart = atoi(frame->Attribute("moduleIndexStart"));
			int moduleIndexStep = atoi(frame->Attribute("moduleIndexStep"));
			int xInFrameStart = atoi(frame->Attribute("xInFrameStart"));
			int yInFrameStart = atoi(frame->Attribute("yInFrameStart"));
			int xInFrameStep = atoi(frame->Attribute("xInFrameStep"));
			int yInFrameStep = atoi(frame->Attribute("yInFrameStep"));

			// Load transform
			TiXmlElement* transform = frame->FirstChildElement("transform");
			Transform trans;
			while (transform)
			{
				trans.append( convertTransformType(transform->GetText()) );
				transform = transform->NextSiblingElement("transform");
			}

			for (int j = 0; j < count; j++)
			{
				Frame& fr = m_pFramesList[i];
				fr.m_fmodulesNumber = 1;
				fr.m_pFModulesList = new FModule[1];

				FModule& fmod = fr.m_pFModulesList[0];
				fmod.m_moduleIndex = moduleIndexStart + j * moduleIndexStep;
				fmod.m_xInFrame = xInFrameStart + j * xInFrameStep;
				fmod.m_yInFrame = yInFrameStart + j * yInFrameStep;
				fmod.m_transform = trans;
						
				i++;
			}
		}
		else
		{
			// Load FModule
			Frame& fr = m_pFramesList[i];
			fr.m_fmodulesNumber = 0;
			TiXmlElement* fmodule = frame->FirstChildElement("fmodule");
			while (fmodule)
			{
				if (fmodule->Attribute("isset"))
				{
					fr.m_fmodulesNumber += atoi(fmodule->Attribute("count"));
				}
				else
				{
					fr.m_fmodulesNumber++;
				}
				fmodule = fmodule->NextSiblingElement("fmodule");
			}

			fr.m_pFModulesList = new FModule[fr.m_fmodulesNumber];

			fmodule = frame->FirstChildElement("fmodule");
			int j = 0;
			while (fmodule)
			{
				if (fmodule->Attribute("isset"))
				{
					int count = atoi(fmodule->Attribute("count"));
					int moduleIndexStart = atoi(fmodule->Attribute("moduleIndexStart"));
					int moduleIndexStep = atoi(fmodule->Attribute("moduleIndexStep"));
					int xInFrameStart = atoi(fmodule->Attribute("xInFrameStart"));
					int yInFrameStart = atoi(fmodule->Attribute("yInFrameStart"));
					int xInFrameStep = atoi(fmodule->Attribute("xInFrameStep"));
					int yInFrameStep = atoi(fmodule->Attribute("yInFrameStep"));

					// Load transform
					Transform trans;
					TiXmlElement* transform = fmodule->FirstChildElement("transform");
					while (transform)
					{
						trans.append( convertTransformType(transform->GetText()) );
						transform = transform->NextSiblingElement("transform");
					}

					for (int k = 0; k < count; k++)
					{
						FModule& fmod = fr.m_pFModulesList[j];
						fmod.m_moduleIndex = moduleIndexStart + k * moduleIndexStep;
						fmod.m_xInFrame = xInFrameStart + k * xInFrameStep;
						fmod.m_yInFrame = yInFrameStart + k * yInFrameStep;
						fmod.m_transform = trans;
						j++;
					}
				}
				else
				{
					FModule& fmod = fr.m_pFModulesList[j];
					fmod.m_moduleIndex		= atoi(fmodule->Attribute("moduleIndex"));
					fmod.m_xInFrame			= atoi(fmodule->Attribute("xInFrame"));
					fmod.m_yInFrame			= atoi(fmodule->Attribute("yInFrame"));

					// Load transform
					TiXmlElement* transform = fmodule->FirstChildElement("transform");
					while (transform)
					{
						fmod.m_transform.append( convertTransformType(transform->GetText()) );
						transform = transform->NextSiblingElement("transform");
					}
					j++;
				}

				fmodule = fmodule->NextSiblingElement("fmodule");
			}

			i++;
		}

		frame = frame->NextSiblingElement("frame");
	}

	// Load Anim ---------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------------------
	TiXmlElement* anims = root->FirstChildElement("anims");
	if (anims)
	{
		TiXmlElement* anim = anims->FirstChildElement("anim");
		m_animsNumber = 0;
		while (anim)
		{
			m_animsNumber++;
			anim = anim->NextSiblingElement("anim");
		}

		if (m_animsNumber > 0)
		{
			m_pAnimsList = new Anim[m_animsNumber];

			anim = anims->FirstChildElement("anim");
			i = 0;
			while (anim)
			{
				Anim& an = m_pAnimsList[i];
				const char* szLoopType = anim->Attribute("loopType");
				if (!strcmp(szLoopType, "NORMAL"))
				{
					an.m_loopType = NORMAL;
				}
				else if (!strcmp(szLoopType, "PINGPONG"))
				{
					an.m_loopType = PINGPONG;
				}
				else if (!strcmp(szLoopType, "CIRCULAR"))
				{
					an.m_loopType = CIRCULAR;
				}
				else
				{
					assert(0);
				}

				an.m_delayNumber = atoi(anim->Attribute("delayNumber"));

				// Load AFrame
				an.m_aframesNumber = 0;
				TiXmlElement* aframe = anim->FirstChildElement("aframe");
				while (aframe)
				{
					if (aframe->Attribute("isset"))
					{
						an.m_aframesNumber += atoi(aframe->Attribute("count"));
					}
					else
					{
						an.m_aframesNumber++;
					}
					aframe = aframe->NextSiblingElement("aframe");
				}

				an.m_pAFramesList = new AFrame[an.m_aframesNumber];

				aframe = anim->FirstChildElement("aframe");
				int j = 0;
				while (aframe)
				{
					if (aframe->Attribute("isset"))
					{
						int count = atoi(aframe->Attribute("count"));
						int frameIndexStart = atoi(aframe->Attribute("frameIndexStart"));
						int frameIndexStep = atoi(aframe->Attribute("frameIndexStep"));
						int xInAnimStart = atoi(aframe->Attribute("xInAnimStart"));
						int yInAnimStart = atoi(aframe->Attribute("yInAnimStart"));
						int xInAnimStep = atoi(aframe->Attribute("xInAnimStep"));
						int yInAnimStep = atoi(aframe->Attribute("yInAnimStep"));
						for (int k = 0; k < count; k++)
						{
							AFrame& afr = an.m_pAFramesList[j];
							afr.m_frameIndex = frameIndexStart + k * frameIndexStep;
							afr.m_xInAnim = xInAnimStart + k * xInAnimStep;
							afr.m_yInAnim = yInAnimStart + k * yInAnimStep;
							j++;
						}
					}
					else
					{
						AFrame& afr = an.m_pAFramesList[j];
						afr.m_frameIndex = atoi(aframe->Attribute("frameIndex"));
						afr.m_xInAnim = atoi(aframe->Attribute("xInAnim"));
						afr.m_yInAnim = atoi(aframe->Attribute("yInAnim"));

						j++;
					}

					aframe = aframe->NextSiblingElement("aframe");
				}

				i++;
				anim = anim->NextSiblingElement("anim");
			}
		}
	}

	// -------------------------------------------------------------------------------------------
	// Load char map for font
	TiXmlElement* charmap = root->FirstChildElement("charmap");
	if (charmap)
	{
		m_charHorizonBetween = atoi(charmap->Attribute("charSpace"));
		m_lineHeight = atoi(charmap->Attribute("lineHeight"));
		m_spaceWidth = atoi(charmap->Attribute("spaceWidth"));

		TiXmlElement* achar = charmap->FirstChildElement("char");
		m_charMapLength = 0;
		while (achar)
		{
			m_charMapLength++;
			achar = achar->NextSiblingElement("char");
		}
				
		assert(m_charMapLength > 0);
		m_pCharMap = new unsigned short[m_charMapLength];
				
		achar = charmap->FirstChildElement("char");
		int i = 0;
		while (achar)
		{
			m_pCharMap[i] = (unsigned short)atoi( achar->GetText() );
			achar = achar->NextSiblingElement("char");
			i++;
		}
	}

	fclose(f);

	// -------------------------------------------------------------------------------------------
	m_isLoaded = true;
	return true;
}

void Sprite::unload()
{
	SAFE_DEL_ARRAY(m_pModulesList);
	SAFE_DEL_ARRAY(m_pFramesList);
	SAFE_DEL_ARRAY(m_pAnimsList);

	SAFE_DEL_ARRAY(m_pCharMap);

	m_modulesNumber = 0;
	m_framesNumber = 0;
	m_animsNumber = 0;

	m_isLoaded = false;
}

void* Sprite::getHandle()
{
	if (m_isLoaded) return (void*)this;
	return 0;
}

void Sprite::paintModule(Graphic* g, int moduleIndex, int x, int y, Transform* pTransform)
{
	paintScaleModule(g, moduleIndex, x, y, 1.0f, 1.0f, pTransform);
}

void Sprite::paintScaleModule(Graphic* g, int moduleIndex, int x, int y, float xScale, float yScale, Transform* pTransform)
{
	assert(g);
	assert(m_pImage);

	g->drawImage(m_pImage, x, y,
		m_pModulesList[moduleIndex].x, m_pModulesList[moduleIndex].y,
		m_pModulesList[moduleIndex].w, m_pModulesList[moduleIndex].h,
		xScale, yScale,
		pTransform);
}

void Sprite::paintFrame(Graphic* g, int frameIndex, int x, int y, Transform* pTransform)
{
	paintScaleFrame(g, frameIndex, x, y, 1.0f, 1.0f, pTransform);
}

void Sprite::paintScaleFrame(Graphic* g, int frameIndex, int x, int y, float xScale, float yScale, Transform* pTransform)
{
	assert(g);
	assert(m_pImage);

	Frame& frame = m_pFramesList[frameIndex];

	for (int i = 0; i < frame.m_fmodulesNumber; i++)
	{
		FModule& fmod = frame.m_pFModulesList[i];
		Module& mod = m_pModulesList[ fmod.m_moduleIndex ];

		int xInFrame = (int)(fmod.m_xInFrame * xScale);
		int yInFrame = (int)(fmod.m_yInFrame * yScale);

		int w = mod.w;
		int h = mod.h;
		bool isChangeOri = fmod.m_transform.isChangeOri();
		if (isChangeOri)
		{
			w = mod.h;
			h = mod.w;
		}
		w = (int)(w * xScale);
		h = (int)(h * yScale);

		transformRect(xInFrame, yInFrame, w, h, pTransform);

		Transform tf = fmod.m_transform;
		if (pTransform)
		{
			tf = Transform::concat(&fmod.m_transform, pTransform);
		}

		if (isChangeOri)
		{
			paintScaleModule(g, fmod.m_moduleIndex, x + xInFrame, y + yInFrame, yScale, xScale, &tf);
		}
		else
		{
			paintScaleModule(g, fmod.m_moduleIndex, x + xInFrame, y + yInFrame, xScale, yScale, &tf);
		}
	}
}

void Sprite::transformRect(int& x, int& y, int w, int h, Transform* pTransform)
{
	if (!pTransform) return;

	int count = pTransform->getCount();
	TransformType* plist = pTransform->getList();
			
	for (int i = 0; i < count; i++)
	{
		switch (plist[i])
		{
			case ROTATE_90:
			{
				int tmp = x;
				x = -y - h;
				y = tmp;
				tmp = w; w = h; h = tmp;
			}
			break;
			case ROTATE_180:
			{
				x = -x - w;
				y = -y - h;
			}
			break;
			case ROTATE_270:
			{
				int tmp = x;
				x = y;
				y = -tmp - w;
				tmp = w; w = h; h = tmp;
			}
			break;
			case FLIP_X:
			{
				x = -x - w;
			}
			break;
			case FLIP_Y:
			{
				y = -y - h;
			}
			break;
		}
	}
}

int Sprite::mapAnim(int animIndex)
{
	for (int i = 0; i < MAX_MANIM_NUMBER; i++)
	{
		if (!m_pMAnimsList[i].m_pAnim)
		{
			m_pMAnimsList[i].setAnim(&m_pAnimsList[animIndex]);
			return i;
		}
	}
	assert(0);
	return -1;
}

void Sprite::unmapMAnim(int manimIndex)
{
	m_pMAnimsList[manimIndex].setAnim(0);
}

void Sprite::resetMAnim(int manimIndex)
{
	MAnim& manim = m_pMAnimsList[manimIndex];
	manim.reset();
}

void Sprite::paintAndUpdateMAnim(Graphic* g, int manimIndex, int x, int y)
{
	MAnim& manim = m_pMAnimsList[manimIndex];
	Anim* pAnim = manim.m_pAnim;
	assert(pAnim);
	AFrame& currentAFrame = pAnim->m_pAFramesList[ manim.m_currentAFrameIndex ];
	paintFrame(g,
		currentAFrame.m_frameIndex,
		x + currentAFrame.m_xInAnim,
		y + currentAFrame.m_yInAnim);
	manim.update();
}

void Sprite::paintScaleAndUpdateMAnim(Graphic* g, int manimIndex, int x, int y, float xScale, float yScale)
{
	MAnim& manim = m_pMAnimsList[manimIndex];
	Anim* pAnim = manim.m_pAnim;
	assert(pAnim);
	AFrame& currentAFrame = pAnim->m_pAFramesList[ manim.m_currentAFrameIndex ];
	paintScaleFrame(g,
		currentAFrame.m_frameIndex,
		x + (int)(currentAFrame.m_xInAnim * xScale),
		y + (int)(currentAFrame.m_yInAnim * yScale),
		xScale, yScale);
	manim.update();
}

void Sprite::paintMAnim(Graphic* g, int manimIndex, int x, int y)
{
	MAnim& manim = m_pMAnimsList[manimIndex];
	Anim* pAnim = manim.m_pAnim;
	assert(pAnim);
	AFrame& currentAFrame = pAnim->m_pAFramesList[ manim.m_currentAFrameIndex ];
	paintFrame(g,
		currentAFrame.m_frameIndex,
		x + currentAFrame.m_xInAnim,
		y + currentAFrame.m_yInAnim);
}

void Sprite::paintScaleMAnim(Graphic* g, int manimIndex, int x, int y, float xScale, float yScale)
{
	MAnim& manim = m_pMAnimsList[manimIndex];
	Anim* pAnim = manim.m_pAnim;
	assert(pAnim);
	AFrame& currentAFrame = pAnim->m_pAFramesList[ manim.m_currentAFrameIndex ];
	paintScaleFrame(g,
		currentAFrame.m_frameIndex,
		x + (int)(currentAFrame.m_xInAnim * xScale),
		y + (int)(currentAFrame.m_yInAnim * yScale),
		xScale, yScale);
}

void Sprite::updateMAnim(int manimIndex)
{
	MAnim& manim = m_pMAnimsList[manimIndex];
	manim.update();
}

//
int Sprite::getFrameWidth(int frameIndex)
{
	Frame& frame = m_pFramesList[frameIndex];
	if (frame.m_fmodulesNumber <= 0) return 0;
	int minX = MY_MAX_INT;
	int maxX = MY_MIN_INT;
	for (int i  = 0; i < frame.m_fmodulesNumber; i++)
	{
		FModule& fmod = frame.m_pFModulesList[i];
		Module& mod = m_pModulesList[ fmod.m_moduleIndex ];

		if (minX > fmod.m_xInFrame) minX = fmod.m_xInFrame;
		if (maxX < fmod.m_xInFrame + mod.w) maxX = fmod.m_xInFrame + mod.w;
	}
	return maxX - minX;
}

int Sprite::getFrameHeight(int frameIndex)
{
	Frame& frame = m_pFramesList[frameIndex];
	if (frame.m_fmodulesNumber <= 0) return 0;
	int minY = MY_MAX_INT;
	int maxY = -minY;
	for (int i  = 0; i < frame.m_fmodulesNumber; i++)
	{
		FModule& fmod = frame.m_pFModulesList[i];
		Module& mod = m_pModulesList[ fmod.m_moduleIndex ];

		if (minY > fmod.m_yInFrame) minY = fmod.m_yInFrame;
		if (maxY < fmod.m_yInFrame + mod.h) maxY = fmod.m_yInFrame + mod.h;
	}
	return maxY - minY;
}

TransformType Sprite::convertTransformType(const char* szTrans)
{
	if (!strcmp(szTrans, "ROTATE_90"))
	{
		return ROTATE_90;
	}
	else if (!strcmp(szTrans, "ROTATE_180"))
	{
		return ROTATE_180;
	}
	else if (!strcmp(szTrans, "ROTATE_270"))
	{
		return ROTATE_270;
	}
	else if (!strcmp(szTrans, "FLIP_X"))
	{
		return FLIP_X;
	}
	else if (!strcmp(szTrans, "FLIP_Y"))
	{
		return FLIP_Y;
	}
	else
	{
		assert(0);
		return ROTATE_90;
	}
}

//--------------------------------------------------------------------------------------------------
// Font

void Sprite::drawString(Graphic* g, const char* pString, int x, int y, int anchors, int justifyWidth, float xScale, float yScale)
{
	int len = strlen(pString);
	if (len <= 0) return;

	int left = 0, right = len - 1;
	int ppp = 0, rrr = 0;
	bool bbb = true;

	if (justifyWidth > 0)
	{
		while (left <= len - 1 && pString[left] == ' ') left++;
		while (right >= 0 && pString[right] == ' ') right--;
	}

	int dx, dy, width, height;
	calcuStringRect(pString, left, right, dx, dy, width, height);
	dx = (int)(dx * xScale);
	dy = (int)(dy * yScale);
	width = (int)(width * xScale);
	height = (int)(height * yScale);

	x -= dx;
	y -= dy;

	if (justifyWidth > 0 && justifyWidth > width)
	{
		int count = 0;
		for (int i = left; i <= right; i++)
		{
			if (pString[i] == ' ') count++;
		}
		if (count > 0)
		{
			ppp = (justifyWidth - width) / count;
			rrr = justifyWidth - width - ppp * count + ppp;
		}
		width = justifyWidth;
	}

	if (anchors & ANCHOR_LEFT)			x = x;
	else if (anchors & ANCHOR_RIGHT)	x = x - width;
	else if (anchors & ANCHOR_HCENTER)	x = x - (width >> 1);
	else { assert(0); }

	if (anchors & ANCHOR_TOP)			y = y;
	else if (anchors & ANCHOR_BOTTOM)	y = y - height;
	else if (anchors & ANCHOR_VCENTER)	y = y - (height >> 1);
	else { assert(0); }

	float xx = (float)x;
	
	for (int i = left; i <= right; i++)
	{
		if (pString[i] == ' ')
		{
			xx += m_spaceWidth * xScale;
			if (bbb)
			{
				xx += rrr;
				bbb = false;
			}
			else
			{
				xx += ppp;
			}
		}
		else
		{
			int index = searchCharIndex(pString[i]);
			assert(index > -1);
			FModule& fmod = m_pFramesList[0].m_pFModulesList[index];
			paintScaleModule(g,
				fmod.m_moduleIndex,
				(int)(xx + fmod.m_xInFrame * xScale), y + (int)(fmod.m_yInFrame * yScale),
				xScale, yScale,
				&fmod.m_transform);

			xx += (fmod.m_xInFrame + m_pModulesList[ fmod.m_moduleIndex ].w + m_charHorizonBetween) * xScale;
		}
	}
}

void Sprite::drawString(Graphic* g, const unsigned short* pString, int x, int y, int anchors, int justifyWidth, float xScale, float yScale)
{
	int len = utf_strlen(pString);
	if (len <= 0) return;

	int left = 0, right = len - 1;
	int ppp = 0, rrr = 0;
	bool bbb = true;

	if (justifyWidth > 0)
	{
		while (left <= len - 1 && pString[left] == ' ') left++;
		while (right >= 0 && pString[right] == ' ') right--;
	}

	int dx, dy, width, height;
	calcuStringRect(pString, left, right, dx, dy, width, height);
	dx = (int)(dx * xScale);
	dy = (int)(dy * yScale);
	width = (int)(width * xScale);
	height = (int)(height * yScale);

	x -= dx;
	y -= dy;

	if (justifyWidth > 0 && justifyWidth > width)
	{
		int count = 0;
		for (int i = left; i <= right; i++)
		{
			if (pString[i] == ' ') count++;
		}
		if (count > 0)
		{
			ppp = (justifyWidth - width) / count;
			rrr = justifyWidth - width - ppp * count + ppp;
		}
		width = justifyWidth;
	}

	if (anchors & ANCHOR_LEFT)			x = x;
	else if (anchors & ANCHOR_RIGHT)	x = x - width;
	else if (anchors & ANCHOR_HCENTER)	x = x - (width >> 1);
	else { assert(0); }

	if (anchors & ANCHOR_TOP)			y = y;
	else if (anchors & ANCHOR_BOTTOM)	y = y - height;
	else if (anchors & ANCHOR_VCENTER)	y = y - (height >> 1);
	else { assert(0); }

	float xx = (float)x;
	
	for (int i = left; i <= right; i++)
	{
		if (pString[i] == ' ')
		{
			xx += m_spaceWidth * xScale;
			if (bbb)
			{
				xx += rrr;
				bbb = false;
			}
			else
			{
				xx += ppp;
			}
		}
		else
		{
			int index = searchCharIndex(pString[i]);
			assert(index > -1);
			FModule& fmod = m_pFramesList[0].m_pFModulesList[index];
			paintScaleModule(g,
				fmod.m_moduleIndex,
				(int)(xx + fmod.m_xInFrame * xScale), y + (int)(fmod.m_yInFrame * yScale),
				xScale, yScale,
				&fmod.m_transform);

			xx += (fmod.m_xInFrame + m_pModulesList[ fmod.m_moduleIndex ].w + m_charHorizonBetween) * xScale;
		}
	}
}

void Sprite::drawWrapString(Graphic* g, const char* pString, int x, int y, int width, int anchors, TextAlignStyle align, float xScale, float yScale)
{
	int len = strlen(pString);
	assert(len <= MAX_WRAP_BUFFER_LENGTH);
	strcpy(m_szWrapBuffer8bit, pString);
	wrapString(m_szWrapBuffer8bit, width, xScale);

	int anchorsLine;
	int justifyWidth = 0;
	switch (align)
	{
		case ALIGN_LEFT:
		{
			anchorsLine = ANCHOR_LEFT | ANCHOR_TOP;
		}
		break;
		case ALIGN_RIGHT:
		{
			anchorsLine = ANCHOR_RIGHT | ANCHOR_TOP;
			x += width;
		}
		break;
		case ALIGN_CENTER:
		{
			anchorsLine = ANCHOR_HCENTER | ANCHOR_TOP;
			x += width / 2;
		}
		break;
		case ALIGN_JUSTIFY:
		{
			anchorsLine = ANCHOR_LEFT | ANCHOR_TOP;
			justifyWidth = width;
		}
		break;
		default:
		{
			assert(0);
		}
	}

	// Count total height
	int count = 1;
	for (int i = 0; i < len; i++)
	{
		if (m_szWrapBuffer8bit[i] == '\n') count++;
	}

	int height = (int)(count * m_lineHeight * yScale);
	
	// Anchors
	if (anchors & ANCHOR_LEFT)			x = x;
	else if (anchors & ANCHOR_RIGHT)	x = x - width;
	else if (anchors & ANCHOR_HCENTER)	x = x - (width >> 1);
	else { assert(0); }

	if (anchors & ANCHOR_TOP)			y = y;
	else if (anchors & ANCHOR_BOTTOM)	y = y - height;
	else if (anchors & ANCHOR_VCENTER)	y = y - (height >> 1);
	else { assert(0); }

	// Draw
	char* p = m_szWrapBuffer8bit;
	count = 0;
	for (int i = 0; i < len; i++)
	{
		if (m_szWrapBuffer8bit[i] == '\n')
		{
			m_szWrapBuffer8bit[i] = 0;
			drawString(g, p, x, (int)(y + count * m_lineHeight * yScale), anchorsLine, justifyWidth, xScale, yScale);
			count++;
			p = m_szWrapBuffer8bit + i + 1;
		}
	}

	drawString(g, p, x, (int)(y + count * m_lineHeight * yScale), anchorsLine, justifyWidth, xScale, yScale);
}

void Sprite::drawWrapString(Graphic* g, const unsigned short* pString, int x, int y, int width, int anchors, TextAlignStyle align, float xScale, float yScale)
{
	int len = utf_strlen(pString);
	assert(len <= MAX_WRAP_BUFFER_LENGTH);
	utf_strcpy(m_szWrapBuffer16bit, pString);
	wrapString(m_szWrapBuffer16bit, width, xScale);

	int anchorsLine;
	int justifyWidth = 0;
	switch (align)
	{
		case ALIGN_LEFT:
		{
			anchorsLine = ANCHOR_LEFT | ANCHOR_TOP;
		}
		break;
		case ALIGN_RIGHT:
		{
			anchorsLine = ANCHOR_RIGHT | ANCHOR_TOP;
			x += width;
		}
		break;
		case ALIGN_CENTER:
		{
			anchorsLine = ANCHOR_HCENTER | ANCHOR_TOP;
			x += width / 2;
		}
		break;
		case ALIGN_JUSTIFY:
		{
			anchorsLine = ANCHOR_LEFT | ANCHOR_TOP;
			justifyWidth = width;
		}
		break;
		default:
		{
			assert(0);
		}
	}

	// Count total height
	int count = 1;
	for (int i = 0; i < len; i++)
	{
		if (m_szWrapBuffer16bit[i] == '\n') count++;
	}

	int height = (int)(count * m_lineHeight * yScale);
	
	// Anchors
	if (anchors & ANCHOR_LEFT)			x = x;
	else if (anchors & ANCHOR_RIGHT)	x = x - width;
	else if (anchors & ANCHOR_HCENTER)	x = x - (width >> 1);
	else { assert(0); }

	if (anchors & ANCHOR_TOP)			y = y;
	else if (anchors & ANCHOR_BOTTOM)	y = y - height;
	else if (anchors & ANCHOR_VCENTER)	y = y - (height >> 1);
	else { assert(0); }

	// Draw
	unsigned short* p = m_szWrapBuffer16bit;
	count = 0;
	for (int i = 0; i < len; i++)
	{
		if (m_szWrapBuffer16bit[i] == '\n')
		{
			m_szWrapBuffer16bit[i] = 0;
			drawString(g, p, x, (int)(y + count * m_lineHeight * yScale), anchorsLine, justifyWidth, xScale, yScale);
			count++;
			p = m_szWrapBuffer16bit + i + 1;
		}
	}

	drawString(g, p, x, (int)(y + count * m_lineHeight * yScale), anchorsLine, justifyWidth, xScale, yScale);
}

int Sprite::searchCharIndex(char ch)
{
	return searchCharIndex((unsigned short)ch);
}

int Sprite::searchCharIndex(unsigned short ch)
{
	int l = 0;
	int r = m_charMapLength - 1;
	while (l <= r)
	{
		int m = (l + r) / 2;
		if (m_pCharMap[m] == ch) return m;
		if (m_pCharMap[m] < ch)
		{
			l = m + 1;
		}
		else
		{
			r = m - 1;
		}
	}
	return -1;
}

void Sprite::calcuStringRect(const char* pString, int left, int right, int& x, int& y, int& width, int& height)
{
	int curx = 0, minx = MY_MAX_INT, maxx = -minx;
	int cury = 0, miny = MY_MAX_INT, maxy = -miny;

	for (int i = left; i <= right; i++)
	{
		if (pString[i] == ' ')
		{
			if (minx > curx) minx = curx;
			if (maxx < curx + m_spaceWidth) maxx = curx + m_spaceWidth;
			curx += m_spaceWidth;
		}
		else
		{
			int index = searchCharIndex(pString[i]);
			assert(index > -1);

			FModule& fmod = m_pFramesList[0].m_pFModulesList[index];
			int w = m_pModulesList[ fmod.m_moduleIndex ].w;
			int h = m_pModulesList[ fmod.m_moduleIndex ].h;

			//
			curx += fmod.m_xInFrame;

			if (minx > curx) minx = curx;
			if (maxx < curx + w) maxx = curx + w;
			
			curx += w + m_charHorizonBetween;
			
			//
			cury = fmod.m_yInFrame;

			if (miny > cury) miny = cury;
			if (maxy < cury + h) maxy = cury + h;
		}
	}

	x = minx;
	width = maxx - minx;

	y = miny;
	height = maxy - miny;
}

void Sprite::calcuStringRect(const unsigned short* pString, int left, int right, int& x, int& y, int& width, int& height)
{
	int curx = 0, minx = MY_MAX_INT, maxx = -minx;
	int cury = 0, miny = MY_MAX_INT, maxy = -miny;

	for (int i = left; i <= right; i++)
	{
		if (pString[i] == ' ')
		{
			if (minx > curx) minx = curx;
			if (maxx < curx + m_spaceWidth) maxx = curx + m_spaceWidth;
			curx += m_spaceWidth;
		}
		else
		{
			int index = searchCharIndex(pString[i]);
			assert(index > -1);

			FModule& fmod = m_pFramesList[0].m_pFModulesList[index];
			int w = m_pModulesList[ fmod.m_moduleIndex ].w;
			int h = m_pModulesList[ fmod.m_moduleIndex ].h;

			//
			curx += fmod.m_xInFrame;

			if (minx > curx) minx = curx;
			if (maxx < curx + w) maxx = curx + w;
			
			curx += w + m_charHorizonBetween;
			
			//
			cury = fmod.m_yInFrame;

			if (miny > cury) miny = cury;
			if (maxy < cury + h) maxy = cury + h;
		}
	}

	x = minx;
	width = maxx - minx;

	y = miny;
	height = maxy - miny;
}

void Sprite::wrapString(char* sz, int width, float xScale)
{
	int len = strlen(sz);

	float curx = 0.0f, minx = MY_MAX_FLOAT, maxx = -minx;

	for (int i = 0; i < len; i++)
	{
		if (sz[i] == '\n')
		{
			curx = 0.0f; minx = MY_MAX_FLOAT; maxx = -minx;
		}
		else if (sz[i] == ' ')
		{
			if (minx > curx) minx = curx;
			if (maxx < curx + m_spaceWidth * xScale) maxx = curx + m_spaceWidth * xScale;

			if (maxx - minx > width)
			{
				curx = 0.0f; minx = MY_MAX_FLOAT; maxx = -minx;
				sz[i] = '\n';
			}
			else
			{
				curx += m_spaceWidth * xScale;
			}
		}
		else
		{
			int index = searchCharIndex(sz[i]);
			assert(index > -1);

			FModule& fmod = m_pFramesList[0].m_pFModulesList[index];
			float w = m_pModulesList[ fmod.m_moduleIndex ].w * xScale;

			//
			curx += fmod.m_xInFrame * xScale;

			if (minx > curx) minx = curx;
			if (maxx < curx + w) maxx = curx + w;

			if (maxx - minx > width)
			{
				bool found = false;
				for (int j = i - 1; j >= 0; j--)
				{
					if (sz[j] == ' ')
					{
						found = true;
						curx = 0.0f; minx = MY_MAX_FLOAT; maxx = -minx;
						sz[j] = '\n';
						i = j;
						break;
					}
					if (sz[j] == '\n') break;
				}
				if (!found)
				{
					for (i++; i < len; i++)
					{
						if (sz[i] == ' ' || sz[i] == '\n')
						{
							curx = 0.0f; minx = MY_MAX_FLOAT; maxx = -minx;
							sz[i] = '\n';
							break;
						}
					}
				}
			}
			else
			{
				curx += w + m_charHorizonBetween * xScale;
			}
		}
	}
}

void Sprite::wrapString(unsigned short* sz, int width, float xScale)
{
	int len = utf_strlen(sz);

	float curx = 0.0f, minx = MY_MAX_FLOAT, maxx = -minx;

	for (int i = 0; i < len; i++)
	{
		if (sz[i] == '\n')
		{
			curx = 0.0f; minx = MY_MAX_FLOAT; maxx = -minx;
		}
		else if (sz[i] == ' ')
		{
			if (minx > curx) minx = curx;
			if (maxx < curx + m_spaceWidth * xScale) maxx = curx + m_spaceWidth * xScale;

			if (maxx - minx > width)
			{
				curx = 0.0f; minx = MY_MAX_FLOAT; maxx = -minx;
				sz[i] = '\n';
			}
			else
			{
				curx += m_spaceWidth * xScale;
			}
		}
		else
		{
			int index = searchCharIndex(sz[i]);
			assert(index > -1);

			FModule& fmod = m_pFramesList[0].m_pFModulesList[index];
			float w = m_pModulesList[ fmod.m_moduleIndex ].w * xScale;

			//
			curx += fmod.m_xInFrame * xScale;

			if (minx > curx) minx = curx;
			if (maxx < curx + w) maxx = curx + w;

			if (maxx - minx > width)
			{
				bool found = false;
				for (int j = i - 1; j >= 0; j--)
				{
					if (sz[j] == ' ')
					{
						found = true;
						curx = 0.0f; minx = MY_MAX_FLOAT; maxx = -minx;
						sz[j] = '\n';
						i = j;
						break;
					}
					if (sz[j] == '\n') break;
				}
				if (!found)
				{
					for (i++; i < len; i++)
					{
						if (sz[i] == ' ' || sz[i] == '\n')
						{
							curx = 0.0f; minx = MY_MAX_FLOAT; maxx = -minx;
							sz[i] = '\n';
							break;
						}
					}
				}
			}
			else
			{
				curx += w + m_charHorizonBetween * xScale;
			}
		}
	}
}

//==============================================================================================================
	}
}