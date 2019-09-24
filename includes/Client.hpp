#ifndef CLIENT__HPP
#define CLIENT__HPP

#include "Message.hpp"
#include "Socket.hpp"
#include "Packet.hpp"
#include "Task.hpp"
#include "Remote.hpp"

#include <string>
#include <atomic>
#include <unordered_map>

class Client
{
    public:
    typedef void (*Handler)(Message);

    private:
    Socket socket = Socket::Create(Socket::PROTOCOL::TCP);
    Remote* remote = nullptr;
    bool connecting = false;

    Task connectionTask;

    std::unordered_map<unsigned short, Handler> handlers;

    public:
    ~Client();

    void Close();

    void Connect(std::string& _address, unsigned short _port);

    bool Connected();
    bool Connecting();

    void Send(const Packet& _packet);
    void RegisterHandler(unsigned short _type, Handler _handler);

    void Update();
};

#endif // !CLIENT__HPP