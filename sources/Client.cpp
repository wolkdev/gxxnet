
#include "Client.hpp"

#include <memory>

Client::~Client()
{
    Close();
}

void Client::Close()
{
    if (remote != nullptr)
        delete remote;

    connectionTask.Stop();
    socket.Close();
}

void Client::Connect(std::string& _address, unsigned short _port)
{
    connectionTask.Start(std::bind(&Socket::Connect, &socket, _address, _port));
    connecting = true;
}

bool Client::Connected()
{
    return remote != nullptr;
}

bool Client::Connecting()
{
    return connecting;
}

void Client::Send(const Packet& _packet)
{
    if (remote != nullptr)
    {
        remote->PushToSendQueue(_packet);
    }
}

void Client::RegisterHandler(unsigned short _type, Handler _handler)
{
    handlers[_type] = _handler;
}

void Client::Update()
{
    if (connecting && !connectionTask.Running()) // On Task End
    {
        if (connectionTask.Succeded())
        {
            if (handlers[Message::TYPE::CONNECTION] != nullptr)
            {
                remote = new Remote(&socket);
                handlers[Message::TYPE::CONNECTION](
                    Message(Message::TYPE::CONNECTION));
            }
        }
        else
        {
            if (handlers[Message::TYPE::CONNECTION_FAILED] != nullptr)
            {
                handlers[Message::TYPE::CONNECTION_FAILED](
                    Message(Message::TYPE::CONNECTION_FAILED));
            }
        }

        connecting = false;
    }

    Message message;

    do
    {
        message = remote->GetNextReceivedMessage();

        if (message.type != Message::TYPE::EMPTY)
        {
            if (handlers[message.type] != nullptr)
            {
                handlers[message.type](message);
            }

            if (message.type == Message::TYPE::DISCONNECTION)
            {
                delete remote;
                remote = nullptr;
                break;
            }
        }

    } while (message.type != Message::TYPE::EMPTY
             && message.type != Message::TYPE::DISCONNECTION);
}