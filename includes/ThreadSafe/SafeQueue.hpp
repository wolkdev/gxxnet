#ifndef SAFE_QUEUE__HPP
#define SAFE_QUEUE__HPP

#include "SpinLock.hpp"

#include <atomic>

template <typename T>
class SafeQueue
{
    public:
    struct ELEMENT
    {
        ELEMENT* previous = nullptr;
        T data;
    };

    private:
    ELEMENT* first = nullptr;
    ELEMENT* last = nullptr;
    std::atomic_size_t count;
    SpinLock spinLock;

    public:
    SafeQueue() { count = 0; }
    ~SafeQueue() { Clear(); }
    
    SafeQueue(const SafeQueue<T>& _other)
    {
        for (size_t i = 0; i < count; i++)
        {
            /* code */
        }

        count = (size_t)_other.count;
    }

    SafeQueue(SafeQueue<T>&& _other)
    {
        first = _other.first;
        last = _other.last;
        count = (size_t)_other.count;
    }

    size_t Count() { return count; }

    void Push(const T& _data)
    {
        ELEMENT* newElement = new ELEMENT();
        newElement->data = _data;

        spinLock.Lock();

        if (first != nullptr)
        {
            first->previous = newElement;
        }
        else
        {
            last = newElement;
        }

        first = newElement;
        count++;

        spinLock.Unlock();
    }

    void Push(T&& _data)
    {
        ELEMENT* newElement = new ELEMENT();
        newElement->data = std::move(_data);

        spinLock.Lock();

        if (first != nullptr)
        {
            first->previous = newElement;
        }
        else
        {
            last = newElement;
        }

        first = newElement;
        count++;

        spinLock.Unlock();
    }

    T Pop()
    {
        if (last != nullptr)
        {
            ELEMENT* toPop = last;
            T data = toPop->data;
            
            spinLock.Lock();

            last = toPop->previous;
            delete toPop;
            count--;

            spinLock.Unlock();

            return std::move(data);
        }

        throw std::logic_error("Nothing to Pop !");
    }

    void Clear()
    {
        ELEMENT* toRemove;

        spinLock.Lock();
        
        for (ELEMENT* e = last; e != nullptr;)
        {
            toRemove = e;
            e = e->previous;
            delete toRemove;
        }

        first = nullptr;
        last = nullptr;
        count = 0;

        spinLock.Unlock();
    }
};

#endif // !SAFE_QUEUE__HPP