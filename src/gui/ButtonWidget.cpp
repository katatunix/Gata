#include <gata/gui/ButtonWidget.h>

#include <string>
#include <cassert>

#include <gata/core/macro.h>

namespace gata {
	namespace gui {
//===============================================================================
ButtonWidget::~ButtonWidget()
{
	SAFE_DEL_ARRAY(m_szText);
}

void ButtonWidget::setText(const char* szText)
{
	SAFE_DEL_ARRAY(m_szText);
	if (!szText) return;

	int len = strlen(szText);
	if (len <= 0) return;

	m_szText = new char[len + 1];
	strcpy(m_szText, szText);
}
//===============================================================================
	}
}
