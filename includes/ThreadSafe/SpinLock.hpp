#ifndef SPIN_LOCK__HPP
#define SPIN_LOCK__HPP

#include <atomic>

class SpinLock
{
    std::atomic_flag locked = ATOMIC_FLAG_INIT;

    public:
    void Lock() { while (locked.test_and_set(std::memory_order_acquire)); }
    void Unlock() { locked.clear(std::memory_order_release); }
};

#endif // !SPIN_LOCK__HPP