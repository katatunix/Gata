#ifndef _RESOURCE_H_
#define _RESOURCE_H_

namespace gata {
	namespace core {
//==================================================================================
class Resource
{
public:
	Resource() { }
	virtual ~Resource() { }

	virtual bool load(const char* szName) = 0;
	virtual void unload() = 0;

	virtual void* getHandle() = 0;

	bool isLoaded() { return getHandle() != 0 ? true : false; }
};
//==================================================================================
	}
}

#endif
