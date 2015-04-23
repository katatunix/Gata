#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "../graphic/Graphic.h"

namespace gata {
	namespace gui {
//==================================================================================================

#define MAX_CHILDREN_WIDGET_NUMBER 128
#define MAX_POINTERS_NUMBER 16

enum MouseEventType
{
	MOUSE_HOVER,
	MOUSE_DOWN,
	MOUSE_UP,
};

typedef struct _MouseEvent
{
	//_MouseEvent(MouseEventType type, int _x, int _y) : m_type(type), x(_x), y(_y), id(0) { }
	_MouseEvent(MouseEventType type, int _x, int _y, int _id) : m_type(type), x(_x), y(_y), id(_id) { }

	MouseEventType m_type;
	int x;
	int y;
	int id;
} MouseEvent;

enum MoveType
{
	MT_NONE,
	MT_OUTSIDE_DAD,
	MT_INSIDE_DAD,
};

class Widget;
class WidgetListener
{
public:
	virtual void onMouseHover(Widget* pSenderWidget, int x, int y, int id) { }
	virtual void onMouseDown(Widget* pSenderWidget, int x, int y, int id) { }
	virtual void onMouseUp(Widget* pSenderWidget, int x, int y, int id) { }
	virtual void onMouseOut(Widget* pSenderWidget, int id) { }

	// Button
	virtual void actionPerformed(Widget* pSenderWidget) { }
};

class Widget
{
public:

	Widget();

	Widget(Widget* pDad, int x, int y, int w, int h, WidgetListener* pListener);

	virtual ~Widget();

	static void init();

	int getRelativeX() { return m_xRelative; }
	int getRelativeY() { return m_yRelative; }
	int getRelativeX_Original() { return m_xRelative_Original; }
	int getRelativeY_Original() { return m_yRelative_Original; }

	int getAbsoluteX() { return m_xAbsolute; }
	int getAbsoluteY() { return m_yAbsolute; }

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	int getWidth_Original() { return m_width_Original; }
	int getHeight_Original() { return m_height_Original; }

	void setRelativePos(int x, int y);
	void setRelativePos_Original(int x, int y);

	void setSize(int w, int h);
	void setSize_Original(int w, int h);

	void setIsFixedRatio(bool b) { m_isFixedRatio = b; }
	bool getIsFixedRatio() { return m_isFixedRatio; }

	void setIsAnchorLeft(bool b) { m_isAnchorLeft = b; }
	void setIsAnchorTop(bool b) { m_isAnchorTop = b; }
	bool getIsAnchorLeft() { return m_isAnchorLeft; }
	bool getIsAnchorTop() { return m_isAnchorTop; }

	//
	void resizeTo(int newWidth, int newHeight);
	//

	void setListener(WidgetListener* p) { m_pListener = p; }

	void setDad(Widget* pDad);

	void setBackgroundColor(int color) { m_backgroundColor = color; }

	void setMoveType(MoveType mt) { m_moveType = mt; }
	MoveType getMoveType() { return m_moveType; }

	void setVisible(bool b) { m_visible = b; }
	bool getVisible() { return m_visible; }

	//
	void setIsModal(bool b);
	bool getIsModal() { return m_isModal; }

	//
	void processMouseEvent(MouseEvent evt);
	void render(gata::graphic::Graphic* g);
	void update();

	//
	virtual void onMouseHover(int x, int y, int id);
	virtual void onMouseDown(int x, int y, int id);
	virtual void onMouseUp(int x, int y, int id);
	virtual void onMouseOut(int id);

	virtual void onRender(gata::graphic::Graphic* g);
	virtual void onUpdate();

protected:
	//
	void appendChild(Widget* p);
	bool containsPoint(int x, int y);

	//
	int m_xRelative, m_yRelative;
	int m_xAbsolute, m_yAbsolute;
	int m_width, m_height;

	//
	int m_xRelative_Original, m_yRelative_Original;
	int m_width_Original, m_height_Original;

	bool m_isFixedRatio;
	bool m_isAnchorLeft;
	bool m_isAnchorTop;

	int m_backgroundColor;

	MoveType m_moveType;
	int m_xStartTouch;
	int m_yStartTouch;

	bool m_visible;

	bool m_isModal;

	//
	static Widget* m_sppHoveringWidgetsList[MAX_POINTERS_NUMBER];
	static Widget* m_sppDowningWidgetsList[MAX_POINTERS_NUMBER];

	//
	WidgetListener* m_pListener;

	//
	Widget* m_pDadWidget;
	Widget* m_pChildrenWidgetList[MAX_CHILDREN_WIDGET_NUMBER];
	int m_childrenWidgetNumber;
};
//==================================================================================================
	}
}
#endif
