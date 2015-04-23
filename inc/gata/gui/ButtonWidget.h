#ifndef _BUTTON_WIDGET_H_
#define _BUTTON_WIDGET_H_

#include "Widget.h"

namespace gata {
	namespace gui {
//======================================================================================================================
class ButtonWidget : public Widget
{
public:
	ButtonWidget() : Widget(), m_szText(0) { }

	ButtonWidget(const char* szText) : Widget(), m_szText(0)
	{
		setText(szText);
	}

	ButtonWidget(Widget* pDad, int x, int y, int w, int h, WidgetListener* pListener, const char* szText) :
			Widget(pDad, x, y, w, h, pListener), m_szText(0)
	{
		setText(szText);
	}

	virtual ~ButtonWidget();

	//
	void setText(const char* szText);

private:
	char* m_szText;
};
//======================================================================================================================
	}
}

#endif
