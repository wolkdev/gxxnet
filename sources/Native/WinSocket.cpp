
#include "Socket.hpp"

#include <cstdlib>
#include <stdexcept>

#include <WS2tcpip.h>
#include <WinSock2.h>

int counter = 0;

void WinSockAPICleanup()
{
    WSACleanup();
}

bool WinSockAPIInit()
{
    WSAData data;

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        WinSockAPICleanup();

        return false;
    }

    return true;
}

int GetNativeSocketType(const Socket::PROTOCOL& _protocol)
{
    switch (_protocol)
    {
        case Socket::PROTOCOL::TCP: return SOCK_STREAM;
        case Socket::PROTOCOL::UDP: return SOCK_DGRAM;
        default: throw std::invalid_argument("Unknown Socket Protocol !");
    }
}

Socket Socket::Create(PROTOCOL _protocol)
{
    if (counter == 0)
    {
        if (!WinSockAPIInit())
        {
            throw std::runtime_error("Cannot Init WSA !");
        }
    }

    Socket sock;
    sock.nativeSocket = socket(AF_INET, GetNativeSocketType(_protocol), 0);

    if (!sock.IsValid())
    {
        if (counter == 0)
        {
            WinSockAPICleanup();
        }

        throw std::overflow_error("Cannot create unix socket !");
    }

    counter++;
    return sock;
}

void Socket::Close()
{
    closesocket(nativeSocket);
    nativeSocket = 0;

    counter--;

    if (counter == 0)
    {
        WinSockAPICleanup();
    }
}

bool Socket::IsValid()
{
    return nativeSocket != 0;
}

bool Socket::Connect(const std::string& _address, unsigned short _port)
{
    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = inet_addr(_address.c_str());
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return connect(nativeSocket, (SOCKADDR*)(&sin), sizeof(sin))
        != SOCKET_ERROR;
}

bool Socket::Bind(const std::string& _address, unsigned short _port)
{
    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = inet_addr(_address.c_str());
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return bind(nativeSocket, (SOCKADDR*)(&sin), sizeof(sin)) != SOCKET_ERROR;
}

bool Socket::Bind(unsigned short _port)
{
    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return bind(nativeSocket, (SOCKADDR*)(&sin), sizeof(sin)) != SOCKET_ERROR;
}

bool Socket::Listen()
{
    return listen(nativeSocket, SOMAXCONN) != SOCKET_ERROR;
}

Socket Socket::Accept()
{
    Socket acceptedSock;
    acceptedSock.nativeSocket = accept(nativeSocket, NULL, NULL);

    return acceptedSock;
}

bool Socket::Send(const char* _data, size_t _size, size_t* _count)
{
    int returnedValue = send(nativeSocket, _data, _size, 0);

    if (returnedValue != SOCKET_ERROR)
    {
        if (_count != nullptr)
        {
            *_count = returnedValue;
        }

        return true;
    }

    return false;
}

bool Socket::Receive(char* _buffer, size_t _bufferSize, size_t* _count)
{
    int returnedValue = recv(nativeSocket, _buffer, _bufferSize, 0);

    if (returnedValue != SOCKET_ERROR)
    {
        if (_count != nullptr)
        {
            *_count = returnedValue;
        }

        return true;
    }

    return false;
}