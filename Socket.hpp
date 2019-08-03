#ifndef SOCKET__HPP
#define SOCKET__HPP

#include <WinSock2.h>
#include <cstdlib>
#include <string>

class Socket
{
    public:
    enum PROTOCOL
    {
        TCP,
        UDP
    };

    private:
    SOCKET nativeSocket = 0;

    public:

    static bool Init();
	static void Cleanup();

    Socket(SOCKET _nativeSocket);
    Socket(Socket::PROTOCOL _protocol);
    ~Socket();

    void Close();

    bool Connect(const std::string& _address, const int& _port);
    bool Bind(const std::string& _address, const int& _port);

    bool Listen();
    Socket* Accep();
    
    bool Send(const char* _data, size_t _size, size_t& _sentBytes);
    bool Receive(char* _buffer, size_t _bufferSize, size_t& _receivedBytes);

    private:
    static int GetNativeProtocol(const Socket::PROTOCOL& _protocol);
};

#endif // !SOCKET__HPP