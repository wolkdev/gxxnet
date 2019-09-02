#ifndef SOCKET__HPP
#define SOCKET__HPP

#include <cstdlib>
#include <string>

#ifdef _WIN32

#include <WS2tcpip.h>
#include <WinSock2.h>


typedef SOCKET NATIVE_SOCKET;

#define NATVIVE_SOCKET_ERROR SOCKET_ERROR

#elif __unix__

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


typedef int NATIVE_SOCKET;

#define NATVIVE_SOCKET_ERROR (-1)

#endif

typedef struct sockaddr NATIVE_SOCKET_ADDRESS;
typedef struct sockaddr_in NATIVE_SOCKET_ADDRESS_IN;
typedef struct sockaddr_storage NATIVE_SOCKET_ADDRESS_STORAGE;

class Socket
{
    public:
    enum PROTOCOL
    {
        TCP,
        UDP
    };

    private:
    NATIVE_SOCKET nativeSocket = 0;

    PROTOCOL protocol;

#ifdef _WIN32
    static int counter;
#endif

    public:
    Socket();
    Socket(PROTOCOL _protocol);
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
    static int GetNativeSocketType(const PROTOCOL& _protocol);

#ifdef _WIN32
    static bool Init();
    static void Cleanup();
#endif
};

#endif // !SOCKET__HPP