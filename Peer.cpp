
#include "Peer.hpp"

Peer::Peer(Socket::TRANSMISSION_PROTOCOL _tp, Socket::INTERNET_PROTOCOL _ip) :
    socket(_tp, _ip)
{
}

Peer::~Peer()
{
    if (sending)
    {
        StopSending();
    }

    if (receiving)
    {
        StopReceiving();
    }
}

void Peer::Connect(const std::string& _address, unsigned short _port)
{
    socket.Connect(_address, _port);

    if (!receiving)
    {
        StartReceiving();
    }
}

void Peer::Send(const Packet& _packet)
{
    sendingMutex.lock();
    sendingQueue.push(_packet);
    sendingMutex.unlock();

    if (!sending)
    {
        StartSending();
    }
}

void Peer::RegisterHandler(unsigned short _type, Handler _handler)
{
    handlers[_type] = _handler;
}

void Peer::Update()
{
    receivingMutex.lock();

    for (size_t i = 0; i < receivedQueue.size(); i++)
    {
        const Packet& packet = receivedQueue.front();

        if (handlers[packet.Type()] != nullptr)
        {
            handlers[packet.Type()](packet);
            receivedQueue.pop();
        }
    }

    receivingMutex.unlock();
}

void Peer::StartAccepting()
{
    accepting = true;
    acceptingThread = new std::thread(&Peer::AsyncAcceptLoop, this);
}

void Peer::StartSending()
{
    sending = true;
    sendingThread = new std::thread(&Peer::AsyncSendLoop, this);
}

void Peer::StartReceiving()
{
    receiving = true;
    receivingThread = new std::thread(&Peer::AsyncReceiveLoop, this);
}

void Peer::StopAccepting()
{
    accepting = false;
    acceptingThread->join();
    delete acceptingThread;
    acceptingThread = nullptr;
}

void Peer::StopSending()
{
    sending = false;
    sendingThread->join();
    delete sendingThread;
    sendingThread = nullptr;
}

void Peer::StopReceiving()
{
    receiving = false;
    receivingThread->join();
    delete receivingThread;
    receivingThread = nullptr;
}

void Peer::AsyncAcceptLoop()
{
    Socket clientBaseSocket;

    while (accepting)
    {
        if (socket.Accep(clientBaseSocket))
        {
            connections.push_back(clientBaseSocket);

            receivingMutex.lock();
            receivedQueue.push(Packet(Packet::MSG_TYPE::CONNECTION));
            receivingMutex.unlock();
        }
    }
}

void Peer::AsyncSendLoop()
{
    size_t sizeSent;

    while (sending)
    {
        sendingMutex.lock();
        size_t pakcetNbrToSend = sendingQueue.size();
        sendingMutex.unlock();

        if (pakcetNbrToSend > 0)
        {
            sendingMutex.lock();
            const Packet& packet = sendingQueue.front();
            sendingMutex.unlock();

            if (socket.Send(packet.Data(), packet.Size(), sizeSent))
            {
                sendingMutex.lock();
                sendingQueue.pop();
                sendingMutex.unlock();
            }
        }
    }
}

void Peer::AsyncReceiveLoop()
{
    size_t sizeReceived;
    Packet packet;

    while (receiving)
    {
        if (socket.Receive(
                packet.Header(), sizeof(Packet::HEADER_SIZE), sizeReceived))
        {
            packet.AllocateBody();

            if (socket.Receive(
                    packet.Body(), sizeof(packet.BodySize()), sizeReceived))
            {
                receivingMutex.lock();
                receivedQueue.push(packet);
                receivingMutex.unlock();
            }
        }
        else
        {
            receivingMutex.lock();
            receivedQueue.push(Packet(Packet::MSG_TYPE::DISCONNECTION));
            receivingMutex.unlock();

            receiving = false;
        }
    }
}