#ifndef _STANDARD_BUTTON_WIDGET_H_
#define _STANDARD_BUTTON_WIDGET_H_

#include "ButtonWidget.h"

namespace gata {
	namespace gui {
//===========================================================================================================================
class StandardButtonWidget : public ButtonWidget
{
public:
	enum StandardButtonState
	{
		SBS_NORMAL,
		SBS_HOVERING,
		SBS_HOLDING
	};

	//
	StandardButtonWidget() : ButtonWidget(), m_state(SBS_NORMAL) { }

	StandardButtonWidget(const char* szText) : ButtonWidget(szText), m_state(SBS_NORMAL) { }

	StandardButtonWidget(Widget* pDad, int x, int y, int w, int h, WidgetListener* pListener, const char* szText) :
			ButtonWidget(pDad, x, y, w, h, pListener, szText), m_state(SBS_NORMAL) { }

	//
	void onRender(gata::graphic::Graphic* g);

	virtual void onRender_Normal(gata::graphic::Graphic* g);
	virtual void onRender_Hover(gata::graphic::Graphic* g);
	virtual void onRender_Hold(gata::graphic::Graphic* g);

	//
	void onMouseHover(int x, int y, int id);
	void onMouseDown(int x, int y, int id);
	void onMouseUp(int x, int y, int id);
	void onMouseOut(int id);

protected:
	StandardButtonState m_state;
};
//===========================================================================================================================
	}
}

#endif
