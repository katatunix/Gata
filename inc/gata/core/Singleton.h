#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <cassert>

namespace gata {
	namespace core {
//==================================================================================
template<typename T>
class Singleton
{
public:
	Singleton()
	{
		assert(!m_spSingleton);
		int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
		m_spSingleton = (T*)( (int)this + offset );
	}

	~Singleton()
	{
		assert(m_spSingleton);
		m_spSingleton = 0;
	}

	static T* getInstance()
	{
		assert(m_spSingleton);
		return m_spSingleton;
	}

	static void freeInstance()
	{
		assert(m_spSingleton);
		delete m_spSingleton;
		m_spSingleton = 0;
	}

private:
	static T* m_spSingleton;
};

template<typename T> T* Singleton<T>::m_spSingleton = 0;
//==================================================================================
	}
}

#endif
