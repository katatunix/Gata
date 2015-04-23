#include <gata/core/macro.h>
#include <gata/gui/Widget.h>

#include <stdio.h>
#include <cassert>

namespace gata {
	namespace gui {
//===============================================================================================

Widget* Widget::m_sppHoveringWidgetsList[MAX_POINTERS_NUMBER];
Widget* Widget::m_sppDowningWidgetsList[MAX_POINTERS_NUMBER];

/*static*/ void Widget::init()
{
	for (int i = 0; i < MAX_POINTERS_NUMBER; i++)
	{
		m_sppDowningWidgetsList[i] = 0;
		m_sppHoveringWidgetsList[i] = 0;
	}
}

// Constructor
Widget::Widget() :
		m_pListener(0),
		m_pDadWidget(0),
		m_childrenWidgetNumber(0),

		m_xRelative(0),
		m_yRelative(0),
		m_xRelative_Original(0),
		m_yRelative_Original(0),

		m_xAbsolute(0),
		m_yAbsolute(0),

		m_width(0),
		m_height(0),
		m_width_Original(0),
		m_height_Original(0),

		m_backgroundColor(0x0),
		m_moveType(MT_NONE),
		m_xStartTouch(-1),
		m_yStartTouch(-1),
		m_visible(true),
		m_isModal(false),
		m_isFixedRatio(false),
		m_isAnchorLeft(true),
		m_isAnchorTop(true)
{
}

Widget::Widget(Widget* pDad, int x, int y, int w, int h, WidgetListener* pListener) :
		m_width(w),
		m_height(h),
		m_width_Original(w),
		m_height_Original(h),

		m_pListener(pListener),
		m_childrenWidgetNumber(0),
		m_backgroundColor(0x0),
		m_moveType(MT_NONE),
		m_xStartTouch(-1),
		m_yStartTouch(-1),
		m_visible(true),
		m_isModal(false),
		m_isFixedRatio(false),
		m_isAnchorLeft(true),
		m_isAnchorTop(true)
{
	setDad(pDad);
	setRelativePos(x, y);
	setRelativePos_Original(x, y);
}

// Destructor
Widget::~Widget()
{
	for (int i = 0; i < m_childrenWidgetNumber; i++)
	{
		SAFE_DEL(m_pChildrenWidgetList[i]);
	}
}

//
void Widget::render(gata::graphic::Graphic* g)
{
	if (!m_visible) return;

	int x, y, w, h;
	g->getClip(x, y, w, h);
	int x2 = m_xAbsolute, y2 = m_yAbsolute;
	int u = x2 + m_width, v = y2 + m_height;

	if (x2 < x) x2 = x;
	if (y2 < y) y2 = y;
	if (u > x + w) u = x + w;
	if (v > y + h) v = y + h;

	if (u - x2 > 0 && v - y2 > 0)
	{
		onRender(g);

		g->setClip(x2, y2, u - x2, v - y2);
			
		for (int i = 0; i < m_childrenWidgetNumber; i++)
		{
			m_pChildrenWidgetList[i]->render(g);
		}

		g->setClip(x, y, w, h);
	}
}

void Widget::update()
{
	onUpdate();
	for (int i = 0; i < m_childrenWidgetNumber; i++)
	{
		m_pChildrenWidgetList[i]->update();
	}
}

void Widget::setRelativePos(int x, int y)
{
	m_xRelative = x;
	m_yRelative = y;
	if (!m_pDadWidget)
	{
		m_xAbsolute = m_xRelative;
		m_yAbsolute = m_yRelative;
	}
	else
	{
		m_xAbsolute = m_pDadWidget->getAbsoluteX() + m_xRelative;
		m_yAbsolute = m_pDadWidget->getAbsoluteY() + m_yRelative;
	}
	for (int i = 0; i < m_childrenWidgetNumber; i++)
	{
		m_pChildrenWidgetList[i]->setRelativePos(
			m_pChildrenWidgetList[i]->getRelativeX(),
			m_pChildrenWidgetList[i]->getRelativeY()
		);
	}
}

void Widget::setRelativePos_Original(int x, int y)
{
	m_xRelative_Original = x;
	m_yRelative_Original = y;
}

void Widget::setSize(int w, int h)
{
	assert(w >= 0 && h >= 0);
	m_width = w;
	m_height = h;
}

void Widget::setSize_Original(int w, int h)
{
	assert(w >= 0 && h >= 0);
	m_width_Original = w;
	m_height_Original = h;
}

void Widget::setIsModal(bool b)
{
	m_isModal = b;
	if (b)
	{
		setVisible(true);
		if (m_pDadWidget)
		{
			int i = 0;
			for (int j = 0; j < m_pDadWidget->m_childrenWidgetNumber; j++)
			{
				if (m_pDadWidget->m_pChildrenWidgetList[j] != this)
				{
					m_pDadWidget->m_pChildrenWidgetList[j]->m_isModal = false;
				}
				else
				{
					i = j;
				}
			}

			for (int j = i; j <= m_pDadWidget->m_childrenWidgetNumber - 2; j++)
			{
				m_pDadWidget->m_pChildrenWidgetList[j] = m_pDadWidget->m_pChildrenWidgetList[j + 1];
			}
			m_pDadWidget->m_pChildrenWidgetList[m_pDadWidget->m_childrenWidgetNumber - 1] = this;
		}
	}
}

bool Widget::containsPoint(int x, int y)
{
	return	m_xRelative <= x && x < m_xRelative + m_width &&
			m_yRelative <= y && y < m_yRelative + m_height;
}

void Widget::setDad(Widget* pDad)
{
	m_pDadWidget = pDad;

	if (pDad)
	{
		pDad->appendChild(this);
	}
}

void Widget::appendChild(Widget* p)
{
	assert(m_childrenWidgetNumber < MAX_CHILDREN_WIDGET_NUMBER);
	for (int i = 0; i < m_childrenWidgetNumber; i++)
	{
		if (m_pChildrenWidgetList[i] == p) return;
	}

	for (int i = 0; i < m_childrenWidgetNumber; i++)
	{
		m_pChildrenWidgetList[i]->setIsModal(false);
	}
	
	m_pChildrenWidgetList[m_childrenWidgetNumber++] = p;
}

void Widget::processMouseEvent(MouseEvent evt)
{
	if (!m_visible) return;

	Widget* pDowningWidget = m_sppDowningWidgetsList[evt.id];
	Widget* pHoveringWidget = m_sppHoveringWidgetsList[evt.id];

	if (evt.x < 0 || evt.y < 0 || evt.x >= m_width || evt.y >= m_height)
	{
		assert(!m_pDadWidget);

		if (pDowningWidget) pDowningWidget->onMouseOut(evt.id);
		m_sppDowningWidgetsList[evt.id] = 0;

		if (pHoveringWidget) pHoveringWidget->onMouseOut(evt.id);
		m_sppHoveringWidgetsList[evt.id] = 0;

		return;
	}

	if  (	(evt.m_type == MOUSE_DOWN || evt.m_type == MOUSE_UP) &&
			pDowningWidget &&
			pDowningWidget->m_moveType != MT_NONE &&
			pDowningWidget->m_pDadWidget == this		)
	{
		if (evt.m_type == MOUSE_DOWN)
		{
			pDowningWidget->onMouseDown(	evt.x + m_xAbsolute - pDowningWidget->m_xAbsolute,
											evt.y + m_yAbsolute - pDowningWidget->m_yAbsolute, evt.id);
		}
		else
		{
			pDowningWidget->onMouseUp(		evt.x + m_xAbsolute - pDowningWidget->m_xAbsolute,
											evt.y + m_yAbsolute - pDowningWidget->m_yAbsolute, evt.id);
			m_sppDowningWidgetsList[evt.id] = 0;
		}

		return;
	}

	// check if we have the modal widget in the children list
	if (m_childrenWidgetNumber >= 1)
	{
		Widget* child = m_pChildrenWidgetList[m_childrenWidgetNumber - 1];
		if (child->getIsModal())
		{
			if (child->containsPoint(evt.x, evt.y))
			{
				child->processMouseEvent(
					MouseEvent(
						evt.m_type,
						evt.x - child->m_xRelative,
						evt.y - child->m_yRelative,
						evt.id
					)
				);
			}
			else
			{
				if (pDowningWidget) pDowningWidget->onMouseOut(evt.id);
				m_sppDowningWidgetsList[evt.id] = 0;

				if (pHoveringWidget) pHoveringWidget->onMouseOut(evt.id);
				m_sppHoveringWidgetsList[evt.id] = 0;
			}

			return;
		}
	}

	// traverse children list
	for (int i = m_childrenWidgetNumber - 1; i >= 0; i--)
	{
		Widget* child = m_pChildrenWidgetList[i];
		if (child->m_visible && child->containsPoint(evt.x, evt.y))
		{
			if (evt.m_type == MOUSE_DOWN)
			{
				for (int j = i; j <= m_childrenWidgetNumber - 2; j++)
				{
					m_pChildrenWidgetList[j] = m_pChildrenWidgetList[j + 1];
				}
				m_pChildrenWidgetList[m_childrenWidgetNumber - 1] = child;
			}

			child->processMouseEvent(
				MouseEvent(
					evt.m_type,
					evt.x - child->m_xRelative,
					evt.y - child->m_yRelative,
					evt.id
				)
			);
			return;
		}
	}

	//
	switch (evt.m_type)
	{
		case MOUSE_HOVER:
		{
			if (pHoveringWidget && pHoveringWidget != this)
			{
				pHoveringWidget->onMouseOut(evt.id);
			}
			m_sppHoveringWidgetsList[evt.id] = this;

			assert(pDowningWidget == 0);

			onMouseHover(evt.x, evt.y, evt.id);
		}
		break;

		case MOUSE_DOWN:
		{
			if (pDowningWidget && pDowningWidget != this)
			{
				pDowningWidget->onMouseOut(evt.id);
			}
					
			m_sppHoveringWidgetsList[evt.id] = 0;
			m_sppDowningWidgetsList[evt.id] = this;

			onMouseDown(evt.x, evt.y, evt.id);
		}
		break;

		case MOUSE_UP:
		{
			assert(pDowningWidget);
			assert(pDowningWidget == this);
			assert(pHoveringWidget == 0);

			m_sppDowningWidgetsList[evt.id] = 0;

			onMouseUp(evt.x, evt.y, evt.id);
		}
		break;
	}
}

//

void Widget::onMouseHover(int x, int y, int id)
{
	m_xStartTouch = m_yStartTouch = -1;

	if (m_pListener)
	{
		m_pListener->onMouseHover(this, x, y, id);
	}
}

void Widget::onMouseDown(int x, int y, int id)
{
	if (m_xStartTouch > -1)
	{
		if (m_moveType == MT_OUTSIDE_DAD)
		{
			setRelativePos(m_xRelative + x - m_xStartTouch, m_yRelative + y - m_yStartTouch);
		}
		else if (m_moveType == MT_INSIDE_DAD)
		{
			int xx = m_xRelative + x - m_xStartTouch;
			int yy = m_yRelative + y - m_yStartTouch;
			if (xx < 0)
			{
				m_xStartTouch = x;
				xx = 0;
			}
			if (yy < 0)
			{
				m_yStartTouch = y;
				yy = 0;
			}
			if (m_pDadWidget)
			{
				if (xx + m_width > m_pDadWidget->m_width)
				{
					m_xStartTouch = x;
					xx = m_pDadWidget->m_width - m_width;
				}
				if (yy + m_height > m_pDadWidget->m_height)
				{
					m_yStartTouch = y;
					yy = m_pDadWidget->m_height - m_height;
				}
			}
			setRelativePos(xx, yy);
		}
	}
	else
	{
		m_xStartTouch = x;
		m_yStartTouch = y;
	}

	if (m_pListener)
	{
		m_pListener->onMouseDown(this, x, y, id);
	}
}

void Widget::onMouseUp(int x, int y, int id)
{
	m_xStartTouch = m_yStartTouch = -1;

	if (m_pListener)
	{
		m_pListener->onMouseUp(this, x, y, id);
	}

	m_sppHoveringWidgetsList[id] = this;
	onMouseHover(x, y, id);
}

void Widget::onMouseOut(int id)
{
	m_xStartTouch = m_yStartTouch = -1;

	if (m_pListener)
	{
		m_pListener->onMouseOut(this, id);
	}
}

//

void Widget::onRender(gata::graphic::Graphic* g)
{
	g->setColor(m_backgroundColor);
	g->fillRect(m_xAbsolute, m_yAbsolute, m_width, m_height);
}

void Widget::onUpdate()
{
}

//
void Widget::resizeTo(int newWidth, int newHeight)
{
	m_width = newWidth;
	m_height = newHeight;

	for (int i = 0; i < m_childrenWidgetNumber; i++)
	{
		Widget* child = m_pChildrenWidgetList[i];
		int x = child->getRelativeX_Original();
		int y = child->getRelativeY_Original();
		int w = child->getWidth_Original();
		int h = child->getHeight_Original();
		int www = w;
		int hhh = h;

		if (child->getIsFixedRatio())
		{
			if (newWidth * m_height_Original < newHeight * m_width_Original)
			{
				w = w * newWidth / m_width_Original;
				h = h * newWidth / m_width_Original;
			}
			else
			{
				w = w * newHeight / m_height_Original;
				h = h * newHeight / m_height_Original;
			}
		}
		else
		{
			w = w * newWidth / m_width_Original;
			h = h * newHeight / m_height_Original;
		}

		if (child->getIsAnchorLeft())
		{
			x = x * newWidth / m_width_Original;
		}
		else
		{
			x = (x + www) * newWidth / m_width_Original - w;
		}

		if (child->getIsAnchorTop())
		{
			y = y * newHeight / m_height_Original;
		}
		else
		{
			y = (y + hhh) * newHeight / m_height_Original - h;
		}

		child->setRelativePos(x, y);
		child->resizeTo(w, h);
	}
	 
}
//===============================================================================================
	}
}
