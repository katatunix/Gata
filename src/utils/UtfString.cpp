#include <gata/utils/UtfString.h>

namespace gata {
	namespace utils {
//================================================================================

int utf_strlen(const unsigned short* p)
{
	int len = 0;
	while (p[len]) len++;
	return len;
}

void utf_strcpy(unsigned short* dst, const unsigned short* src)
{
	int i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
}

//================================================================================
	}
}
