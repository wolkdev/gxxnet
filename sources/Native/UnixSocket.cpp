
#include "Socket.hpp"

#include <cstdlib>
#include <stdexcept>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int GetNativeSocketType(const Socket::PROTOCOL& _protocol)
{
    switch (_protocol)
    {
        case Socket::PROTOCOL::TCP: return SOCK_STREAM;
        case Socket::PROTOCOL::UDP: return SOCK_DGRAM;
        default: throw std::invalid_argument("Unknown Socket Protocol !");
    }
}

static Socket Socket::Create(PROTOCOL _protocol)
{
    Socket sock;
    sock.nativeSocket = socket(AF_INET, GetNativeSocketType(_protocol), 0);

    if (!sock.IsValid())
    {
        throw std::overflow_error("Cannot create unix socket !");
    }

    return sock;
}

void Socket::Close()
{
    close(nativeSocket);
    nativeSocket = 0;
}

bool Socket::IsValid()
{
    return nativeSocket != 0;
}

bool Socket::Connect(const std::string& _address, unsigned short _port)
{
    sockaddr_in sin;
    sin.sin_addr.s_addr = inet_addr(_address.c_str());
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return connect(nativeSocket, (sockaddr*)(&sin), sizeof(sin)) != -1;
}

bool Socket::Bind(const std::string& _address, unsigned short _port)
{
    sockaddr_in sin;
    sin.sin_addr.s_addr = inet_addr(_address.c_str());
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return bind(nativeSocket, (sockaddr*)(&sin), sizeof(sin)) != -1;
}

bool Socket::Bind(unsigned short _port)
{
    sockaddr_in sin;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return bind(nativeSocket, (sockaddr*)(&sin), sizeof(sin)) != -1;
}

bool Socket::Listen()
{
    return listen(nativeSocket, SOMAXCONN) != -1;
}

Socket Socket::Accept()
{
    Socket acceptedSock;
    acceptedSock.nativeSocket = accept(nativeSocket, NULL, NULL);

    return acceptedSock;
}

bool Socket::Send(const char* _data, size_t _size, size_t* _count)
{
    int returnedValue = write(nativeSocket, _data, _size);

    if (returnedValue != -1)
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
    int returnedValue = read(nativeSocket, _buffer, _bufferSize);

    if (returnedValue != -1)
    {
        if (_count != nullptr)
        {
            *_count = returnedValue;
        }

        return true;
    }

    return false;
}