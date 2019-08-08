#ifndef PEER__HPP
#define PEER__HPP

#include "Packet.hpp"
#include "Socket.hpp"

#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

class Peer
{
    public:
    typedef void (*Handler)(const Packet&);

    private:
    Socket socket;
    std::unordered_map<unsigned short, Handler> handlers;

    std::vector<Socket> connections;
    std::queue<Packet> sendingQueue;
    std::queue<Packet> receivedQueue;

    std::thread* acceptingThread = nullptr;
    std::thread* sendingThread = nullptr;
    std::thread* receivingThread = nullptr;

    std::mutex acceptingMutex;
    std::mutex sendingMutex;
    std::mutex receivingMutex;

    bool accepting = false;
    bool sending = false;
    bool receiving = false;

    public:
    Peer(
        Socket::TRANSMISSION_PROTOCOL _tp,
        Socket::INTERNET_PROTOCOL _ip = Socket::INTERNET_PROTOCOL::IPv4);
    ~Peer();

    void Connect(const std::string& _address, unsigned short _port);

    void Send(const Packet& _packet);

    void RegisterHandler(unsigned short _type, Handler _handler);

    void Update();

    private:
    void StartAccepting();
    void StartSending();
    void StartReceiving();

    void StopAccepting();
    void StopSending();
    void StopReceiving();

    void AsyncAcceptLoop();
    void AsyncSendLoop();
    void AsyncReceiveLoop();
};

#endif // !PEER__HPP