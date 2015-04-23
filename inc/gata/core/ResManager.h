#ifndef _RES_MANAGER_H_
#define _RES_MANAGER_H_

#include <string>
#include "macro.h"
#include "Resource.h"
#include "Singleton.h"

namespace gata
{
	namespace core
	{
//==================================================================================
template<typename R>
class ResManager : public Singleton< ResManager<R> >
{
public:
	ResManager(int number, const char* pszNamesList[])
	{
		assert(number > 0);
		assert(pszNamesList);
		m_number = number;

		m_pszNamesList = new char*[number];
		for (int i = 0; i < number; i++)
		{
			assert(pszNamesList[i]);
			assert(strlen(pszNamesList[i]) > 0);
			m_pszNamesList[i] = new char[strlen(pszNamesList[i]) + 1];
			strcpy(m_pszNamesList[i], pszNamesList[i]);
		}

		m_ppResList = new R*[number];
		for (int i = 0; i < number; i++)
		{
			m_ppResList[i] = R::create();
		}
	}

	~ResManager()
	{
		unloadAll();
		for (int i = 0; i < m_number; i++)
		{
			SAFE_DEL(m_ppResList[i]);
		}
		SAFE_DEL_ARRAY(m_ppResList);

		for (int i = 0; i < m_number; i++)
		{
			SAFE_DEL(m_pszNamesList[i]);
		}
		SAFE_DEL_ARRAY(m_pszNamesList);
	}

	bool load(int resIdx)
	{
		assert(resIdx >= 0 && resIdx < m_number);
		Resource* p = (Resource*)m_ppResList[resIdx];
		if (p->isLoaded()) return true;
		return p->load(m_pszNamesList[resIdx]);
	}

	int loadAll()
	{
		int countFailed = 0;
		for (int i = 0; i < m_number; i++)
		{
			if (!load(i))
			{
				countFailed++;
			}
		}
		return countFailed;
	}

	void unload(int resIdx)
	{
		assert(resIdx >= 0 && resIdx < m_number);
		Resource* p = (Resource*)m_ppResList[resIdx];
		if (!p->isLoaded()) return;
		p->unload();
	}

	void unloadAll()
	{
		for (int i = 0; i < m_number; i++)
		{
			unload(i);
		}
	}

	R* getRes(int resIdx)
	{
		assert(resIdx >= 0 && resIdx < m_number);
		Resource* p = (Resource*)m_ppResList[resIdx];
		if (!p->isLoaded())
		{
			if (!p->load(m_pszNamesList[resIdx]))
			{
				assert(0);
				return 0;
			}
		}
		return m_ppResList[resIdx];
	}

	int getNumber()
	{
		return m_number;
	}

private:
	int m_number;
	R** m_ppResList;
	char** m_pszNamesList;
};
//==================================================================================
	}
}
#endif
