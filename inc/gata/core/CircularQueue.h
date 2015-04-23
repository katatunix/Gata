#ifndef _CIRCULAR_QUEUE_H_
#define _CIRCULAR_QUEUE_H_

#include <cassert>

namespace gata {
	namespace core {
//=============================================================================================

#define MAX_CIRCULAR_QUEUE_LENGTH 128

template<typename T>
class CircularQueue
{
public:
	CircularQueue()
	{
		m_len = 0;
	}

	~CircularQueue()
	{
	}

	int len()
	{
		return m_len;
	}

	void put(const T& t)
	{
		assert(m_len < MAX_CIRCULAR_QUEUE_LENGTH);
		if (m_len == 0)
		{
			m_data[0] = t;
			m_head = 0;
			m_len = 1;
			return;
		}

		int tail = m_head + m_len;
		if (tail >= MAX_CIRCULAR_QUEUE_LENGTH)
		{
			tail -= MAX_CIRCULAR_QUEUE_LENGTH;
		}
		m_data[tail] = t;
		m_len++;
	}

	T get()
	{
		assert(m_len > 0);
		int old_head = m_head;
		m_head++;
		if (m_head >= MAX_CIRCULAR_QUEUE_LENGTH)
		{
			m_head -= MAX_CIRCULAR_QUEUE_LENGTH;
		}
		m_len--;
		return m_data[old_head];
	}

private:
	int m_head;
	int m_len;
	T m_data[MAX_CIRCULAR_QUEUE_LENGTH];
};

//=============================================================================================
	}
}

#endif
