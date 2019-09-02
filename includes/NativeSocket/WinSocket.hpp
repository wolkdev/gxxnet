#ifndef WIN_SOCKET__HPP
#define WIN_SOCKET__HPP

#include <cstdlib>
#include <stdexcept>
#include <string>

#include <WS2tcpip.h>
#include <WinSock2.h>

class Socket
{
    public:
    enum PROTOCOL
    {
        TCP,
        UDP
    };

    private:
    SOCKET winSocket = 0;
    static int counter;

    private:
    Socket() {}

    public:
    static Socket Create(PROTOCOL _protocol)
    {
        if (counter == 0)
        {
            if (!WinSockAPIInit())
            {
                throw std::runtime_error("Cannot Init WSA !");
            }
        }

        Socket sock;
        sock.winSocket = socket(AF_INET, GetWinSocketType(_protocol), 0);

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

    void Close()
    {
        closesocket(winSocket);
        winSocket = 0;

        counter--;

        if (counter == 0)
        {
            WinSockAPICleanup();
        }
    }

    bool IsValid() { return winSocket != 0; }

    bool Connect(const std::string& _address, unsigned short _port)
    {
        SOCKADDR_IN sin;
        sin.sin_addr.s_addr = inet_addr(_address.c_str());
        sin.sin_family = AF_INET;
        sin.sin_port = htons(_port);

        return connect(winSocket, (SOCKADDR*)(&sin), sizeof(sin)) != SOCKET_ERROR;
    }

    bool Bind(const std::string& _address, unsigned short _port)
    {
        SOCKADDR_IN sin;
        sin.sin_addr.s_addr = inet_addr(_address.c_str());
        sin.sin_family = AF_INET;
        sin.sin_port = htons(_port);

        return bind(winSocket, (SOCKADDR*)(&sin), sizeof(sin)) != SOCKET_ERROR;
    }

    bool Bind(unsigned short _port)
    {
        SOCKADDR_IN sin;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(_port);

        return bind(winSocket, (SOCKADDR*)(&sin), sizeof(sin)) != SOCKET_ERROR;
    }

    bool Listen() { return listen(winSocket, SOMAXCONN) != SOCKET_ERROR; }

    Socket Accep()
    {
        Socket acceptedSock;
        acceptedSock.winSocket = accept(winSocket, NULL, NULL);

        return acceptedSock;
    }

    bool Send(const char* _data, size_t _size, size_t* _sentBytes = nullptr)
    {
        int returnedValue = send(winSocket, _data, _size, 0);

        if (returnedValue != SOCKET_ERROR)
        {
            if (_sentBytes != nullptr)
            {
                *_sentBytes = returnedValue;
            }

            return true;
        }

        return false;
    }

    bool
    Receive(char* _buffer, size_t _bufferSize, size_t* _receivedBytes = nullptr)
    {
        int returnedValue = recv(winSocket, _buffer, _bufferSize, 0);

        if (returnedValue != SOCKET_ERROR)
        {
            if (_receivedBytes != nullptr)
            {
                *_receivedBytes = returnedValue;
            }

            return true;
        }

        return false;
    }

    private:
    static int GetWinSocketType(const PROTOCOL& _protocol)
    {
        switch (_protocol)
        {
            case PROTOCOL::TCP: return SOCK_STREAM;
            case PROTOCOL::UDP: return SOCK_DGRAM;
            default: throw std::invalid_argument("Unknown Socket Protocol !");
        }
    }

    static bool WinSockAPIInit()
    {
        WSAData data;

        if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
        {
            WinSockAPICleanup();

            return false;
        }

        return true;
    }

    static void WinSockAPICleanup()
    {
        WSACleanup();
    }
};

int Socket::counter = 0;

#endif // !WIN_SOCKET__HPP