#ifndef REMOTE__HPP
#define REMOTE__HPP

#include "Message.hpp"
#include "Packet.hpp"
#include "Socket.hpp"
#include "Task.hpp"
#include "ThreadSafe/SafeQueue.hpp"

#include <memory>

class Remote
{
    private:
    Socket* assoiciatedSocket = nullptr;
    Task receiveTask;
    Task sendTask;

    SafeQueue<Message> receiveQueue;
    SafeQueue<Packet*> sendQueue;

    Packet buffer;

    public:
    Remote(Socket* _assoiciatedSocket);

    Message GetNextReceivedMessage();
    void PushToSendQueue(const Packet& _packet);

    private:
    bool Receive();
    bool Send();
};

#endif // !REMOTE__HPP