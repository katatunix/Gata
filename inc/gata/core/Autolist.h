#ifndef _AUTOLIST_H_
#define _AUTOLIST_H_

#include "LinkedList.h"

namespace gata {
	namespace core {
//=============================================================================================

template<typename T>
class Autolist
{
public:
	Autolist()
	{
		m_sList.add( (T*)this );
	}

	~Autolist()
	{
		m_sList.remove( (T*)this );
	}

	static LinkedNode<T*>* firstNode()
	{
		return m_sList.head;
	}

	static T* head()
	{
		m_spCurrentNode = m_sList.head;
		return m_spCurrentNode->value;
	}

	static T* head2()
	{
		m_spCurrentNode2 = m_sList.head;
		return m_spCurrentNode2->value;
	}

	static T* next()
	{
		if (!m_spCurrentNode) return 0;
		m_spCurrentNode = m_spCurrentNode->next;
		if (!m_spCurrentNode) return 0;
		return m_spCurrentNode->value;
	}

	static T* next2()
	{
		if (!m_spCurrentNode2) return 0;
		m_spCurrentNode2 = m_spCurrentNode2->next;
		if (!m_spCurrentNode2) return 0;
		return m_spCurrentNode2->value;
	}

private:
	static LinkedList<T*> m_sList;
	static LinkedNode<T*>* m_spCurrentNode;
	static LinkedNode<T*>* m_spCurrentNode2;
};

template<typename T>	LinkedList<T*>				Autolist<T>::m_sList;
template<typename T>	LinkedNode<T*>*				Autolist<T>::m_spCurrentNode	= 0;
template<typename T>	LinkedNode<T*>*				Autolist<T>::m_spCurrentNode2	= 0;
//=============================================================================================
	}
}

#endif
