#ifndef TASK__HPP
#define TASK__HPP

#include <functional>
#include <thread>

class Task
{
    private:
    std::thread* thread = nullptr;

    bool running = false;
    bool succeded = false;

    public:
    ~Task();

    void Start(std::function<bool()> _function);
    void StartLoop(std::function<bool()> _function);
    void Stop();

    bool Running();
    bool Succeded();

    void Once(std::function<bool()> _function);
    void Loop(std::function<bool()> _function);
};

#endif // !TASK__HPP