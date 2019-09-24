
#include "Task.hpp"

#include <stdexcept>

Task::~Task()
{
    Stop();
}

void Task::Start(std::function<bool()> _function)
{
    Stop();

    running = true;
    thread = new std::thread(&Task::Once, this, _function);
}

void Task::StartLoop(std::function<bool()> _function)
{
    Stop();

    running = true;
    thread = new std::thread(&Task::Loop, this, _function);
}

void Task::Stop()
{
    running = false;

    if (thread != nullptr)
    {
        thread->join();
        delete thread;
        thread = nullptr;
    }
}

bool Task::Running()
{
    return running;
}

bool Task::Succeded()
{
    return !running && succeded;
}

void Task::Once(std::function<bool()> _function)
{
    succeded = _function();
    running = false;
}

void Task::Loop(std::function<bool()> _function)
{
    while (running)
    {
        succeded = _function();

        if (succeded == false)
        {
            running = false;
        }
    }
}