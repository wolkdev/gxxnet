#ifndef UNIX_SOCKET__HPP
#define UNIX_SOCKET__HPP

#include <cstdlib>
#include <stdexcept>
#include <string>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class Socket
{
    public:
    enum PROTOCOL
    {
        TCP,
        UDP
    };

    private:
    int unixSocket = 0;

    private:
    Socket() {}

    public:
    static Socket Create(PROTOCOL _protocol)
    {
        Socket sock;
        sock.unixSocket = socket(AF_INET, GetUnixSocketType(_protocol), 0);

        if (!sock.IsValid())
        {
            throw std::overflow_error("Cannot create unix socket !");
        }

        return sock;
    }

    void Close()
    {
        close(unixSocket);
        unixSocket = 0;
    }

    bool IsValid() { return unixSocket != 0; }

    bool Connect(const std::string& _address, unsigned short _port)
    {
        sockaddr_in sin;
        sin.sin_addr.s_addr = inet_addr(_address.c_str());
        sin.sin_family = AF_INET;
        sin.sin_port = htons(_port);

        return connect(unixSocket, (sockaddr*)(&sin), sizeof(sin)) != -1;
    }

    bool Bind(const std::string& _address, unsigned short _port)
    {
        sockaddr_in sin;
        sin.sin_addr.s_addr = inet_addr(_address.c_str());
        sin.sin_family = AF_INET;
        sin.sin_port = htons(_port);

        return bind(unixSocket, (sockaddr*)(&sin), sizeof(sin)) != -1;
    }

    bool Bind(unsigned short _port)
    {
        sockaddr_in sin;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(_port);

        return bind(unixSocket, (sockaddr*)(&sin), sizeof(sin)) != -1;
    }

    bool Listen() { return listen(unixSocket, SOMAXCONN) != -1; }

    Socket Accep()
    {
        Socket acceptedSock;
        acceptedSock.unixSocket = accept(unixSocket, NULL, NULL);

        return acceptedSock;
    }

    bool Send(const char* _data, size_t _size, size_t* _sentBytes = nullptr)
    {
        int returnedValue = write(unixSocket, _data, _size);

        if (returnedValue != -1)
        {
            if (_sentBytes != nullptr)
            {
                *_sentBytes = returnedValue;
            }

            return true;
        }

        return false;
    }

    bool Receive(char* _buffer, size_t _bufferSize, size_t* _receivedBytes = nullptr)
    {
        int returnedValue = read(unixSocket, _buffer, _bufferSize);

        if (returnedValue != -1)
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
    static int GetUnixSocketType(const PROTOCOL& _protocol)
    {
        switch (_protocol)
        {
            case PROTOCOL::TCP: return SOCK_STREAM;
            case PROTOCOL::UDP: return SOCK_DGRAM;
            default: throw std::invalid_argument("Unknown Socket Protocol !");
        }
    }
};

#endif // !UNIX_SOCKET__HPP