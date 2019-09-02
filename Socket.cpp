
#include "Socket.hpp"

#include <stdexcept>

#ifdef _WIN32
int Socket::counter = 0;
#endif

Socket::Socket()
{
#ifdef _WIN32

    if (counter == 0)
    {
        Init();
    }

    counter++;

#endif
}

Socket::Socket(PROTOCOL _protocol) : Socket()
{
    nativeSocket = socket(AF_INET, GetNativeSocketType(_protocol), 0);

    if (nativeSocket == 0)
    {
        throw std::overflow_error("Cannot create native socket !");
    }
}

Socket::~Socket()
{
    Close();

#ifdef _WIN32

    counter--;

    if (counter == 0)
    {
        Cleanup();
    }

#endif
}

void Socket::Close()
{
#ifdef _WIN32
    closesocket(nativeSocket);
#elif __unix__
    close(nativeSocket);
#endif
}

bool Socket::Connect(const std::string& _address, unsigned short _port)
{
    NATIVE_SOCKET_ADDRESS_IN sin;
    sin.sin_addr.s_addr = inet_addr(_address.c_str());
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return connect(nativeSocket, (NATIVE_SOCKET_ADDRESS*)(&sin), sizeof(sin))
        != NATVIVE_SOCKET_ERROR;
}

bool Socket::Bind(const std::string& _address, unsigned short _port)
{
    NATIVE_SOCKET_ADDRESS_IN sin;
    sin.sin_addr.s_addr = inet_addr(_address.c_str());
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return bind(nativeSocket, (NATIVE_SOCKET_ADDRESS*)(&sin), sizeof(sin))
        != NATVIVE_SOCKET_ERROR;
}

bool Socket::Bind(unsigned short _port)
{
    NATIVE_SOCKET_ADDRESS_IN sin;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);

    return bind(nativeSocket, (NATIVE_SOCKET_ADDRESS*)(&sin), sizeof(sin))
        != NATVIVE_SOCKET_ERROR;
}

bool Socket::Listen()
{
    return listen(nativeSocket, SOMAXCONN) != NATVIVE_SOCKET_ERROR;
}

bool Socket::Accep(Socket& _outSocket)
{
    _outSocket.nativeSocket = accept(nativeSocket, NULL, NULL);
    return _outSocket.nativeSocket != 0;
}

bool Socket::Send(const char* _data, size_t _size, size_t& _sentBytes)
{
#ifdef _WIN32
    int returnedValue = send(nativeSocket, _data, _size, 0);
#elif __unix__
    int returnedValue = write(nativeSocket, _data, _size);
#endif

    if (returnedValue != NATVIVE_SOCKET_ERROR)
    {
        _sentBytes = returnedValue;
        return true;
    }

    return false;
}

bool Socket::Receive(char* _buffer, size_t _bufferSize, size_t& _receivedBytes)
{
#ifdef _WIN32
    int returnedValue = recv(nativeSocket, _buffer, _bufferSize, 0);
#elif __unix__
    int returnedValue = read(nativeSocket, _buffer, _bufferSize);
#endif

    if (returnedValue != NATVIVE_SOCKET_ERROR)
    {
        _receivedBytes = returnedValue;
        return true;
    }

    return false;
}

int Socket::GetNativeSocketType(const PROTOCOL& _protocol)
{
    switch (_protocol)
    {
        case PROTOCOL::TCP: return SOCK_STREAM;
        case PROTOCOL::UDP: return SOCK_DGRAM;
        default: return SOCK_STREAM; // the defalut transmission protocol is TCP
    }
}

#ifdef _WIN32

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

#endif