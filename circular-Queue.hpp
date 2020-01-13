
/***********************************************************************************
 * Author: Ahmad Afkandeh (Arta Afkandeh)
 * 
 * a very simple fixed size circular-queue in cpp which optimized and
 * designed to be realy simple, fast, and thread safety consideration
 * 
 * Queue has fixed size items so it's limitation is defined with "QUEUE_MAX_ITEMS"
 * 
 * 
 **********************************************************************************/
#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H
#include <mutex>
#include "Config.h"
#define QUEUE_MAX_ITEMS 100

template <class T>
class CircularQueue
{
public   :
        CircularQueue();
        ~CircularQueue();
        int ADD(const T *obj);
        bool Read(T * obj);
private:
	typedef struct {
		T object;
        bool IsCopied;
	}Queuestruct;

	Queuestruct m_QueueElements[QUEUE_MAX_ITEMS];
    std::mutex m_hwnd;
    int m_ReaderPointer;
    int m_InsertPointer;
	int m_SizeOfQueue;
};

template <class T>
CircularQueue<T>::CircularQueue()
{
	m_SizeOfQueue=QUEUE_MAX_ITEMS;
	m_ReaderPointer=0;
	m_InsertPointer=0;
    for (int i=0;i<QUEUE_MAX_ITEMS;i++)
    {
        m_QueueElements[i].IsCopied=true;
        memset(&m_QueueElements[i].object,0,sizeof(T));
    }
}

template <class T>
CircularQueue<T>::~CircularQueue()
{
}

/**
 * Add to Queue, it takes the address of object to be written
 * @returns returns zero if queue was full, otherwise returns the size of written bytes
 */
template <class T>
int CircularQueue<T>::ADD(const T *obj)
{
    std::unique_lock<std::mutex> lock(m_hwnd);
    if (m_InsertPointer==m_SizeOfQueue) m_InsertPointer=0;
    if (m_InsertPointer==m_ReaderPointer && m_QueueElements[m_ReaderPointer].IsCopied==false) 
		return 0;
    memcpy(&m_QueueElements[m_InsertPointer].object,obj,sizeof (T));
    m_QueueElements[m_InsertPointer].IsCopied = false;
    if (m_InsertPointer<m_SizeOfQueue)
        m_InsertPointer++;
    return sizeof (T);
}
/**
 * Read from Queue, it takes the address of destination object which filled with readed data from queue.
 * @returns returns zero if nothing was in the queue, otherwize returns true
 */
template <class T>
bool CircularQueue<T>::Read(T * obj)
{
    std::unique_lock<std::mutex> lock(m_hwnd);
    if(m_ReaderPointer==m_SizeOfQueue) m_ReaderPointer=0;
    if (m_QueueElements[m_ReaderPointer].IsCopied==true)
		return false;
    memcpy(obj,&m_QueueElements[m_ReaderPointer].object,sizeof(T));
    m_QueueElements[m_ReaderPointer].IsCopied = true;
    if (m_ReaderPointer<m_SizeOfQueue) m_ReaderPointer++;
    return true;
}

#endif // CIRCULARQEUE_H
