#ifndef SOCKET__HPP
#define SOCKET__HPP

#include <WinSock2.h>
#include <cstdlib>
#include <string>

class Socket
{
    public:
    enum INTERNET_PROTOCOL
    {
        IPv4,
        IPv6
    };

    enum TRANSMISSION_PROTOCOL
    {
        TCP,
        UDP
    };

    private:
    SOCKET nativeSocket = 0;

    public:
    static bool Init();
    static void Cleanup();

    Socket(
        TRANSMISSION_PROTOCOL _tp,
        INTERNET_PROTOCOL _ip = INTERNET_PROTOCOL::IPv4);
    Socket(SOCKET _nativeSocket);
    Socket();
    ~Socket();

    void Close();

    bool Connect(const std::string& _address, unsigned short _port);
    bool Bind(const std::string& _address, unsigned short _port);
    bool Bind(unsigned short _port);

    bool Listen();
    bool Accep(Socket& _outSocket);

    bool Send(const char* _data, size_t _size, size_t& _sentBytes);
    bool Receive(char* _buffer, size_t _bufferSize, size_t& _receivedBytes);

    private:
    static int GetNativeInternetProtocol(const INTERNET_PROTOCOL& _ip);
    static int GetNativeTransmissionProtocol(const TRANSMISSION_PROTOCOL& _tp);
    static int GetNativeSocketType(const TRANSMISSION_PROTOCOL& _tp);
};

#endif // !SOCKET__HPP