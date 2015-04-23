#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

namespace gata {
	namespace core {
//=============================================================================================

template<typename T>
struct LinkedNode
{
	LinkedNode(T v, LinkedNode* n) : value(v), next(n) { }

	T value;
	LinkedNode* next;
};

template<typename T>
class LinkedList
{
public:
	LinkedList() : head(0), tail(0) { }

	~LinkedList()
	{
		LinkedNode<T>* p = head;
		LinkedNode<T>* q = 0;
		while (p)
		{
			q = p->next;
			delete p;
			p = q;
		}
	}

	void add(T t)
	{
		if (head)
		{
			LinkedNode<T>* node = new LinkedNode<T>(t, 0);
			tail->next = node;
			tail = node;
		}
		else
		{
			head = new LinkedNode<T>(t, 0);
			tail = head;
		}
	}

	bool remove(T t)
	{
		LinkedNode<T>* p = head;
		LinkedNode<T>* q = 0;
		while (p)
		{
			if (p->value == t)
			{
				if (p == tail) tail = q;
				if (q)
				{
					q->next = p->next;
					delete p;
				}
				else
				{
					head = p->next;
					delete p;
				}
				return true;
			}

			q = p;
			p = p->next;
		}
		return false;
	}

	LinkedNode<T>* head;
	LinkedNode<T>* tail;
};

//=============================================================================================
	}
}

#endif
