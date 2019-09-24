#ifndef HOST__HPP
#define HOST__HPP

#include "Socket.hpp"
#include "Task.hpp"
#include "Remote.hpp"

#include <vector>

class Host
{
    private:
    Socket socket;
    std::vector<Remote> remotes;

    Task acceptTask;

    public:
    Host();
    ~Host();

    void Listen(unsigned short _port);

    bool Accept();
};

#endif // !HOST__HPP