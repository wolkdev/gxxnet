
#include "Host.hpp"

void Host::Listen(unsigned short _port)
{
    acceptTask.StartLoop(std::bind(&Host::Accept, this));
}

bool Host::Accept()
{
    Socket newSocket = socket.Accept();

    if (newSocket.IsValid())
    {
        remotes.push_back(Remote(&newSocket));

        return true;
    }

    return false;
}