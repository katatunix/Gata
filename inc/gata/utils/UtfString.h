#ifndef _UTF_STRING_H_
#define _UTF_STRING_H_

namespace gata {
	namespace utils {
//===================================================================================================
int utf_strlen(const unsigned short* p);

void utf_strcpy(unsigned short* dst, const unsigned short* src);
//===================================================================================================
	}
}

#endif
