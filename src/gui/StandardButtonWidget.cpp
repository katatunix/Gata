#include <gata/gui/StandardButtonWidget.h>

namespace gata {
	namespace gui {
//===============================================================================
void StandardButtonWidget::onRender(gata::graphic::Graphic* g)
{
	switch (m_state)
	{
		case SBS_NORMAL:		onRender_Normal(g);	break;
		case SBS_HOVERING:		onRender_Hover(g);	break;
		case SBS_HOLDING:		onRender_Hold(g);	break;
	}
}

void StandardButtonWidget::onRender_Normal(gata::graphic::Graphic* g)
{
	g->setColor(0x0000FFFF);
	g->fillRect(m_xAbsolute, m_yAbsolute, m_width, m_height);
}

void StandardButtonWidget::onRender_Hover(gata::graphic::Graphic* g)
{
	g->setColor(0x654321FF);
	g->fillRect(m_xAbsolute, m_yAbsolute, m_width, m_height);
}

void StandardButtonWidget::onRender_Hold(gata::graphic::Graphic* g)
{
	g->setColor(0xFF00FFFF);
	g->fillRect(m_xAbsolute, m_yAbsolute, m_width, m_height);
}

//

void StandardButtonWidget::onMouseHover(int x, int y, int id)
{
	m_state = SBS_HOVERING;

	ButtonWidget::onMouseHover(x, y, id);
}

void StandardButtonWidget::onMouseDown(int x, int y, int id)
{
	m_state = SBS_HOLDING;

	ButtonWidget::onMouseDown(x, y, id);
}

void StandardButtonWidget::onMouseUp(int x, int y, int id)
{
	m_state = SBS_NORMAL;

	ButtonWidget::onMouseUp(x, y, id);

	if (m_pListener)
	{
		m_pListener->actionPerformed(this);
	}
}

void StandardButtonWidget::onMouseOut(int id)
{
	m_state = SBS_NORMAL;

	ButtonWidget::onMouseOut(id);
}
//===============================================================================
	}
}
