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

Socket::Socket(TRANSMISSION_PROTOCOL _tp, INTERNET_PROTOCOL _ip)
{
    nativeSocket = socket(
        GetNativeInternetProtocol(_ip),
        GetNativeSocketType(_tp),
        GetNativeTransmissionProtocol(_tp));

    if (nativeSocket == 0)
    {
        throw std::overflow_error("Cannot create native socket !");
    }
}

Socket::Socket(SOCKET _nativeSocket)
{
    if (_nativeSocket == 0)
    {
        throw std::invalid_argument("Null native socket argument !");
    }

    nativeSocket = _nativeSocket;
}

Socket::Socket() {}

Socket::~Socket()
{
    Close();
}

void Socket::Close()
{
    closesocket(nativeSocket);
}

bool Socket::Connect(const std::string& _address, unsigned short _port)
{
    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = inet_addr(_address.c_str());
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return connect(nativeSocket, reinterpret_cast<SOCKADDR*>(&sin), sizeof(sin))
        != SOCKET_ERROR;
}

bool Socket::Bind(const std::string& _address, unsigned short _port)
{
    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = inet_addr(_address.c_str());
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return bind(nativeSocket, reinterpret_cast<SOCKADDR*>(&sin), sizeof(sin))
        != SOCKET_ERROR;
}

bool Socket::Bind(unsigned short _port)
{
    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return bind(nativeSocket, reinterpret_cast<SOCKADDR*>(&sin), sizeof(sin))
        != SOCKET_ERROR;
}

bool Socket::Listen()
{
    return listen(nativeSocket, SOMAXCONN) != SOCKET_ERROR;
}

bool Socket::Accep(Socket& _outSocket)
{
    _outSocket.nativeSocket = accept(nativeSocket, NULL, NULL);
    return _outSocket.nativeSocket != 0;
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

int Socket::GetNativeInternetProtocol(const INTERNET_PROTOCOL& _ip)
{
    switch (_ip)
    {
        case INTERNET_PROTOCOL::IPv4: return AF_INET;
        case INTERNET_PROTOCOL::IPv6: return AF_INET6;
        default: return AF_INET6; // the defalut internet protocol is IPv6
    }
}

int Socket::GetNativeTransmissionProtocol(const TRANSMISSION_PROTOCOL& _tp)
{
    switch (_tp)
    {
        case TRANSMISSION_PROTOCOL::TCP: return IPPROTO_TCP;
        case TRANSMISSION_PROTOCOL::UDP: return IPPROTO_UDP;
        default: return IPPROTO_TCP; // the defalut transmission protocol is TCP
    }
}

int Socket::GetNativeSocketType(const TRANSMISSION_PROTOCOL& _tp)
{
    switch (_tp)
    {
        case TRANSMISSION_PROTOCOL::TCP: return SOCK_STREAM;
        case TRANSMISSION_PROTOCOL::UDP: return SOCK_DGRAM;
        default: return SOCK_STREAM; // the defalut transmission protocol is TCP
    }
}