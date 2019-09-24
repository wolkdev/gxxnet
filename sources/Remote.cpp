
#include "Remote.hpp"

Remote::Remote(Socket* _assoiciatedSocket)
{
    assoiciatedSocket = _assoiciatedSocket;

    receiveTask.StartLoop(std::bind(&Remote::Receive, this));
    receiveTask.StartLoop(std::bind(&Remote::Send, this));
}

Message Remote::GetNextReceivedMessage()
{
    if (receiveQueue.Count() > 0)
    {
        return receiveQueue.Pop();
    }

    return Message(Message::TYPE::EMPTY);
}

void Remote::PushToSendQueue(const Packet& _packet)
{
    sendQueue.Push(new Packet(_packet));
}

bool Remote::Receive()
{
    if (assoiciatedSocket->Receive(
            buffer.Header(), sizeof(Packet::HEADER_SIZE)))
    {
        buffer.AllocateBody();

        if (assoiciatedSocket->Receive(
                buffer.Body(), sizeof(buffer.BodySize())))
        {
            Message message(Message::TYPE::DATA);
            message.packet = new Packet(buffer);
            receiveQueue.Push(message);
            return true;
        }
    }

    receiveQueue.Push(Message(Message::TYPE::DISCONNECTION));
    return false;
}

bool Remote::Send()
{
    if (sendQueue.Count() > 0)
    {
        Packet* packet = sendQueue.Pop();
        bool success = assoiciatedSocket->Send(packet->Data(), packet->Size());
        delete packet;
        return success;
    }

    return false;
}