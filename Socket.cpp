#include "socket.hpp"

#include <stdexcept>

bool Socket::Init()
{
    WSAData data;

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        Cleanup();

        return false;
    }

    return true;
}

void Socket::Cleanup()
{
    WSACleanup();
}

Socket::Socket(SOCKET _nativeSocket)
{
    if (_nativeSocket == 0)
    {
        throw std::invalid_argument("Null native socket argument !");
    }

    nativeSocket = _nativeSocket;
}

Socket::Socket(Socket::PROTOCOL _protocol)
{
    int socketType =
        _protocol == Socket::PROTOCOL::TCP ? SOCK_STREAM : SOCK_DGRAM;

    // AF_INET = IPv4   AF_INET6 = IPv6
    nativeSocket = socket(AF_INET, socketType, GetNativeProtocol(_protocol));

    if (nativeSocket == 0)
    {
        throw std::overflow_error("Cannot create native socket !");
    }
}

Socket::~Socket()
{
    Close();
}

void Socket::Close()
{
    closesocket(nativeSocket);
}

bool Socket::Connect(const std::string& _address, const int& _port)
{
    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = inet_addr(_address.c_str());
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return connect(nativeSocket, reinterpret_cast<SOCKADDR*>(&sin), sizeof(sin))
        >= 0;
}

bool Socket::Bind(const std::string& _address, const int& _port)
{
    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = inet_addr(_address.c_str());
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return bind(nativeSocket, reinterpret_cast<SOCKADDR*>(&sin), sizeof(sin))
        >= 0;
}

bool Socket::Listen()
{
    return listen(nativeSocket, SOMAXCONN) >= 0;
}

Socket* Socket::Accep()
{
    SOCKET clientNativeSocket = accept(nativeSocket, NULL, NULL);

    if (clientNativeSocket != 0)
    {
        return new Socket(clientNativeSocket);
    }

    return nullptr;
}

bool Socket::Send(const char* _data, size_t _size, size_t& _sentBytes)
{
    int returnedValue = send(nativeSocket, _data, _size, 0);

    if (returnedValue != SOCKET_ERROR)
    {
        _sentBytes = returnedValue;
        return true;
    }

    return false;
}

bool Socket::Receive(char* _buffer, size_t _bufferSize, size_t& _receivedBytes)
{
    int returnedValue = recv(nativeSocket, _buffer, _bufferSize, 0);

    if (returnedValue != SOCKET_ERROR)
    {
        _receivedBytes = returnedValue;
        return true;
    }

    return false;
}

int Socket::GetNativeProtocol(const Socket::PROTOCOL& _protocol)
{
    switch (_protocol)
    {
        case Socket::PROTOCOL::TCP: return IPPROTO_TCP;
        case Socket::PROTOCOL::UDP: return IPPROTO_UDP;
        default: return IPPROTO_TCP; // the defalut protocol is TCP
    }
}